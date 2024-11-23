#include "ros/ros.h"
#include "turtlesim/Pose.h"
#include "geometry_msgs/Twist.h"
#include "turtlesim/Spawn.h"

#include <iostream>
using namespace std; 


int main (int argc, char **argv){
	ros::init(argc, argv, "node_1");
	ros::NodeHandle n;
	
	ros::Publisher turtle1_pub = n.advertise <geometry_msgs::Twist>("/turtle1/cmd_vel", 10);	
	ros::Publisher turtle2_pub = n.advertise <geometry_msgs::Twist>("/turtle2/cmd_vel",10);
	
	ros::ServiceClient turtle_client =n.serviceClient<turtlesim::Spawn>("/spawn");
	turtlesim::Spawn my_spawn;
	my_spawn.request.x= 2.0;
	my_spawn.request.y=2.0;
	my_spawn.request.theta=0.0;
	my_spawn.request.name="turtle2";
	
	turtle_client.call(my_spawn);
	
	float new_velx, new_vely, new_velz;
	
	ros::Rate loop_rate(1);
	geometry_msgs::Twist my_vel; 
	
	while (ros::ok()){
		int choose = 0;
		cout<<"Choose one turtle between 1 and 2: ";
		cin>>choose;
		if (choose == 1 || choose ==2){
			cout<<"Choose linear velocity along x: ";
			cin>>new_velx;
			cout<<"Choose linear velocity along y: ";
			cin>>new_vely;
			cout<<"Choose angular velocity along z: ";
			cin>>new_velz;
			my_vel.linear.x=new_velx;
			my_vel.linear.y=new_vely;
			my_vel.angular.z=new_velz;
			switch (choose){
				case 1:  
					turtle1_pub.publish(my_vel);
					break;
				case 2:
					turtle2_pub.publish(my_vel);
					break;
				default:
					cout<<"Error...";
			}
		}
		else {
			my_vel.linear.x= my_vel.linear.y= my_vel.angular.z=0;
			turtle1_pub.publish(my_vel);
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


