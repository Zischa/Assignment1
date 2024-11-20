#include "ros/ros.h"
#include "turtlesim/Pose.h"
#include "geometry_msgs/Twist.h"
#include <cmath>
using namespace std;

turtlesim::Pose turtle1_pose;
turtlesim::Pose turtle2_pose;

void tutlepose1Callback (const::turtlesim::Pose::ConstPtr& msg){
	turtle1_pose = *msg;
}
void tutlepose2Callback (const::turtlesim::Pose::ConstPtr& msg){
	turtle2_pose = *msg;
}

double distance_cal (const::turtlesim::Pose pose1, const::turtlesim::Pose pose2){
	return std::sqrt(std::pow(pose1.x-pose2.x, 2)+std::pow(pose1.y - pose2.y,2));
}


int main (int argc, char**argv){
	ros::init(argc, argv, "node2");
	ros::NodeHandle n;
	ros::Subscriber turtle_sub1 = n.subscribe("/turtle1/pose", 1000, tutlepose1Callback);
	ros::Subscriber turtle_sub2 = n.subscribe("/turtle2/pose", 1000, tutlepose2Callback);
	ros::Publisher turtle1_pub = n.advertise <geometry_msgs::Twist>("/turtle1/cmd_vel", 10);	
	ros::Publisher turtle2_pub = n.advertise <geometry_msgs::Twist>("/turtle2/cmd_vel",10);
	
	ros::Rate loop_rate(1);
	geometry_msgs::Twist my_vel;
	
	while (ros::ok()){
		double distance = distance_cal (turtle1_pose, turtle2_pose);
		if (distance < 2){
			my_vel.linear.x = 0;
			std::cout <<"Turtles are too close!!"<<std::endl;
			turtle1_pub.publish(my_vel);
			turtle2_pub.publish(my_vel);
		}
		else{
			ROS_INFO ("Distance between turtle1 and turtle2: %f", distance);
			
		}
		ros::spinOnce();
		loop_rate.sleep();
	}
	
	return 0;
}
