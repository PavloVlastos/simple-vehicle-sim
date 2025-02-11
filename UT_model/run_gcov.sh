#!/bin/bash
make clean
make UT_model

./bin/UT_model

mv ./obj/*.gcda coverage/.

gcov -o coverage/ UT_model.c 

lcov  -c --directory coverage/ --output-file coverage/UT_model.info

genhtml coverage/UT_model.info --output-directory coverage/