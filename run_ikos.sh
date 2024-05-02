#!/bin/bash
rm -r ikos_generated
mkdir ikos_generated

ikos -I modules/common/ \
    -I modules/controller \
    -I modules/lin_alg \
    -I modules/interface \
    -I modules/model \
    main.c -o ikos_generated/main.db
    # -a=boa,nullity,dbz,prover,upa,uva,sio,uio,shc,poa

ikos -I modules/common/ secondary.c -o ikos_generated/secondary.db
