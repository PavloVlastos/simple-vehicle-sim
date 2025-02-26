#!/bin/bash
BINARY_PATH="./bin/main"

cd ./make_primary \
&& make clean \
&& make

cd coverage
rm *.*
cd ..

mv ./obj/*.gcno ./coverage/.

if [ ! -x "$BINARY_PATH" ]; then
    echo "Error: svs binary not found or not executable at $BINARY_PATH"
    echo "Please build the executable first: bash main.sh"
    exit 1
fi

python ../visualize/visualize_sim.py &

sleep 3

./bin/main --kp 1.5 -v -m 1000 -a --speed 10.0 --dt 0.01 -p 1 
#--stress-test

gcov -o ./coverage/ ../main.c 

mv ./obj/*.gcda coverage/.
mv ./obj/*.gcno coverage/.

lcov  -c --directory ./coverage/ --output-file ./coverage/main.info

ls ./coverage

genhtml ./coverage/main.info --output-directory ./coverage/

firefox ./coverage/index.html