#RT1 Assignment

Node1:
Given the simulation "turtlesim_node", first of all the code add a turtle in the envirovment in the specified position. After that, inside the loop (rate of 1 Hz), the user can chose which one of the two wants to control. If the value is not 1 or 2, the programm reload the choosing interface. After that the user can chose linear velocity, along x and y, and angular velocity, along z. 

Node2:
In node 2 I added the /Distance.c library in which the user can check the distance between the two turtle running on shell the command rostopic echo /distance_topic. 
This node calculate the distance between turtles. When they are too close to each other or to the edges it returns a warning and stops the turtles. After that the user can input a velocity only if that makes them apart. The same is done with the borders.


Distance.msg:
Create a msg in which there are two variables:
-String name
-float distance
