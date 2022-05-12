#! /usr/bin/env python
from requests import NullHandler
import rospy
from math import sin
from math import cos
from geometry_msgs.msg import Vector3Stamped
from nav_msgs.msg import Odometry
import tf
from sensor_msgs.msg import JointState
import time
length = 0.1884 #distance between midpoint of each wheel
radius = 0.0381
prev_rad_R = 0.0
prev_rad_L = 0.0
currT = rospy.Time()

#callback for actual speed (real robo)
def callback(speed_msg): 
    rospy.loginfo("right: "+str(speed_msg.vector.x)+"  left: "+str(speed_msg.vector.y)+"--")

#callback for joint states (gazebo)
def feedback(simulation_speed):
    rospy.loginfo("\nsimulation:\n"+"  x:"+str(simulation_speed.position[0])+"\n  y:"+str(simulation_speed.position[1]))
    rad_R = simulation_speed.position[0]
    rad_L = simulation_speed.position[1]
    global prev_rad_R
    global prev_rad_L
    global currT
    vel_R  = ((rad_R - prev_rad_R)/0.1)*radius
    vel_L  = ((rad_L - prev_rad_L)/0.1)*radius
    if vel_R == vel_L:
        rotation = 0
        R = float('inf')
    else:
        R = (length/2)*(vel_R+vel_L)/(vel_R-vel_L) #instantaneous center of curvature to mid of robot
        rotation = (vel_R-vel_L)/length #robot angular velocity around ICC
        #ICC = [x-R*sin(theta), y-R*cos(theta)]  
    print(" "+str(vel_R)+" "+str(vel_L)+"   "+str(currT.to_sec()))    
    prev_rad_R = rad_R
    prev_rad_L = rad_L


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