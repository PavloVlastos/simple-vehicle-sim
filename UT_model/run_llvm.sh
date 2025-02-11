#!/bin/bash

# Exit on any error
set -e

mkdir -p llvm_coverage

echo "Compiling..."
clang -fprofile-instr-generate -fcoverage-mapping \
    -I../modules/common \
    -I../modules/model \
    -I../modules/lin_alg \
    UT_model.c \
    ../modules/model/model.c \
    ../modules/lin_alg/lin_alg.c \
    -lm \
   -o llvm_coverage/UT_model

echo "Running tests..."
LLVM_PROFILE_FILE="llvm_coverage/UT_model.profraw" ./llvm_coverage/UT_model

echo "Generating coverage report..."
llvm-profdata merge -sparse llvm_coverage/UT_model.profraw -o llvm_coverage/UT_model.profdata

llvm-cov show llvm_coverage/UT_model \
    -instr-profile=llvm_coverage/UT_model.profdata \
    -show-line-counts-or-regions \
    -show-branches=count \
    -format=html \
    -o llvm_coverage/coverage_report \
    UT_model.c \
    ../modules/model/model.c \
    ../modules/lin_alg/lin_alg.c

llvm-cov report llvm_coverage/UT_model \
    -instr-profile=llvm_coverage/UT_model.profdata \
    UT_model.c \
    ../modules/model/model.c \
    ../modules/lin_alg/lin_alg.c

echo "Coverage report generated in llvm_coverage/coverage_report directory"