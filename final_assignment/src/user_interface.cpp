#include "ros/ros.h"
#include "std_msgs/String.h"

#include <stdlib.h>
#include <stdio.h>

#include <iostream>
#include <sstream>

#include "nav_msgs/Odometry.h"
#include "geometry_msgs/Twist.h"

#include <math.h>
#include "final_assignment/service.h"

	//Declare all variables as global
	ros::Publisher pub; 		//Declare the publisher as global variable with name "pub"
	ros::ServiceClient client;	//Declare the client service as global variable with name "client"
	final_assignment::service srv;	//Declare the service file as "srv"


int main(int argc, char **argv)
{	
	ros::init(argc, argv, "call_target");	//third argumment is the name of the node that I'm creating

	//Declare new nodes	
	ros::NodeHandle n;

	//Inicialize the client
	client = n.serviceClient<final_assignment::service>("/setting_target");

	//Call the random position
	client.call(srv);

	//Initialize my publisher in /cmd_vel
	pub = n.advertise<geometry_msgs::Twist>("/cmd_vel", 1000);//To fill the publisher, I need to use the comand "rostopic type /cmd_vel

	//Subscribe and set a position in positionCall back function
	//ros::Subscriber sub = n.subscribe("/odom", 1000, positionCallback); //I'm subscribing in "/odom"	 

	//Set the limits of my random number to be between -6 and 6
	srv.request.min = 0;		//--------> N		
	srv.request.max = 5;		//--------> M		

	ros::spin();

	while (ros::ok())
	{	
	/**/
	}

return 0;
}

