#!/bin/bash

set -e

make
echo Verifying output...
./sample-tests | diff -u - expected-output
echo Passed
