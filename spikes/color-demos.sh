#!/bin/sh

MUTED=0
COMPROMISE=1
LOUD=0

if [ $MUTED -eq 1 -o $COMPROMISE -eq 1 ]; then

# Pass, muted
printf "\n"
printf "PFSTest 0.2\n===========\n"
printf "core-test-cases.c:should_run \033[32mPASS\033[0m\n"
printf "core-test-cases.c:should_also_run \033[32mPASS\033[0m\n"
printf "\n"
printf 'Run complete. \033[32m2 passed\033[0m, 0 failed, 0 ignored\n\n'

# Pass + Ignore, muted
printf "\n"
printf "PFSTest 0.2\n===========\n"
printf "core-test-cases.c:should_run \033[32mPASS\033[0m\n"
printf "core-test-cases.c:should_be_ignored \033[33mIGNORED\033[0m\n"
printf "core-test-cases.c:should_also_run \033[32mPASS\033[0m\n"
printf "\n"
printf 'Run complete. \033[32m2 passed\033[0m, 0 failed, \033[33m1 ignored\033[0m\n\n'

if [ $MUTED -eq 1 ]; then
# Fail, muted
printf "\n"
printf "PFSTest 0.2\n===========\n"
printf ".\n"
printf "core-test-cases.c:should_fail \033[31mFAIL\033[0m\n"
printf "    Location: core-test-cases.c:22\n"
printf "    assert_that called with NULL value\n"
printf ".\n"
printf 'Run complete. 2 passed, \033[31m1 failed\033[0m, 0 ignored\n\n'
fi

if [ $COMPROMISE -eq 1 ]; then
# Fail, compromise
printf "\n"
printf "PFSTest 0.2\n===========\n"
printf "core-test-cases.c:should_run \033[32mPASS\033[0m\n"
printf "core-test-cases.c:should_fail \033[31mFAIL\033[0m\n"
printf "    Location: \033[37;1mcore-test-cases.c:22\033[0m\n"
printf "    assert_that called with NULL value\n"
printf "core-test-cases.c:should_also_run \033[32mPASS\033[0m\n"
printf "\n"
printf 'Run complete. 2 passed, \033[37;41m1 failed\033[0m, 0 ignored\n\n'
fi
fi

if [ $LOUD -eq 1 ]; then

# Pass, loud
printf "\n"
printf "PFSTest 0.2\n===========\n"
printf "..\n"
printf '\033[42;37m\033[KRun complete. 2 passed, 0 failed, 0 ignored\033[0m\n\n'

# Pass + Ignore, loud
printf "\n"
printf "PFSTest 0.2\n===========\n"
printf ".I.\n"
printf '\033[42;37m\033[KRun complete. 2 passed, 0 failed, \033[33m1 ignored\033[0m\n\n'

# Fail, loud
printf "\n"
printf "PFSTest 0.2\n===========\n"
printf ".\n"
printf "core-test-cases.c:should_fail FAIL\n"
printf "    Location: core-test-cases.c:22\n"
printf "    assert_that called with NULL value\n"
printf ".\n"
printf "\n"
printf '\033[41;37m\033[KRun complete. 2 passed, 1 failed, 0 ignored\033[0m\n\n'

# Fail + Ignore, loud
printf "\n"
printf "PFSTest 0.2\n===========\n"
printf "I\n"
printf "core-test-cases.c:should_fail \033[41;37mFAIL\033[0m\n"
printf "    Location: core-test-cases.c:22\n"
printf "    assert_that called with NULL value\n"
printf ".\n"
printf "\n"
printf '\033[41;37m\033[KRun complete. 1 passed, 1 failed, \033[33m1 ignored\033[0m\n\n'

fi
