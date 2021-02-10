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
	square_robot::service srv;	//Call my service.srv fille from my square_robot::package and give the name of "srv"

//SubscriberCallback LOOP
void subscriberCallback(const nav_msgs::Odometry::ConstPtr& msg)
{
	//Logic to check that I never receive a position outside my range
	if((msg->pose.pose.position.x > 6.0 || msg->pose.pose.position.x < -6.0) || (msg->pose.pose.position.y > 6.0 || msg->pose.pose.position.y < -6.0))
		{
		std::cout << "Robot out of range" << std::endl;
		ros::shutdown();
		}
}
	
	//Server function to generate my random number from -6(N) to 6(M)
	double randMToN(double M, double N)
	{     
	return M + (rand() / (RAND_MAX / (N-M)));	
        }

	//Function to take my random numbers from randMToN and then attribute then
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
