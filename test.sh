#!/bin/bash

RESET="\e[0m"
RED="\e[1;31m"
GREEN="\e[1;32m"
YELLOW="\e[1;33m"

if [ "$#" -ne 6 ]; then
    echo "usage: $(basename $0) <compiler_flags> <src> <test> <compiler> <o_level> <g_level>"
    exit 1
fi

flags=$1
src=$2
test=$3
compiler=$4
o_level=$5
g_level=$6

executables=(wdiff $compiler)
for executable in "${executables[@]}"; do
    if [ ! -x "$(command -v $executable)" ]; then
        echo -e "$RED[ERROR]$RESET $executable is not installed"
        exit 1
    fi
done

test_id="$test-$compiler-$o_level-$g_level"
dir="$BUILD_DIR/test/$test"
bin="$dir/$compiler-$o_level-$g_level"
log="$bin.log"
output="$bin.out"
baseline="$BASELINE_DIR/$test.out"

is_clang=false
if [ $(echo "$compiler" | head -c 5) = "clang" ]; then is_clang=true; fi

# Some tests fail on older versions of compilers
function should_skip {
    result=false

    if [ $is_clang = true ]; then
        major_version=$("$compiler" -dumpversion | cut -d. -f1)

        if [ $major_version -le 15 ]; then
            # Old bit fields format (not DWARFv5)
            if [ "$test" = "bits" ]; then result=true; fi
        fi
    fi

    echo $result
}

# Regular tests share single uprintf implementation, but option tests need their own.
function use_shared_implementation {
    if   [ "$test" = "depth_option" ];       then echo false;
    elif [ "$test" = "indentation_option" ]; then echo false;
    elif [ "$test" = "stdio_file" ];         then echo false;
    elif [ "$test" = "string_truncation" ];  then echo false;
    else echo true; fi
}

# Some tests may not match baseline exactly, so they get custom target similarity percentage.
function get_similarity {
    result=100

    if [ $is_clang = true ]; then
        # These tests contain stdint.h types which have different typenames in gcc and clang.
        # Baselines are generated from gcc, so decrease its similarity for clang:
        if   [ "$test" = "struct" ];        then result=90;
        elif [ "$test" = "packed_struct" ]; then result=90;
        elif [ "$test" = "primitives" ];    then result=95;
        # Clang doesn't produce DW_AT_subprogram for external functions, so cross-CU
        # retrieval of function definition doesn't work.
        # Also, some GCC versions use "size_t" while others use "unsigned long int".
        elif [ "$test" = "function" ]; then result=85; fi
    else
        # Some GCC versions use "size_t" while others use "unsigned long int".
        if [ "$test" = "function" ]; then result=95;
        # GCC doesn't output the information about the second function.
        elif [ "$test" = "reference" ]; then result=85; fi
    fi

    # FILE has different implementation depending on stdio.h and it often has pointers
    # that point out-of-bounds which prints garbage data.
    # The primary goal is to check that there are no errors, i.e. segfaults, leaks.
    if [ "$test" = "stdio_file" ]; then result=10;
    # When union is initialized with pointers, its number members are different every time.
    elif [ "$test" = "union" ]; then result=90; fi

    echo $result
}


if [ $(should_skip) = true ]; then
    echo -e "$YELLOW[SKIPPING]$RESET $test_id"
    exit 0
fi

# Compiling
mkdir -p $dir
if [ $(use_shared_implementation) = true ]; then
    object="$bin.o"
    implementation="$BUILD_DIR/impl/$compiler.o"

    if [ ! -r $implementation ]; then
        echo -e "$RED[COMPILATION FAILED]$RESET $test_id. \"$implementation\" doesn't exist.";
        exit 1
    fi

    $compiler $flags -$o_level -$g_level -c $src -o $object > $log 2>&1
    ret=$?
    $compiler $flags -$o_level -$g_level -o $bin $object $implementation >> $log 2>&1
else
    $compiler $flags -$o_level -$g_level -o $bin $src > $log 2>&1
    ret=$?
fi

if [ $ret -ne 0 ]; then
    if [ $CI ]; then
        echo -e "$RED[COMPILATION FAILED]$RESET $test_id. Log:"
        cat $log
    else
        echo -e "$RED[COMPILATION FAILED]$RESET $test_id. Log: $log"
    fi
    exit 1
fi

./$bin > $output 2>&1
ret=$?
cat $output >> $log

if [ $ret -ne 0 ]; then
    if [ $CI ]; then
        echo -e "$RED[TEST FAILED]$RESET $test_id. Log:"
        cat $log
    else
        echo -e "$RED[TEST FAILED]$RESET $test_id. Log: $log. Binary: $bin."
    fi
    exit 1
fi

if [ ! -r $baseline ] || [ ! -s $baseline ]; then
    echo -e "$YELLOW[WARNING]$RESET There is no $test baseline."
    echo -e "$GREEN[TEST PASSED]$RESET $test_id: ?";
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
        echo -e "$RED[DIFF FAILED]$RESET $test_id. Similarity is $similarity%, required $(get_similarity)%. Log:"
        cat $log
        echo "Diff:"
        cat $diff
    else
        echo -e "$RED[DIFF FAILED]$RESET $test_id. Similarity is $similarity%, required $(get_similarity)%. Log: $log. Diff: $diff."
    fi
    exit 1
fi

echo -e "$GREEN[TEST PASSED]$RESET $test_id: $similarity%";
