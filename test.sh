#!/bin/bash

RESET="\e[0m"
RED="\e[1;31m"
GREEN="\e[1;32m"
YELLOW="\e[1;33m"

if [ "$#" -ne 8 ]; then
    echo "usage: $(basename $0) <test name> <test id> <compiler> <compiler flags> <src path> <impl path> <out path> <baseline path>"
    exit 1
fi

test=$1
test_id=$2
compiler=$3
flags=$4
src_path=$5
impl_path=$6
bin_path=$7
baseline_path=$8

log_path="$bin_path.log"
out_path="$bin_path.out"
diff_path="$bin_path.diff"


executables=(wdiff $compiler awk sed getconf)
for executable in "${executables[@]}"; do
    if [ ! -x "$(command -v $executable)" ]; then
        echo -e "$RED[ERROR]$RESET $executable is not installed"
        exit 1
    fi
done


is_clang=$("$compiler" --version 2>&1 | grep -q 'clang' && echo true || echo false)
is_64bit=$([ "$(getconf LONG_BIT)" = "64" ] && echo true || echo false)

# Some tests fail on older versions of compilers
function should_skip {
    result=false

    if [ $is_clang = true ]; then
        major_version=$("$compiler" -dumpversion | cut -d. -f1)

        if [ $major_version -le 15 ]; then
            # Old bit fields format (not DWARFv5)
            if [ "$test" = "bits" ]; then result=true; fi
        fi

        if [ "$test" = "scoped_type" ]; then result=true; fi
    fi

    echo $result
}

# Disable address sanitizer for tests that are expected to leak memory.
function disable_asan {
    if   [ "$test" = "new" ]; then echo true;
    else echo false; fi
}

# Tests that #define options need to recompile the implementation.
function use_shared_implementation {
    if   [ "$test" = "depth_option" ];       then echo false;
    elif [ "$test" = "indentation_option" ]; then echo false;
    elif [ "$test" = "stdio_file" ];         then echo false;
    elif [ "$test" = "string_truncation" ];  then echo false;
    else echo true; fi
}

function get_similarity {
    result=100

    if [ $is_clang = true ]; then
        # These tests contain stdint.h types which have different typenames in gcc and clang.
        # Baselines are generated from gcc, so it decreases the similarity for clang:
        if   [ "$test" = "struct" ];        then result=90;
        elif [ "$test" = "packed_struct" ]; then result=90;
        elif [ "$test" = "primitives" ];    then result=95; fi
    else
        # GCC doesn't output the information about the second function.
        if [ "$test" = "reference" ]; then result=85; fi
    fi

    if [ $is_64bit = false ]; then
        # `(unsigned) long int` has different size depending on architecture.
        if [ "$test" = "ints" ]; then result=90; fi
    fi

    # FILE has different implementation depending on stdio.h and it often has pointers
    # that point out-of-bounds which prints garbage data.
    # The primary goal is to check that there are no errors, i.e. segfaults, leaks.
    if [ "$test" = "stdio_file" ]; then result=10;
    # When union is initialized with pointers, its number members are different every time.
    elif [ "$test" = "union" ]; then result=90;
    # Some GCC versions use "size_t" while others use "unsigned long int".
    elif [ "$test" = "function" ]; then result=85; fi

    echo $result
}


if [ $(should_skip) = true ]; then
    echo -e "$YELLOW[SKIPPING]$RESET $test_id"
    exit 0
fi

mkdir -p $(dirname $bin_path)
if [ $(use_shared_implementation) = true ]; then
    $compiler $flags $src_path $impl_path -o $bin_path > $log_path 2>&1
else
    $compiler $flags $src_path -o $bin_path > $log_path 2>&1
fi

if [ $? -ne 0 ]; then
    if [ $CI ]; then
        echo -e "$RED[COMPILATION FAILED]$RESET $test_id. Log:"
        cat $log_path
    else
        echo -e "$RED[COMPILATION FAILED]$RESET $test_id. Log: $log_path"
    fi
    exit 1
fi

if [ $(disable_asan) = true ]; then
    export ASAN_OPTIONS=detect_leaks=0
fi

./$bin_path > $out_path 2>&1
ret=$?
cat $out_path >> $log_path

if [ $ret -ne 0 ]; then
    if [ -n "$CI" ]; then
        echo -e "$RED[TEST FAILED]$RESET $test_id. Log:"
        cat $log_path
    else
        echo -e "$RED[TEST FAILED]$RESET $test_id. Log: $log_path. Binary: $bin_path."
    fi
    exit 1
fi

if [ ! -r $baseline_path ] || [ ! -s $baseline_path ]; then
    echo -e "$YELLOW[WARNING]$RESET There is no $test baseline."
    echo -e "$GREEN[TEST PASSED]$RESET $test_id: ?";
    exit 0
fi

# Replace pointers with "POINTER" since they differ between every execution.
sed -i -E "s/0x[0-9a-fA-F]{6,16}/POINTER/g" $baseline_path $out_path

similarity=$(wdiff -123 --statistics $baseline_path $out_path | awk -F ':' '{print $NF}' | awk -F ' ' '{print $4}' | sed 's/%$//' | sort -g | head -n 1)

echo "Similarity is $similarity%" >> $log_path
if [ $similarity -lt $(get_similarity) ]; then
    wdiff $baseline_path $out_path > $diff_path

    if [ -n "$CI" ]; then
        echo -e "$RED[DIFF FAILED]$RESET $test_id. Similarity is $similarity%, required $(get_similarity)%. Log:"
        cat $log_path
        echo "Diff:"
        cat $diff_path
    else
        echo -e "$RED[DIFF FAILED]$RESET $test_id. Similarity is $similarity%, required $(get_similarity)%. Log: $log_path. Diff: $diff_path."
    fi
    exit 1
fi

echo -e "$GREEN[TEST PASSED]$RESET $test_id: $similarity%";
