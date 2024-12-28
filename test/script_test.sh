#!/bin/bash

set -eu

FILE=test_file
if [ -f $FILE ]; then
    echo "File Found! Test PASSED."
else
    echo "File not found! Test FAILED."
    exit 1
fi

exit 0
