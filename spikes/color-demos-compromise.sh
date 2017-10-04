#!/bin/sh

# Pass, muted
printf "\n"
printf "PFSTest 0.2\n===========\n"
printf "core-test-cases.c:should_run \033[32mPASS\033[m\n"
printf "core-test-cases.c:should_also_run \033[32mPASS\033[m\n"
printf "\n"
printf 'Run complete. \033[32m2 passed\033[m, 0 failed, 0 ignored\n\n'

# Pass + Ignore, muted
printf "\n"
printf "PFSTest 0.2\n===========\n"
printf "core-test-cases.c:should_run \033[32mPASS\033[m\n"
printf "core-test-cases.c:should_be_ignored \033[33mIGNORED\033[m\n"
printf "core-test-cases.c:should_also_run \033[32mPASS\033[m\n"
printf "\n"
printf 'Run complete. \033[32m2 passed\033[m, 0 failed, \033[33m1 ignored\033[m\n\n'

# Fail, compromise
printf "\n"
printf "PFSTest 0.2\n===========\n"
printf "core-test-cases.c:should_run \033[32mPASS\033[m\n"
printf "core-test-cases.c:should_fail \033[31mFAIL\033[m\n"
printf "    Location: \033[1mcore-test-cases.c:22\033[m\n"
printf "    assert_that called with NULL value\n"
printf "core-test-cases.c:should_also_run \033[32mPASS\033[m\n"
printf "\n"
printf 'Run complete. 2 passed, \033[37;41;1m1 failed\033[m, 0 ignored\n\n'
