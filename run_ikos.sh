#!/bin/bash
rm -r ikos_generated
mkdir ikos_generated

ikos -I modules/common/ \
    -I modules/controller \
    -I modules/lin_alg \
    -I modules/interface \
    -I modules/model \
    -I modules/plan \
    -I modules/map \
    -I modules/planner \
    main.c -o ikos_generated/main.db
    # -a=boa,dbz,nullity,prover,upa,uva,sio,uio,shc,poa,pcmp,sound,fca,dca,dfa,dbg,watch\

# ikos -I modules/common/ \
#      -I modules/lin_alg \
#      --entry-points=main,sum_vec\
#     UT_lin_alg/UT_lin_alg.c -o ikos_generated/UT_lin_alg.db
