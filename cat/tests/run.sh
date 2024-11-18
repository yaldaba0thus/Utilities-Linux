#!/bin/bash

SUCCESS=0
FAIL=0
TOTAL=5
FILE="./tests/test.txt"
FLAGS="b e n s t"

make build || { echo "Build failed. Exiting."; exit 1; }

run_test() {
    local flag=$1
    local test_log="test_cat.log"
    local sys_cat_log="test_sys_cat.log"

    ./build/cat "$flag" "$FILE" > "$test_log"
    cat "$flag" "$FILE" > "$sys_cat_log"

    if diff -s "$test_log" "$sys_cat_log" &>/dev/null; then
        ((SUCCESS++))
        RESULT="SUCCESS"
    else
        ((FAIL++))
        RESULT="FAIL"
    fi

    echo "[${SUCCESS}/${TOTAL}] ${RESULT} cat $flag $FILE"
    rm "$test_log" "$sys_cat_log"
}

for flag in $FLAGS; do
    run_test "-$flag"
done

echo "SUCCESS: ${SUCCESS}"
echo "FAIL: ${FAIL}"

make clean