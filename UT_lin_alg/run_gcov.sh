#!/bin/bash
make clean
make UT_lin_alg

./bin/UT_lin_alg

mv ./obj/*.gcda coverage/.

gcov -o coverage/ UT_lin_alg.c