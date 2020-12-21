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
#include "roscpp/GetLoggers.h"		//Vou usar no Service
#include "roscpp/SetLoggerLevel.h"	//Vou usar no Service
#include "std_srvs/Empty.h"		//Vou usar no Service

#include "square_robot/service.h"


ros::Publisher pub; //Declaro o publisher como variavel global de nome "pub"

//void positionCallback(const geometry_msgs::Point position::ConstPtr& msg)

void positionCallback(const nav_msgs::Odometry::ConstPtr& msg)//Callback e executado toda vez q recebe algo de um topico
{
	ROS_INFO("The robot position is: [%f, %f]", msg->pose.pose.position.x, msg->pose.pose.position.y);//recebo a posiçao do meu robo
	geometry_msgs::Twist vel;	//criei uma mensagem do tipo geometry_msgs Twist
	/* Agora eu preciso preencher as informacoes das mensagens, p/ isso eu uso o comando "rosmsg show geometry_msgs/Twist" */
	
	float t, X, Y;
	
	//PosX = (rand() %6)-6;	//randon numbers from -6 to 6
	//PosY = (rand() %6)-6;
	//X = PosX;
	//Y = PosY;

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
	
	vel.angular.x = 0.0;
	vel.angular.y = 0.0;
	vel.angular.z = 0.0;

	pub.publish(vel);

	std::cout << "Your target in X is: " << X << std::endl;
	std::cout << "Your target in Y is: " << Y << std::endl;

if((X - msg->pose.pose.position.x)< 0.06)
	{
	std::cout << "Robot on target X" << std::endl;
	}
else if((Y - msg->pose.pose.position.y)< 0.06)
	{
	std::cout << "Robot on target Y" << std::endl;
	}

}

int main(int argc, char **argv)
{	float PosX, PosY;
	ros::init(argc, argv, "call_robot_target");	//third argumment is the name of the node that I'm creating
	ros::NodeHandle n;

	ros::ServiceClient client = n.serviceClient<square_robot::service>("setting_target");

	pub = n.advertise<geometry_msgs::Twist>("/cmd_vel", 1000);//P/ preencher o publisher, eu preciso usar o comando "rostopic type /cmd_vel
	ros::Subscriber sub = n.subscribe("/odom", 1000, positionCallback); //escrevo a posiçao do robo	 

	square_robot::service srv;

	srv.request.RandX = PosX;	//atoll(argv[1]);
	srv.request.RandY = PosY;	//atoll(argv[2]);

	client.call(srv);

	ros::spin();

	//ros::Rate loop_rate(10);

	while (ros::ok())
	{	
	/*

	*/
	}

return 0;
}

