#! /usr/bin/env python
from cmath import pi
from requests import NullHandler
import rospy
from math import sin
from math import cos
from geometry_msgs.msg import Vector3Stamped
from nav_msgs.msg import Odometry
import tf
from sensor_msgs.msg import JointState
import time
import numpy as np
length = 0.1883 #distance between midpoint of each wheel
radius = 0.0381
prev_rad_R = 0.0
prev_rad_L = 0.0
th = 0.0
x = 0.0
y = 0.0
#callback for actual speed (real robot)
def callback(speed_msg): 
    rospy.loginfo("right: "+str(speed_msg.vector.x)+"  left: "+str(speed_msg.vector.y)+"--")

#callback for joint states (gazebo)
def feedback(simulation_speed):
    rad_R = simulation_speed.position[0]
    rad_L = simulation_speed.position[1]
    global prev_rad_R, prev_rad_L, th, x, y
    vel_R  = ((rad_R - prev_rad_R)/0.1)*radius
    vel_L  = ((rad_L - prev_rad_L)/0.1)*radius
    vel_avg = (vel_R + vel_L)/2
    angular_vel = (vel_R - vel_L)/length
    dth = ((vel_R - vel_L)*0.1)/length
    dxy = vel_avg*0.1
    dx = dxy*cos(dth)
    dy = dxy*sin(dth)

    x += (cos(th)*dx-sin(th)*dy)
    y += (sin(th)*dx+cos(th)*dy)
    th += dth

    prev_rad_R = rad_R
    prev_rad_L = rad_L
    if th >= 2*pi: 
        th -= 2*pi
    if th <= -2*pi: 
        th += 2*pi

    rospy.loginfo("\nvelocity:\n"+"  right_wheel (m/s):"+
        str(vel_R)+"\n  left_wheel (m/s):"+
        str(vel_L)+"\n  total_direction (m/s):"+
        str(vel_avg)+"\n  angular (rad/s):"+
        str(angular_vel)+"\npose:\n  x (m):"+
        str(x)+"\n  y (m):"+str(y)+"\n  theta (rad):"+str(th)+"\n")

#callback for pose/twist (gazebo)
def getback(odom):
    rospy.loginfo("\nodom:\n  position:\n    x:"+str(odom.pose.pose.position.x)+
    "\n"+"    y:"+str(odom.pose.pose.position.y)+"\n")
    
if __name__ == '__main__':
    try:            
        rospy.init_node('drive_controller')  
        #rospy.Subscriber('speed', Vector3Stamped, callback)
        rospy.Subscriber('joint_states', JointState, feedback)
        #rospy.Subscriber('odom', Odometry, getback)
        rate = rospy.Rate(10)
        while not rospy.is_shutdown():
            rospy.spin()
            rate.sleep()
        print("done")
    except rospy.ROSInterruptException:
        pass