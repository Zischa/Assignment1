#include "ros/ros.h"
#include "turtlesim/Pose.h"
#include "geometry_msgs/Twist.h"
#include <cmath>
#include "assignment1_rt/Distance.h"
using namespace std;

turtlesim::Pose turtle1_pose;
turtlesim::Pose turtle2_pose;
bool turtle1_pose_received = false;
bool turtle2_pose_received = false;
bool stop_turtle = false;

float previous_distance;
double current_distance;
double threshold = 2.0;

double borderx;
double bordery;
double new_borderx;
double new_bordery;

void tutlepose1Callback (const::turtlesim::Pose::ConstPtr& msg){
	turtle1_pose = *msg;
	turtle1_pose_received = true;
}
void tutlepose2Callback (const::turtlesim::Pose::ConstPtr& msg){
	turtle2_pose = *msg;
	turtle2_pose_received=true;
}

float distance_cal (const::turtlesim::Pose pose1, const::turtlesim::Pose pose2){
	return sqrt(pow(pose1.x-pose2.x, 2)+pow(pose1.y - pose2.y,2));
}


int main (int argc, char**argv){
	ros::init(argc, argv, "node2");
	ros::NodeHandle n;
	ros::Subscriber turtle_sub1 = n.subscribe("/turtle1/pose", 1000, tutlepose1Callback);
	ros::Subscriber turtle_sub2 = n.subscribe("/turtle2/pose", 1000, tutlepose2Callback);
	
	ros::Publisher turtle1_pub = n.advertise <geometry_msgs::Twist>("/turtle1/cmd_vel", 10);	
	ros::Publisher turtle2_pub = n.advertise <geometry_msgs::Twist>("/turtle2/cmd_vel", 10);
	
	ros::Publisher distance_pub = n.advertise <assignment1_rt::Distance>("/distance_topic", 10);
	assignment1_rt::Distance distance_msg;
	
	ros::Rate loop_rate(100);
	geometry_msgs::Twist my_vel;
	my_vel.linear.x = 0.0;
	my_vel.linear.y = 0.0;
	my_vel.angular.z = 0.0;
	
	while (ros::ok()){
		if (turtle1_pose_received && turtle2_pose_received){
			previous_distance = current_distance;
			current_distance = distance_cal(turtle1_pose, turtle2_pose);
			distance_msg.dist = distance_cal(turtle1_pose, turtle2_pose);
			distance_msg.name = "Current distance between turtles";
			distance_pub.publish (distance_msg);
			if (current_distance < threshold && current_distance - previous_distance < 0.0){
				turtle1_pub.publish (my_vel);
				turtle2_pub.publish(my_vel);
				ROS_INFO ("Turtles are too close, move the part!");
			}
			if (turtle1_pose.x> 10.0 || turtle1_pose.x<1 || turtle1_pose.y>10.0 || turtle1_pose.y<1.0){
			borderx=new_borderx;
			bordery=new_bordery;
			new_borderx = turtle1_pose.x;
			new_bordery = turtle1_pose.y;
				if (turtle1_pose.x>10.0 || turtle1_pose.y>10.0){
					if ((new_borderx-borderx>0)||(new_bordery-bordery>0)){
						turtle1_pub.publish(my_vel);
					ROS_INFO ("Turtle1 is too close to the border!! Please move...");
					}
				}
				if (turtle1_pose.x<1.0 || turtle1_pose.y<1.0){
					if ((new_borderx-borderx<0)||(new_bordery-bordery<0)){
						turtle1_pub.publish(my_vel);
					ROS_INFO ("Turtle1 is too close to the border!! Please move...");
					}
				}
			}
			if (turtle2_pose.x> 10.0 || turtle2_pose.x<1 || turtle2_pose.y>10.0 || turtle2_pose.y<1.0){
			borderx=new_borderx;
			bordery=new_bordery;
			new_borderx = turtle2_pose.x;
			new_bordery = turtle2_pose.y;
				if (turtle2_pose.x>10.0 || turtle2_pose.y>10.0){
					if ((new_borderx-borderx>0)||(new_bordery-bordery>0)){
						turtle2_pub.publish(my_vel);
					ROS_INFO ("Turtle1 is too close to the border!! Please move...");
					}
				}
				if (turtle2_pose.x<1.0 || turtle2_pose.y<1.0){
					if ((new_borderx-borderx<0)||(new_bordery-bordery<0)){
						turtle2_pub.publish(my_vel);
					ROS_INFO ("Turtle1 is too close to the border!! Please move...");
					}
				}
			}
		}
		ros::spinOnce();
		loop_rate.sleep();
	}
	
	return 0;
}
