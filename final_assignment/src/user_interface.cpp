#include "ros/ros.h"
#include "std_msgs/String.h"

#include <stdlib.h>
#include <stdio.h>

#include <iostream>
#include <sstream>

#include "nav_msgs/Odometry.h"
#include "geometry_msgs/Twist.h"

#include "move_base_msgs/MoveBaseActionGoal.h"
#include "std_srvs/SetBool.h"

#include <math.h>
#include "final_assignment/service.h"


	//Declare all variables as global
	ros::Publisher pub; 				//Declare the publisher as global variable with name "pub"
	ros::ServiceClient client, client2;		//Declare the client service as global variable with name "client"
	move_base_msgs::MoveBaseActionGoal goal;	//Declare with name goal
	std_srvs::SetBool follow_wall;			//Declare with name follow_wall
	final_assignment::service srv;			//Declare the service file as "srv"

	int X_pos, Y_pos;				//Global variable to store my X an Y values

//positonCallback LOOP
void positionCallback(const nav_msgs::Odometry::ConstPtr& msg)	//Callback is executed verytime I receive a new topic
{
	//Receive the position of my robot and print it on the shell
 	//remove the comment if you want to see in real time

	//ROS_INFO("The robot position is: [%f, %f]", msg->pose.pose.position.x, msg->pose.pose.position.y);

	 X_pos = msg->pose.pose.position.x;	//Actual position of robot in X axis
	 Y_pos = msg->pose.pose.position.y;	//Actual position of robot in Y axis
}

//SubscriberCallback LOOP to check if the robbbot is out of range
void subscriberCallback(const nav_msgs::Odometry::ConstPtr& msg)
{
	//Logic to check that I never receive a position outside my range
	if((msg->pose.pose.position.x > 6.0 || msg->pose.pose.position.x < -6.0) || (msg->pose.pose.position.y > 9.0 || msg->pose.pose.position.y < -9.0))
		{
		std::cout << "Robot out of range" << std::endl;
		ros::shutdown();
		}
}
	


int main(int argc, char **argv)
{	
	ros::init(argc, argv, "call_target");	//third argumment is the name of the node that I'm creating

	//Declare new nodes	
	ros::NodeHandle n;

	//Inicialize the clients
	client = n.serviceClient<final_assignment::service>("/setting_target");		
	client2 = n.serviceClient<std_srvs::SetBool>("/wall_follower_switch");		//check file "wall_follow_service_m.py" line 30	

	//Call the random position
	client.call(srv);

	//Initialize my publisher in /move_base
	pub = n.advertise<move_base_msgs::MoveBaseActionGoal>("move_base/goal", 1000);	//To fill the publisher, I need to use the comand "rostopic type /move_base

	//Subscribe and set a position in positionCall back function
	ros::Subscriber sub = n.subscribe("/odom", 1000, positionCallback); 		//I'm subscribing in "/odom"	 

	//Set the limits of my random number to be between -6 and 6
	srv.request.min = 0;		//--------> N		
	srv.request.max = 5;		//--------> M		

	//Created a mesage of type geometry_msgs Twist
	geometry_msgs::Twist vel;

	//Declare a delay of 2 seconds with name "r
	ros::Rate r(2);			

	int X = srv.response.PosX;	//store my random generated values in X
	int Y = srv.response.PosY;	//store my random generated values in Y

	int choice;			//variable 'int' to store the user choice
	char manual;			//variable 'char' to store the user choice when in manual


	//	1) move randomly in the environment, by choosing 1 out of 6 possible target positions:[(-4,-3);(-4,2);(-4,7);(5,-7);(5,-3);(5,1)], implementing a random position service as in the assignment 1

while(choice !=0){	//Loop to check the user choice

	std::cout << "Please choose one of the options bellow:"<< std::endl;
	std::cout << "Press 0 to END THE PROGRAM:"<< std::endl;
	std::cout << "Press 1 for automatic and random:"<< std::endl;
	std::cout << "Press 2 for manual:"<< std::endl;
	std::cout << "Press 3 to use the wall follower option:"<< std::endl;
	std::cout << "Press 4 to stop the robot in the last position:"<< std::endl;
	std::cout << "Press 5 to stop the robot in the last know random position:"<< std::endl;
	std::cin >> choice;	//atribute the user choice to the variable 'choice'

//Automatic random target
if(choice == 1)			
{
	client.call(srv);						//call a new random position
	goal.goal.target_pose.header.frame_id = "map";			//set the field to goal.target_pose.header.frame_id to 'map'
	goal.goal.target_pose.pose.orientation.w = 1;			//set the field to goal.target_pose.pose.orientation.w to '1'
	goal.goal.target_pose.pose.position.x = X;			//set the field to goal.target_pose.pose.position to the X received
	goal.goal.target_pose.pose.position.y = Y;			//set the field to goal.target_pose.pose.position to the Y received
	std::cout << "Your target in X is: " << X << std::endl;		//print in the user shell telling the position of the target in X axis
	std::cout << "Your target in Y is: " << Y << std::endl;		//print in the user shell telling the position of the target in Y axis
	r.sleep();							//delay of 2s
	pub.publish(goal);						//publish my goal

//while((X - X_pos >= 0.5 && X - X_pos < -0.5) && (Y - Y_pos >= 0.5 && Y - Y_pos < -0.5))
while((X != X_pos) && (Y != Y_pos))					//loop to check if the target is reached
	{
	std::cout << "Robot not on target X and Y" << std::endl;
	ros::spinOnce();
	r.sleep();		//delay of 2s
	std::cout << "Do you wish to continue in automatic mode? If yes, press 7 otherwise press 2 for manual mode"<< std::endl;
	std::cin >> choice;
	client.call(srv);	//call a new random position
	}
	//std::cout << "REACHED"<< std::endl;
	//std::cout << "Do you wish to continue in automatic mode? If yes, press 7 otherwise press any buttom to return to menu"<< std::endl;
	//std::cin >> choice;

//logic to keep in automatic mode with a new random target position
if(choice == 7)
	{
	client.call(srv);	//call a new random position	
	}
else {std::cout << "Leaving automatic mode"<< std::endl;}	
}


//	2) directly ask the user for the next target position (checking that the position is one of the possible six) and reach it

//Manual position target
if (choice == 2)
	{
	std::cout << "NICE, now you have the option to chose one of the 6 possible targets" << std::endl;
	std::cout << "Press A to go to possible_position[0] = {-4,-3}" << std::endl;
	std::cout << "Press B to go to possible_position[1] = {-4,-2}" << std::endl;
	std::cout << "Press C to go to possible_position[2] = {-4, 7}" << std::endl;
	std::cout << "Press D to go to possible_position[3] = {5,-7}" << std::endl;
	std::cout << "Press E to go to possible_position[4] = {5,-3}" << std::endl;
	std::cout << "Press F to go to possible_position[5] = {5, -1}" << std::endl;

	std::cin >> manual;

	switch(manual) {
	      case 'A' :
			goal.goal.target_pose.header.frame_id = "map";
			goal.goal.target_pose.pose.orientation.w = 1;
			goal.goal.target_pose.pose.position.x = -4;
			goal.goal.target_pose.pose.position.y = -3;
			std::cout << "You chose option A, X = -4 and Y = -3" << std::endl;
			pub.publish(goal);

			//if((goal.goal.target_pose.pose.position.x - X_pos <= 0.05 && goal.goal.target_pose.pose.position.x - X_pos > -0.05) && (goal.goal.target_pose.pose.position.y - Y_pos <= 0.05 && goal.goal.target_pose.pose.position.y - Y_pos > -0.05))
				while((X != X_pos) && (Y != Y_pos))
				{
				std::cout << "NOT REACHED" << std::endl;
				ros::spinOnce();
				r.sleep();		//delay of 2s
				}
		 break;
	      case 'B' :
			goal.goal.target_pose.header.frame_id = "map";
			goal.goal.target_pose.pose.orientation.w = 1;
			goal.goal.target_pose.pose.position.x = -4;
			goal.goal.target_pose.pose.position.y = -2;
			std::cout << "You chose option B, X = -4 and Y = -2" << std::endl;
			pub.publish(goal);

			//if((goal.goal.target_pose.pose.position.x - X_pos <= 0.05 && goal.goal.target_pose.pose.position.x - X_pos > -0.05) && (goal.goal.target_pose.pose.position.y - Y_pos <= 0.05 && goal.goal.target_pose.pose.position.y - Y_pos > -0.05))
				while((X != X_pos) && (Y != Y_pos))
				{
				std::cout << "NOT REACHED" << std::endl;
				ros::spinOnce();
				r.sleep();		//delay of 2s
				}
		 break;
	      case 'C' :
			goal.goal.target_pose.header.frame_id = "map";
			goal.goal.target_pose.pose.orientation.w = 1;
			goal.goal.target_pose.pose.position.x = -4;
			goal.goal.target_pose.pose.position.y = 7;
			std::cout << "You chose option C, X = -4 and Y = 7" << std::endl;
			pub.publish(goal);
			
			//if((goal.goal.target_pose.pose.position.x - X_pos <= 0.05 && goal.goal.target_pose.pose.position.x - X_pos > -0.05) && (goal.goal.target_pose.pose.position.y - Y_pos <= 0.05 && goal.goal.target_pose.pose.position.y - Y_pos > -0.05))
				while((X != X_pos) && (Y != Y_pos))
				{
				std::cout << "NOT REACHED" << std::endl;
				ros::spinOnce();
				r.sleep();		//delay of 2s
				}
		 break;
	      case 'D' :
			goal.goal.target_pose.header.frame_id = "map";
			goal.goal.target_pose.pose.orientation.w = 1;
			goal.goal.target_pose.pose.position.x = 5;
			goal.goal.target_pose.pose.position.y = -7;
			std::cout << "You chose option D, X = 5 and Y = -7" << std::endl;
			pub.publish(goal);

			//if((goal.goal.target_pose.pose.position.x - X_pos <= 0.05 && goal.goal.target_pose.pose.position.x - X_pos > -0.05) && (goal.goal.target_pose.pose.position.y - Y_pos <= 0.05 && goal.goal.target_pose.pose.position.y - Y_pos > -0.05))
				while((X != X_pos) && (Y != Y_pos))
				{
				std::cout << "NOT REACHED" << std::endl;
				ros::spinOnce();
				r.sleep();		//delay of 2s
				}
		 break;
	      case 'E' :
			goal.goal.target_pose.header.frame_id = "map";
			goal.goal.target_pose.pose.orientation.w = 1;
			goal.goal.target_pose.pose.position.x = 5;
			goal.goal.target_pose.pose.position.y = -3;
			std::cout << "You chose option E, X = 5 and Y = -3" << std::endl;
			pub.publish(goal);

			//if((goal.goal.target_pose.pose.position.x - X_pos <= 0.05 && goal.goal.target_pose.pose.position.x - X_pos > -0.05) && (goal.goal.target_pose.pose.position.y - Y_pos <= 0.05 && goal.goal.target_pose.pose.position.y - Y_pos > -0.05))
				while((X != X_pos) && (Y != Y_pos))
				{
				std::cout << "NOT REACHED" << std::endl;
				ros::spinOnce();
				r.sleep();		//delay of 2s
				}
		 break;
	      case 'F' :
			goal.goal.target_pose.header.frame_id = "map";
			goal.goal.target_pose.pose.orientation.w = 1;
			goal.goal.target_pose.pose.position.x = 5;
			goal.goal.target_pose.pose.position.y = -1;
			std::cout << "You chose option F, X = 5 and Y = -1" << std::endl;
			pub.publish(goal);

			//if((goal.goal.target_pose.pose.position.x - X_pos <= 0.05 && goal.goal.target_pose.pose.position.x - X_pos > -0.05) && (goal.goal.target_pose.pose.position.y - Y_pos <= 0.05 && goal.goal.target_pose.pose.position.y - Y_pos > -0.05))
				while((X != X_pos) && (Y != Y_pos))
				{
				std::cout << "NOT REACHED" << std::endl;
				ros::spinOnce();
				r.sleep();		//delay of 2s
				}
		 break;
	      default :
		 std::cout << "Invalid option, try again" << std::endl;
   			}
	}


		// 3) start following the external walls

//logic to follow the wall exploring the environment
if (choice == 3)
	{
	std::cout << "NICE, now your robot will follow the wall exploring the environment" << std::endl;
	follow_wall.request.data = true;		//request to the 
	client2.call(follow_wall);			//call the service to follow the wall
	std::cout << "Press 8 to stop following the wall" << std::endl;
	std::cin >> choice;
		if(choice == 8)
		{
		follow_wall.request.data = false;		//request to the
		pub.publish(goal);
		std::cout << "Robot not following the wall anymore" << std::endl; 
		}
		else
		{
		std::cout << "wrong number" << std::endl;
		}
	}


		//4) stop in the last position

//logic to the robot go back to the last know position 
if (choice == 4)
	{
	std::cout << "Your robot will stop at the last position" << std::endl;
	follow_wall.request.data = false;
	client2.call(follow_wall);
	goal.goal.target_pose.header.frame_id = "map";
	goal.goal.target_pose.pose.orientation.w = 1;
	goal.goal.target_pose.pose.position.x = X_pos;
	goal.goal.target_pose.pose.position.y = Y_pos;
	pub.publish(goal);
	std::cout << "REACHED" << std::endl;
	}

//logic to the robot go back to the last know random position 
if (choice == 5)
	{
	std::cout << "Your robot will stop at the last know random position" << std::endl;
	follow_wall.request.data = false;
	client2.call(follow_wall);
	goal.goal.target_pose.header.frame_id = "map";
	goal.goal.target_pose.pose.orientation.w = 1;
	goal.goal.target_pose.pose.position.x = X;
	goal.goal.target_pose.pose.position.y = Y;
	pub.publish(goal);
	std::cout << "REACHED" << std::endl;
	}

	ros::spinOnce();

}
return 0;
}

