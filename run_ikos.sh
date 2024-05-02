#!/bin/bash
ikos -I modules/common/ \
-I modules/controller \
-I modules/lin_alg \
-I modules/interface \
-I modules/model \
main.c