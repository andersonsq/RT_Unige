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

	int target_position;
	int possible_position[6];

	possible_position[0] = {-4,-3};
	possible_position[1] = {-4,2};
	possible_position[2] = {-4,7};
	possible_position[3] = {5,-7};
	possible_position[4] = {5,-3};
	possible_position[5] = {5,-1};

	target_position = randMToN(0,5);

	res.Pos = possible_position[target_position];
	std::cout << "Random robot cordinates targets are: [%f]"<< possible_position << std::endl;

    	//res.PosY = randMToN(req.RandX, req.RandY);
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
