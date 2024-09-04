#!/bin/bash
rm -r ikos_generated
mkdir ikos_generated

ikos -I ../modules/common/ \
    -I ../modules/controller \
    -I ../modules/lin_alg \
    -I ../modules/interface \
    -I ../modules/model \
    -I ../modules/plan \
    UT_model.c -o ikos_generated/UT_model.db
    # -a=boa,dbz,nullity,prover,upa,uva,sio,uio,shc,poa,pcmp,sound,fca,dca,dfa,dbg,watch\
