#!/bin/bash

input="$TEST_DIR/$1.c"
output_file="$1-$2-$3-$4"
baseline="$BASELINE_DIR/$1.out"
dir="$BUILD_DIR/test/$1"
bin="$dir/$output_file"
log="$bin.log"
output="$bin.out"

# Some tests may not match baseline exactly, so they get custom target similarity percentage.
function get_similarity {
    # These tests contain stdint.h types which have different typenames in gcc and clang
    if   [ "$1" = "struct" ]; then echo 90;
    elif [ "$1" = "packed" ]; then echo 90;
    # FILE has different implementation which is up to stdio.h and it often has pointers
    # that point out-of-bounds causing uprintf to print garbage from the memory, thus the
    # primary goal of the test is to check that there are no errors, i.e. segfaults, leaks.
    elif [ "$1" = "stdio_file" ]; then echo 1;
    else echo 100; fi
}

# Regular tests share single uprintf implementation, but option tests need their own.
function uses_shared_implementation {
    if   [ "$1" = "depth_option" ];       then echo false;
    elif [ "$1" = "indentation_option" ]; then echo false;
    elif [ "$1" = "stdio_file" ];         then echo false;
    else echo true; fi
}

# Compiling
mkdir -p $dir
if [ $(uses_shared_implementation $1) = false ]; then
    $2 $CFLAGS -Werror -$3 -$4 -o $bin $input > $log 2>&1
    ret=$?
else
    object="$bin.o"
    implementation="$BUILD_DIR/impl/$2.o"

    $2 $CFLAGS -Werror -$3 -$4 -c $input -o $object > $log 2>&1
    ret=$?
    $2 $CFLAGS -Werror -$3 -$4 -o $bin $object $implementation >> $log 2>&1
fi

if [ $ret -ne 0 ]; then
    echo "[COMPILATION FAILED] Log: $log. Rerun test: make $bin"
    exit 1
fi

# Running
./$bin > $output 2>&1
if [ $? -ne 0 ]; then
    echo "[TEST FAILED] Log: $log. Failed test binary: $bin. Rerun test: make $bin"
    exit 1
fi
cat $output >> $log

# Comparing
if [ ! -f $baseline ]; then
    echo "[WARNING] There is no baseline for $1!"
    echo "[TEST PASSED] $output_file: ?";
    exit 0
fi

sed -E "s/0x[0-9a-fA-F]{6,16}/POINTER/g" -i $baseline -i $output
similarity=$(wdiff -123 --statistics $baseline $output | \
    awk -F ':' '{print $NF}' | awk -F ' ' '{print $4}' | \
    sed 's/%$//' | sort | tail -n 1)
echo "Similarity is $similarity%" >> $log
if [ $similarity -lt $(get_similarity $1) ]; then
    diff="$bin.diff"
    wdiff $baseline $output > $diff
    
    echo "[DIFF FAILED] Similarity is $similarity%. Diff: $diff. Log: $log. Rerun test: make $bin"
    exit 1
fi

echo "[TEST PASSED] $output_file: $similarity%";
