# RT_Unige Assignment 1

The repo should have a README.md file with:

"Description of the content of the package (nodes, custom messages or services (if any)).
	- The package name is square_robot, inside this packege we have 2 (two) .cpp files:
		- in robot_target.cpp we can find the node with name <call_robot_target>,  this node should implement a ROS publisher (cmd_vel, for setting the robot speed), a ROS subscriber (odom, for knowing the actual robot position) and a ROS client (for receiving the random target).
		- in square_robot_reply.cpp we can find the node <reply_target>, this node should implement a ROS server (for setting the random target).
	- The package <square_robot> also as a server of name "service.srv". 

	float32 RandX	------> Receive the minimum value of the robot space range
	float32 RandY	------> Receive the maximum value of the robot space range
	---
	float32 PosX	------> Atribute a value in the range for PosX
	float32 PosY	------> Atribute a value in the range for PosY

PS: DON'T FORGET to declare the nodes, servies and message in the CMakeLists.xt

"Computational graph of the system (how do nodes communicate? You may use rqt_graph).

	<insert rqt_graph>

"Instructions about how to run the code.

	Clone the package <square_robot> in your work space
	- Open a shell and type "roscore"
	- Open second shell and type "rosrun stage_ros stageros $(rospack find assignment1)/world/exercise.world"
	- Open a third and go to your work space and then type <catkin_make>, once it compiled 100% without errors, at the sma eshell type <rosrun square_robot reply_target>
	- Open a forth shell and then type <srosrun square_robot call_robot_target>

Once you did those steps, you see the square blue robot running in your screen.


