#!/bin/bash

if [ -d logout ]; then
    rm -rf logout
fi

if [ -r test-res.csv ]; then
    rm test-res.csv
fi

python /Users/apple/project/work/DXTester/caserunner.py -i test-case.json -rp test-res.csv -ld logout