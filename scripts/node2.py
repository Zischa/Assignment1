#!/usr/bin/env python

import rospy
from turtlesim.msg import Pose
from geometry_msgs.msg import Twist
from assignment1_rt.msg import Distance
import math


turtle1_pose = Pose()
turtle2_pose = Pose()
turtle1_pose_received = False
turtle2_pose_received = False

previous_distance = 0.0
current_distance = 0.0
threshold = 2.0

borderx = 0.0
bordery = 0.0
new_borderx = 0.0
new_bordery = 0.0

def turtlepose1Callback (msg):
	global turtle1_pose, turtle1_pose_received
	turtle1_pose=msg
	turtle1_pose_received = True
def turtlepose2Callback (msg):
	global turtle2_pose, turtle2_pose_received
	turtle2_pose=msg
	turtle2_pose_received = True
	
def distance_cal (pose1, pose2):
	return math.sqrt((pose1.x -pose2.x)**2+(pose1.y-pose2.y)**2)

def main():
	rospy.init_node ("node2", anonymous=True)
	rospy.Subscriber("turtle1/pose",Pose,turtlepose1Callback)
	rospy.Subscriber("turtle2/pose",Pose,turtlepose2Callback)
	
	turtle1_pub=rospy.Publisher("/turtle1/cmd_vel",Twist,queue_size=10)
	turtle2_pub=rospy.Publisher("/turtle2/cmd_vel",Twist,queue_size=10)
	
	distance_pub=rospy.Publisher("distance_topic", Distance, queue_size=10)
	distance_msg=Distance()
	my_vel=Twist()
	my_vel.linear.x=0.0
	my_vel.linear.y=0.0
	my_vel.angular.z=0.0
	rate=rospy.Rate(100)
	while not rospy.is_shutdown():
		if turtle1_pose_received and turtle2_pose_received:
			global previous_distance, current_distance, borderx, fordery, new_borderx, new_bordery
			previous_distance=current_distance
			current_distance = distance_cal(turtle1_pose,turtle2_pose)
			distance_msg.name = "Current distance between turtles: "
			distance_msg.dist = current_distance
			distance_pub.publish(distance_msg)
		
			if current_distance < threshold and current_distance-previous_distance<0:
				turtle1_pub.publish(my_vel)
				turtle2_pub.publish(my_vel)
				rospy.loginfo("Turtle are too close, move them apart!")
			if turtle1_pose.x > 10.0 or turtle1_pose.y > 10.0:
				borderx=new_borderx
				bordery=new_bordery
				new_borderx=turtle1_pose.x
				new_bordery=turtle1_pose.y
				if (new_borderx-borderx>0) or (new_bordery-bordery>0):
					turtle1_pub.publish(my_vel)
					rospy.loginfo ("Turtle 1 is too close to the border!")
			if turtle2_pose.x > 10.0 or turtle2_pose.y > 10.0:
				borderx=new_borderx
				bordery=new_bordery
				new_borderx=turtle2_pose.x
				new_bordery=turtle2_pose.y
				if (new_borderx-borderx>0) or (new_bordery-bordery>0):
					turtle2_pub.publish(my_vel)
					rospy.loginfo ("Turtle 2 is too close to the border!")
			if turtle1_pose.x < 1.0 or turtle1_pose.y < 1.0:
				borderx=new_borderx
				bordery=new_bordery
				new_borderx=turtle1_pose.x
				new_bordery=turtle1_pose.y
				if (new_borderx-borderx<0) or (new_bordery-bordery<0):
					turtle1_pub.publish(my_vel)
					rospy.loginfo ("Turtle 1 is too close to the border!")
			if turtle2_pose.x < 1.0 or turtle2_pose.y < 1.0:
				borderx=new_borderx
				bordery=new_bordery
				new_borderx=turtle2_pose.x
				new_bordery=turtle2_pose.y
				if (new_borderx-borderx<0) or (new_bordery-bordery<0):
					turtle2_pub.publish(my_vel)
					rospy.loginfo ("Turtle 2 is too close to the border!")
		rate.sleep()

if __name__ == "__main__":
	try:
		main()
	except rospy.ROSInterruptException:
		pass
