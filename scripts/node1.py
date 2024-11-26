#!/usr/bin/env python

import rospy
from turtlesim.srv import Spawn
from geometry_msgs.msg import Twist

def main ():
	rospy.init_node ("node_1")
	turtle1_pub=rospy.Publisher("/turtle1/cmd_vel", Twist, queue_size=10)
	turtle2_pub=rospy.Publisher("/turtle2/cmd_vel", Twist, queue_size=10)
	rospy.wait_for_service("/spawn")
	try:
		turtle_client=rospy.ServiceProxy("/spawn", Spawn)
		my_spawn=Spawn()
		my_spawn.x=2.0
		my_spawn.y=2.0
		my_spawn.theta=0.0
		my_spawn.name="turtle2"
		turtle_client(my_spawn.x, my_spawn.y, my_spawn.theta, my_spawn.name)
		rospy.loginfo("Spawned turtle2!")
	except rospy.ServiceException as e:
		rospy.logerr("Service call failed: %s" %e)
	rate=rospy.Rate(1)
	my_vel=Twist()
	
	while not rospy.is_shutdown():
		choose=int(input("Choose one turtle between 1 and 2: "))
		if choose == 1 or choose == 2:
			new_velx=float(input("Choose linear velocity along x:"))
			new_vely=float(input("Choose linear velocity along y:"))
			new_velz=float(input("Choose angular velocity along z:"))
			my_vel.linear.x=new_velx
			my_vel.linear.y=new_vely
			my_vel.angular.z=new_velz
			if choose==1:
				turtle1_pub.publish(my_vel)
			elif choose==2:
				turtle2_pub.publish(my_vel)
		else:
			my_vel.linear.x=0
			my_vel.linear.y=0
			my_vel.angular.z=0
			turtle1_pub.publish(my_vel)
			turtle2_pub.publish(my_vel)
		rate.sleep()
if __name__=="__main__":
	try:
		main()
	except rospy.ROSInterruptException:
		pass
