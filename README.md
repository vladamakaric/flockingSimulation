# Flocking simulation
A sandbox for flocking algorithms comparison. 
Here flocking means movement of a group of entities to a common goal, 
without overlapping with each other and avoiding obstacles in the terrain.

Three flocking algorithms (behaviors) are implemented:

1. Leader following: Every entity follows the leader
2. Closest entity following: Every entity follows the one closest to it.
3. Autonomous movement: Every entity knows the path to the goal. 

##Dependencies, compilation
The only library needed to compile this is [libsdl1.2](https://www.libsdl.org/download-1.2.php)
For compilation just use the makefile.

##Instructions

Run the application from the console. Type the number of the algorithm you want to run. 
After that type 0 to create a new simulation. Press ENTER.

![Alt text](/../screenshots/img/consoleScreen.png?raw=true "Optional Title")

After that you will get a new window with a blank screen, where you must define obstacles,
goal and entity positions, the instructions for doing so are given in the console window.

# flockingSimulation
Sandbox for flocking algorithms comparison. 

