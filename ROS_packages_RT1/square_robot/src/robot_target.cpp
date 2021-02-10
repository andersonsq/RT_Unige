/* The first process will be in charge of:
	Calling a service for receiving a random target;
	Making the robot reach the target. */

#include "ros/ros.h"
#include "std_msgs/String.h"

#include <stdlib.h>
#include <stdio.h>

#include <iostream>
#include <sstream>

#include "nav_msgs/Odometry.h"
#include "geometry_msgs/Twist.h"

#include <math.h>
#include "square_robot/service.h"

	//Declare all variables as global
	ros::Publisher pub; 		//Declare the publisher as global variable with name "pub"
	ros::ServiceClient client;	//Declare the client service as global variable with name "client"
	square_robot::service srv;	//Declare the service file as "srv"

//positonCallback LOOP
void positionCallback(const nav_msgs::Odometry::ConstPtr& msg)	//Callback is executed verytime I receive a new topic
{
	//Receive the position of my robot and print it on the shell
	ROS_INFO("The robot position is: [%f, %f]", msg->pose.pose.position.x, msg->pose.pose.position.y);
	
	//Created a mesage of type geometry_msgs Twist
	geometry_msgs::Twist vel;

	ros::Rate r(2);			//Declare a delay of 2 seconds with name "r
	
	/* Now I need to fill the inforations of te messages, for that, I use the command "rosmsg show geometry_msgs/Twist" */
	
	//Store the random values of my service PosX and PosY in my variables X and Y
	float X	= srv.response.PosX;
	float Y = srv.response.PosY;

//Send the robot to my target position in the X axis positive
if (msg->pose.pose.position.x < X)
	{
	vel.linear.x = 0.2;
	vel.linear.y = 0.0;
	}

//Send the robot to my target position in the Y axis positive
else if(msg->pose.pose.position.y < Y)
	{
	vel.linear.x = 0.0;
	vel.linear.y = 0.2;
	}
	//client.call(srv);

//Send the robot to my target position in the X axis negative
if(msg->pose.pose.position.x > X)
	{
	vel.linear.x = -0.2;
	//vel.linear.y = 0.0;
	}

//Send the robot to my target position in the Y axis negative
else if(msg->pose.pose.position.y > Y)
	{
	vel.linear.x = 0.0;
	vel.linear.y = -0.2;
	}
	
	//Set all mu angula velocities to 0
	vel.angular.x = 0.0;
	vel.angular.y = 0.0;
	vel.angular.z = 0.0;
	
	//Print in the shell the postion of my target in X and Y
	std::cout << "Your target in X is: " << X << std::endl;
	std::cout << "Your target in Y is: " << Y << std::endl;

	//Publish my velocity
	pub.publish(vel);

//Logic to chech if my robot reached the target position
if((X - msg->pose.pose.position.x <= 0.05 && X - msg->pose.pose.position.x > -0.05) && (Y - msg->pose.pose.position.y <= 0.05 && Y - msg->pose.pose.position.y>-0.05))
	{
	std::cout << "Robot on target X and Y" << std::endl;
	vel.linear.x = 0.0;	//stop the robot
	vel.linear.y = 0.0;	//stop the robot
	r.sleep();		//delay of 2s
	client.call(srv);	//call a new random position
	}
}

int main(int argc, char **argv)
{	
	ros::init(argc, argv, "call_robot_target");	//third argumment is the name of the node that I'm creating

	//Declare new nodes	
	ros::NodeHandle n;

	//Inicialize the client
	client = n.serviceClient<square_robot::service>("/setting_target");

	//Call the random position
	client.call(srv);

	//Initialize my publisher in /cmd_vel
	pub = n.advertise<geometry_msgs::Twist>("/cmd_vel", 1000);//To fill the publisher, I need to use the comand "rostopic type /cmd_vel

	//Subscribe and set a position in positionCall back function
	ros::Subscriber sub = n.subscribe("/odom", 1000, positionCallback); //I'm subscribing in "/odom"	 

	//Set the limits of my random number to be between -6 and 6
	srv.request.RandX = -6;		//--------> N		
	srv.request.RandY = 6;		//--------> M		

	ros::spin();

	while (ros::ok())
	{	
	/**/
	}

return 0;
}

