# mav-sim
This repo holds a small example cmake project that simulates a simple miniature air vehicle (MAV). It uses a simple model, controller, and python script to graph the simulation in real-time. This project is meant as an example project to run tools like IKOS wit (https://github.com/NASA-SW-VnV/ikos). 

## Steps
Run the following commands to build mav-sim
1. `cd mav-sim`
2. `mkdir build`
3. `cmake ..`
4. `make`

or 

1. `bash quick_build`
2. `./build/mav_sim` Use `-h` for options

Example command:
`python visualize/visualizes_sim.py` This must be started first to start the tcp server
and in another console:
`./build/mav_sim --kp 1.0 -v -m 1000 -t --speed 5.0 --dt 0.05 `