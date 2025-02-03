#!/bin/bash
make clean
make UT_model

./bin/UT_model

mv ./obj/*.gcda coverage/.

gcov -o coverage/ UT_model.c