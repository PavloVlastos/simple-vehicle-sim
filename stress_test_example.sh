#!/bin/bash
BINARY_PATH="./build/svs"

if [ ! -x "$BINARY_PATH" ]; then
    echo "Error: svs binary not found or not executable at $BINARY_PATH"
    echo "Please build the executable first: bash quick_build"
    exit 1
fi

python visualize/visualize_sim.py &

sleep 3

./build/svs --kp 1.5 -v -m 10000 -a --speed 10.0 --dt 0.01 -p 1 --stress-test

BINARY_PID=$!
kill $BINARY_PID
wait $BINARY_PID 2>/dev/null
echo "Example svs run complete"