/* Second process will act as a Server, by replying to the client with a randon target, having X and Y in the interval (-6.0 and 6.0)
A target is considered reached when the distance between the robot and the target is below 0.1 */

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
#include "square_robot/service.h"

	ros::ServiceServer service;	//Declare the service server as global variable with name "service"


void subscriberCallback(const nav_msgs::Odometry::ConstPtr& msg)
{
	if((msg->pose.pose.position.x > 6.0 || msg->pose.pose.position.x < -6.0) || (msg->pose.pose.position.y > 6.0 || msg->pose.pose.position.y < -6.0))
		{
		std::cout << "Robot out of range" << std::endl;
		ros::shutdown();
		}
}
	
	/*Server function*/

	double randMToN(double M, double N)
	{     
	//rand() % 6 + 1;
	//int rand();
	//int M = 6;
	//int N = -6;
	return M + (rand() / (RAND_MAX / (N-M)));	
        }

	bool myrandom (square_robot::service::Request &req, 
	square_robot::service::Response &res)
	{
	res.PosX = randMToN(req.RandX, req.RandY);
    	res.PosY = randMToN(req.RandX, req.RandY);
    	return true;
	}

int main(int argc, char **argv)
{
	ros::init(argc, argv, "reply_target");	//third argumment is the name of the node that I'm creating
	ros::NodeHandle n;	
	
	service = n.advertiseService("/setting_target", myrandom);

	ros::spin();

	while (ros::ok())
	{
	/* Will reply to the client with the random target */	
	}

return 0;

}
