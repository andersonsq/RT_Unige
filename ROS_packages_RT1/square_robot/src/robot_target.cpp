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
	//ros::ServiceServer service;

void positionCallback(const nav_msgs::Odometry::ConstPtr& msg)//Callback is executed verytime I receive a new topic
{
	//Receive the position of my robot
	ROS_INFO("The robot position is: [%f, %f]", msg->pose.pose.position.x, msg->pose.pose.position.y);
	
	//Created a mesage of type geometry_msgs Twist
	geometry_msgs::Twist vel;
	
	/* Agora eu preciso preencher as informacoes das mensagens, p/ isso eu uso o comando "rosmsg show geometry_msgs/Twist" */
	
	float X	= srv.response.PosX-msg->pose.pose.position.x;
	float Y = srv.response.PosY-msg->pose.pose.position.y;
	
if (msg->pose.pose.position.x < X)
	{
	vel.linear.x = 0.2;
	vel.linear.y = 0.0;
	}

else if(msg->pose.pose.position.y < Y)
	{
	vel.linear.x = 0.0;
	vel.linear.y = 0.2;
	}
	client.call(srv);

if(msg->pose.pose.position.x > X)
	{
	vel.linear.x = -0.2;
	//vel.linear.y = 0.0;
	}
else if(msg->pose.pose.position.y > Y)
	{
	vel.linear.x = 0.0;
	vel.linear.y = -0.2;
	}
	client.call(srv);
	
	vel.angular.x = 0.0;
	vel.angular.y = 0.0;
	vel.angular.z = 0.0;

	pub.publish(vel);

	std::cout << "Your target in X is: " << X << std::endl;
	std::cout << "Your target in Y is: " << Y << std::endl;

if((X - msg->pose.pose.position.x)< 0.06)
	{
	std::cout << "Robot on target X" << std::endl;
	vel.linear.x = 0.0;
	}
if((Y - msg->pose.pose.position.y)< 0.06)
	{
	std::cout << "Robot on target Y" << std::endl;
	vel.linear.y = 0.0;
	}
	
	//Call a new random position 
	client.call(srv);
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
	ros::Subscriber sub = n.subscribe("/odom", 1000, positionCallback); //escrevo a posiçao do robo	 

	square_robot::service srv;

	srv.request.RandX = -=6;		//<--------My error is here, don't know how to solve it	
	srv.request.RandY = 6;		//<--------My error is here, don't know how to solve it


/*
	srv.request.RandX = PosX;	//<--------My error is here, don't know how to solve it	
	srv.request.RandY = PosY;	//<--------My error is here, don't know how to solve it
*/

	ros::spin();

	//ros::Rate loop_rate(10);

	while (ros::ok())
	{	
	/**/
	}

return 0;
}

