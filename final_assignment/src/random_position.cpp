#include "ros/ros.h"
#include <stdlib.h>
#include <cstdlib>
#include <stdio.h>

#include "std_msgs/String.h"
#include <iostream>
#include <sstream>

#include "nav_msgs/Odometry.h"
#include "geometry_msgs/Twist.h"

#include <math.h>
#include "final_assignment/service.h"

	ros::ServiceServer service;	//Declare the service server as global variable with name "service"
	final_assignment::service srv;	//Call my service.srv fille from my square_robot::package and give the name of "srv"	//Server function to generate my random number from 0(N) to 5(M)

	double randMToN(double M, double N)
	{     
	return M + (rand() / (RAND_MAX / (N-M)));	
        }

	//Function to take my random numbers from randMToN and then atribute then
	bool myrandom (final_assignment::service::Request &req, 
	final_assignment::service::Response &res)
	{
	int target_position = randMToN(0,5);
	
	//res.PosX = randMToN(req.min, req.max);
	//res.PosY = randMToN(req.min, req.max);

	//possible_position[0] = {-4,-3};
	if(target_position == 0)
	{
	res.PosX = -4;
	res.PosY = -3;
	}

	//possible_position[0] = {-4,-2};
	else if(target_position == 1)
	{
	res.PosX = -4;
	res.PosY = -2;
	}

	//possible_position[0] = {-4, 7};
	else if(target_position == 2)
	{
	res.PosX = -4;
	res.PosY = 7;
	}

	//possible_position[0] = {5,-7};
	else if(target_position == 3)
	{
	res.PosX = 5;
	res.PosY = -7;
	}

	//possible_position[0] = {5,-3};
	else if(target_position == 4)
	{
	res.PosX = 5;
	res.PosY = -3;
	}

	//possible_position[0] = {5, -1};
	else if(target_position == 5)
	{
	res.PosX = 5;
	res.PosY = -1;
	}
	return true;
	}

int main(int argc, char **argv)
{
	ros::init(argc, argv, "target_reply");	//third argumment is the name of the node that I'm creating

	//Declare new nodes
	ros::NodeHandle n;	
	
	service = n.advertiseService("/setting_target", myrandom);

	ros::spin();

	while (ros::ok())
	{
	/**/	
	}

return 0;

}
