# Flocking simulation
This was a research project I did at Petnica science center (http://www.petnica.rs/), during the summer of 2010 after finishing my second year of high school.

The result of the research is a paper that was published in the 2010 Petnica science center students' projects journal (number 68).

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

##Example
These couple of examples are screenshots taken from the sanbox application. 

The light blue lines represent paths
computed by each entity using the A* algorithm. Some algorithms require these paths
to be recalculated every several frames, because the target
is moving (leader following and closest entity following), only the autonomous movement algorithm requires these
paths to be calculated only once, because the target is the goal position and it is known from the start.

The green colored enitites are the leaders, the ones that were initialy closest to the goal.

The dark blue colored circle is the goal position.

The pink colored blue circles are the entities.

The green colored polygons are the obstacles. The sandbox only supports the creation of convex obstacles, but they
can be overlapped yielding more complex (concave) shapes.

###Leader following
![Alt text](/../screenshots/img/leader.png?raw=true "Optional Title")
###Closest entity following
![Alt text](/../screenshots/img/closest.png?raw=true "Optional Title")
###Autonomous movement
![Alt text](/../screenshots/img/autonomous.png?raw=true "Optional Title")
