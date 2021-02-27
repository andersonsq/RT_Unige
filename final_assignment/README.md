# RT_Unige Assignment 2

"Description of the content of the package (nodes, custom messages or services)
- In this README file, you will find a brief description of the functionality of a robot using a simulated envrironment in GAZEBO and Rviz, they are:
	1) move randomly in the environment, by choosing 1 out of 6 possible target positions:[(-4,-3);(-4,2);(-4,7);(5,-7);(5,-3);(5,1)], implementing a random position service as in the assignment1
	2) directly ask the user for the next target position (checking that the position is one of the possibl esix) and reach it
	3) start following the external walls
	4) stop in the last position
For futher informations about the nodes and services, please chack the code random_positoin.cpp and user_interface.cpp, there you'll find this informations commented in the code.
	

Computational graph of the system 


"Instructions about how to run the code."Roslaunch(if needed, even more than one roslaunch files) should be used to start the simulation and all necessary nodes.
	- To launch the simulation after you'll have to:
		- open a terminal and write: roslaunch final_assignment simulation_gmapping.launch 
		- open a terminal and write: roslaunch final_assignment move_base.launch
		- open a terminal and write: rosrun final_assignment target_reply
		- open a terminal and write: rosrun final_assignment call_target
		- open a terminal and write: rosrun final_assignment wall_follow_service_m.py
	
	After that, follow the instructions as in the user_interface terminal.


- Robot behaviors implemented
	
	When in automatic mode, the robot will receive a random position that is generated in the random_positoin.cpp and will try to reach it.
	When in manual mode, the user will have the oportunity to choose 1 out 6 possible targets in the environment.
	When in follow_the_wall mode, the robot will explore the environment following the wall avoiding obstacles.
	To stop in the last position, just choose option 4 or 5 in the menu.

- Describing the software architecture and the architectural choices made
	For that, please check both ".cpp" files, it's easier to follow the sequency following the code comments at the same time.

- Describing system's limitations and possible improvements
	- The system appears to have a diffcult to receive new commands after reach the target goal. So, while, the robot hasn't reached the goal position, it can receive any new command without problems; like a new manual or automatic random position.
	- For the follow external wall option, the robot won't stop while the user doesn't kill/terrrminate the option directly in the terminal with "Ctrl+z".










