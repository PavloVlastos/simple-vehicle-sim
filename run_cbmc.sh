#!/bin/bash
cbmc --bounds-check \
 --pointer-check \
 --unwind 2 \
 -I modules/common \
 -I modules/controller \
 -I modules/interface \
 -I modules/lin_alg \
 -I modules/map \
 -I modules/model \
 -I modules/parse_args \
 -I modules/planner \
 main.c \
 modules/common/common.c \
 modules/controller/controller.c \
 modules/interface/interface.c \
 modules/lin_alg/lin_alg.c \
 modules/map/map.c \
 modules/model/model.c \
 modules/parse_args/parse_args.c \
 modules/planner/planner.c