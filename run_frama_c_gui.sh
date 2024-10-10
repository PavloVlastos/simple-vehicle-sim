#!/bin/bash
frama-c-gui -eva -cpp-command "gcc -C -E \
 -I ./modules \
 -I ./modules/common \
 -I ./modules/controller \
 -I ./modules/interface \
 -I ./modules/lin_alg \
 -I ./modules/map \
 -I ./modules/model \
 -I ./modules/parse_args \
 -I ./modules/planner" \
 main.c