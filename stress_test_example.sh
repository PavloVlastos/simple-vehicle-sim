#!/bin/bash
BINARY_PATH="./build/svs"

if [ ! -x "$BINARY_PATH" ]; then
    echo "Error: svs binary not found or not executable at $BINARY_PATH"
    echo "Please build the executable first: bash quick_build"
    exit 1
fi

python visualize/visualize_sim.py &

sleep 3

# ./build/svs --kp 1.5 -v -m 10000 -a --speed 10.0 --dt 0.01 -p 1 --stress-test

cd ./make_primary \
&& make clean \
&& make

cd coverage
rm *.*
cd ..

./bin/main --kp 1.5 -v -m 100 -a --speed 10.0 --dt 0.01 -p 1 --stress-test

gcov -o ./coverage/ ../main.c 

mv ./obj/*.gcda coverage/.
mv ./obj/*.gcno coverage/.

lcov  -c --directory ./coverage/ --output-file ./coverage/main.info

ls ./coverage

genhtml ./coverage/main.info --output-directory ./coverage/

BINARY_PID=$!
kill $BINARY_PID
wait $BINARY_PID 2>/dev/null
echo "Stress test of main executable complete"

firefox ./coverage/index.html