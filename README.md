# simple-vehicle-sim
This repo holds a small example cmake project for a simple vehicle simulation (SVS). It uses a simple model, controller, and python script to graph the simulation in "real-time". This project is meant as an example project to run tools like IKOS wit (https://github.com/NASA-SW-VnV/ikos). 

## Steps
Run the following commands to build svs-sim using `cmake`
1. `cd svs-sim`
2. `mkdir build`
3. `cmake ..`
4. `make`

or 

1. `bash quick_build`
2. `./build/svs_sim` Use `-h` for options

Example command:
`python visualize/visualizes_sim.py` This must be started first to start the tcp server if the `-t` option for `svs_sim` is used (see below).

In another console:
`./build/svs_sim --kp 1.0 -v -m 1000 -t --speed 5.0 --dt 0.05 `

Another option is you can build svs-sim using `make`:
1. `cd make_primary`
2. `make clean`
3. `make`

## IKOS
To use IKOS for static analysis:
1. build svs-sim using the steps above
2. run `bash run_ikos.sh`
3. run `report.sh`

Try using `ikos-view`, for example, after generating the .db files with `run_ikos.sh`,
1. `cd ikos_generated`
2. `ikos-view main.db` or `ikos-view secondary.db` (make sure you're running a desktop version).

Try using `ikos-scan-extract <file_name>.o`. This should generate a `<file_name>.bc`.

Try analyzing a program with a specific entry point. IKOS needs an entry point (main). If you have a library in the form of `<file_name>.o` already, you can specify your own main as an entry point:\
`ikos <file_name>.bc -e=<your_main_file>`