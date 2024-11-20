#include "ros/ros.h"
#include "turtlesim/Pose.h"
#include "geometry_msgs/Twist.h"
#include "turtlesim/Spawn.h"

#include <iostream>
using namespace std; 

double px;

void turtlesimCallback(const turtlesim::Pose::ConstPtr& msg){
	ROS_INFO("The position of the turtle is: %f, %f, %f\n", 	msg->x,msg-> y,msg-> theta);
	px=msg->x;
	
}
int main (int argc, char **argv){
	ros::init(argc, argv, "node_1");
	ros::NodeHandle n;
	ros::Subscriber turtle_sub = n.subscribe("/turtle1/pose", 10, turtlesimCallback); //topic,buffer and the name of the callback
	ros::Publisher turtle1_pub = n.advertise <geometry_msgs::Twist>("/turtle1/cmd_vel", 10);	
	ros::Publisher turtle2_pub = n.advertise <geometry_msgs::Twist>("/turtle2/cmd_vel",10);
	
	ros::ServiceClient turtle_client =n.serviceClient<turtlesim::Spawn>("/spawn");
	turtlesim::Spawn my_spawn;
	my_spawn.request.x= 2.0;
	my_spawn.request.y=2.0;
	my_spawn.request.theta=0.0;
	my_spawn.request.name="turtle2";
	
	turtle_client.call(my_spawn);
	
	int choose = 0;
	float new_vel=0.0;
	
	ros::Rate loop_rate(1);
	geometry_msgs::Twist my_vel; 
	
	while (ros::ok()){
		cout<<"Choose a turtle: ";
		cin>>choose;
		cout<<"Choose velocity: ";
		cin>>new_vel;
		my_vel.linear.x=new_vel;
		if (choose == 0){
			turtle1_pub.publish(my_vel);
		}
		if (choose == 1){
			turtle2_pub.publish(my_vel);
		}
		ros::spinOnce();
		loop_rate.sleep();
	}
	return 0;
}




//turtlesim/Pose 	float32 x
//			float32 y
//			float32 theta
//			float32 linear_velocity
//			float32 angular_velocity

//geometry_msgs/Twist
//			geometry_msgs/Vector3 linear
//			  float64 x
//			  float64 y
//			  float64 z
//			geometry_msgs/Vector3 angular
//			  float64 x
//			  float64 y
//			  float64 z
//SERVICES
//turtlesim/Spawn
//			Args: x y theta name
//turtlesim/Kill
//			Args: name


