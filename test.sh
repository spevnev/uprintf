#!/bin/bash

if [ "$#" -ne 4 ]; then
    echo "usage: ./test.sh [test] [compiler] [o_level] [g_level]"
    exit 1
fi

test=$1
compiler=$2
o_level=$3
g_level=$4


if [ ! -x "$(command -v wdiff)" ]; then
    echo "[ERROR] wdiff is required, but isn't installed."
    exit 1
fi

if [ -x "$compiler" ]; then
    echo "[ERROR] $compiler doesn't exist or isn't executable,"
    exit 1
fi


test_id="$test-$compiler-$o_level-$g_level"
dir="$BUILD_DIR/test/$test"
src="$TEST_DIR/$test.c"
bin="$dir/$compiler-$o_level-$g_level"
log="$bin.log"
output="$bin.out"
baseline="$BASELINE_DIR/$test.out"
is_gcc=false
if [ $(echo "$compiler" | head -c 3) = "gcc" ]; then is_gcc=true; fi


# Some tests don't work on older versions of compilers
function should_skip {
    result=false

    if [ $is_gcc = false ]; then
        major_version=$("$compiler" -dumpversion | cut -d. -f1)

        if [ $major_version -le 15 ]; then
            # uses old (non-v5) format for bit fields
            if [ "$test" = "bits" ]; then result=true; fi
        fi
    fi

    echo $result
}

# Regular tests share single uprintf implementation, but option tests need their own.
function uses_shared_implementation {
    if   [ "$test" = "depth_option" ];       then echo false;
    elif [ "$test" = "indentation_option" ]; then echo false;
    elif [ "$test" = "stdio_file" ];         then echo false;
    elif [ "$test" = "string_truncation" ];  then echo false;
    else echo true; fi
}

# Some tests may not match baseline exactly, so they get custom target similarity percentage.
function get_similarity {
    result=100

    if [ $is_gcc = false ]; then
        # These tests contain stdint.h types which have different typenames in gcc and clang.
        # Baselines are generated from gcc, so decrease its similarity for clang:
        if   [ "$test" = "struct" ];        then result=90;
        elif [ "$test" = "packed_struct" ]; then result=90;
        elif [ "$test" = "primitives" ];    then result=95; fi
    fi

    # FILE has different implementation which is up to stdio.h and it often has pointers
    # that point out-of-bounds causing uprintf to print garbage from the memory, thus the
    # primary goal of the test is to check that there are no errors, i.e. segfaults, leaks.
    if [ "$test" = "stdio_file" ]; then result=10;
    # Clang doesn't produce DW_AT_subprogram for external functions, i.e. shared libraries
    # or different CUs, so cross-CU retrieval of function definition doesn't work.
    # Also, some GCC versions use "size_t" while others use "unsigned long int".
    elif [ "$test" = "function" ]; then result=85; fi

    echo $result
}


if [ $(should_skip) = true ]; then
    echo "[SKIPPING] $test_id"
    exit 0
fi

# Compiling
mkdir -p $dir
if [ $(uses_shared_implementation) = true ]; then
    object="$bin.o"
    implementation="$BUILD_DIR/impl/$compiler.o"

    if [ ! -r $implementation ]; then
        echo "[COMPILATION FAILED] $test_id. \"$implementation\" doesn't exist.";
        exit 1
    fi

    $compiler $CFLAGS -Werror -$o_level -$g_level -c $src -o $object > $log 2>&1
    ret=$?
    $compiler $CFLAGS -Werror -$o_level -$g_level -o $bin $object $implementation >> $log 2>&1
else
    $compiler $CFLAGS -Werror -$o_level -$g_level -o $bin $src > $log 2>&1
    ret=$?
fi

if [ $ret -ne 0 ]; then
    if [ $CI ]; then
        echo "[COMPILATION FAILED] $test_id. Log:"
        cat $log
    else
        echo "[COMPILATION FAILED] $test_id. Log: $log"
    fi
    exit 1
fi

./$bin > $output 2>&1
ret=$?
cat $output >> $log

if [ $ret -ne 0 ]; then
    if [ $CI ]; then
        echo "[TEST FAILED] $test_id. Log:"
        cat $log
    else
        echo "[TEST FAILED] $test_id. Log: $log. Binary: $bin."
    fi
    exit 1
fi

if [ ! -r $baseline ]; then
    echo "[WARNING] There is no $test baseline."
    echo "[TEST PASSED] $test_id: ?";
    exit 0
fi

# Replace pointers with "POINTER" since they differ between every execution.
sed -E "s/0x[0-9a-fA-F]{6,16}/POINTER/g" -i $baseline -i $output

diff=$(wdiff -123 --statistics $baseline $output)
similarity=$(echo "$diff" | awk -F ':' '{print $NF}' | awk -F ' ' '{print $4}' | sed 's/%$//' | sort -g | head -n 1)

echo "Similarity is $similarity%" >> $log
if [ $similarity -lt $(get_similarity) ]; then
    diff="$bin.diff"
    wdiff $baseline $output > $diff

    if [ $CI ]; then
        echo "[DIFF FAILED] $test_id. Similarity is $similarity%, required $(get_similarity)%. Log:"
        cat $log
        echo "Diff:"
        cat $diff
    else
        echo "[DIFF FAILED] $test_id. Similarity is $similarity%, required $(get_similarity)%. Log: $log. Diff: $diff."
    fi
    exit 1
fi

echo "[TEST PASSED] $test_id: $similarity%";
