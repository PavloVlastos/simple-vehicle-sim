# simple-vehicle-sim
This repo holds a small example cmake project for a simple vehicle simulation (SVS). It uses a simple model, controller, and python script to graph the simulation in "real-time". This project is meant as an example project to run tools like IKOS wit (https://github.com/NASA-SW-VnV/ikos). 

## Build Options
There are three different ways to compile and run the code for SVS

### Quick-Build Option (Recommended)
Run the following commands to build simple-vehicle-sim using a bash script
1. `bash quick_build`
2. `./build/svs` Use `-h` for options

    Example command: `python visualize/visualizes_sim.py` This must be started first to start the tcp server if the `-t` option for `svs` is used (see below).

    In another console: `./build/svs --kp 1.0 -v -m 1000 -t --speed 5.0 --dt 0.05`

### Build with `make` 
Here are the commands to build simple-vehicle-sim using `make`
1. `cd make_primary`
2. `make clean`
3. `make` \
3.1 (optional) in a seperate terminal run `python visualize/visualizes_sim.py`
4. './bin/main -t -m 10000 --kp 1.1 -p 1`

### Build with `cmake` 
Run the following commands to build simple-vehicle-sim using `cmake`
1. `cd simple-vehilce-sim`
2. `mkdir build`
3. `cd build`
4. `cmake ..`
5. `make` \
5.1 (optional) in a seperate terminal run `python visualize/visualizes_sim.py`
6. `./svs --kp 1.0 -v -m 1000 -t --speed 5.0 --dt 0.05`


## IKOS
To use IKOS for static analysis:

### `ikos` command:
1. build svs-sim using the steps above
2. run `bash run_ikos.sh`
3. run `report.sh`

### `ikos-scan` command:
1. `cd make_primary`
2. `make clean`
3. `ikos-scan make`
4. When prompted to "Analyze bin/main? [Y/n]", type "Y"
5. You should see output like this:
```
# Time stats:
ikos-analyzer: 0.149 sec
ikos-pp      : 0.020 sec

# Summary:
Total number of checks                : 980
Total number of unreachable checks    : 14
Total number of safe checks           : 870
Total number of definite unsafe checks: 0
Total number of warnings              : 96
```

6. Try `ikos-report bin/main.db`
7. Try `ikos-view bin/main.db`

### `ikows-scan-extract`
1. Try using `ikos-scan-extract <file_name>.o`. This should generate a `<file_name>.bc`.
2. Try analyzing a program with a specific entry point. IKOS needs an entry point (main). If you have a library in the form of `<file_name>.o` already, you can specify your own main as an entry point:\
`ikos <file_name>.bc -e=<your_main_file>`