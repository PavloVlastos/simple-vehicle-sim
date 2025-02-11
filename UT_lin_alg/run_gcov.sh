#!/bin/bash
make clean
make UT_lin_alg

./bin/UT_lin_alg

mv ./obj/*.gcda coverage/.

gcov -o coverage/ UT_lin_alg.c

lcov  -c --directory coverage/ --output-file coverage/UT_lin_alg.info

genhtml coverage/UT_lin_alg.info --output-directory coverage/