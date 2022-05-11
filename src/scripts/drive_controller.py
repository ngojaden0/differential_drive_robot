#! /usr/bin/env python
import rospy
from math import sin
from math import cos
from geometry_msgs.msg import Vector3Stamped
from nav_msgs.msg import Odometry
import tf
from sensor_msgs.msg import JointState
#callback for actual speed
def callback(speed_msg): 
    rospy.loginfo("right: "+str(speed_msg.vector.x)+"  left: "+str(speed_msg.vector.y)+"--")

#callback for joint states
def feedback(simulation_speed):
    rospy.loginfo("simulation: "+str(simulation_speed.position))

#callback for pose/twist
def getback(odom):
    rospy.loginfo("\nodom:\n  position:\n    x:"+str(odom.pose.pose.position.x)+
    "\n"+"    y:"+str(odom.pose.pose.position.y)+"\n")
    
def controller():
    rospy.init_node('drive_controller')
    rate = rospy.Rate(10)
    length = 0.1884 #distance between midpoint of each wheel
    x = 0
    y = 0
    theta = 0
    vel_R = 0 
    vel_L = 0
    if vel_R == vel_L:
        rotation = 0
        R = float('inf')
    else:
        R = (length/2)*(vel_R+vel_L)/(vel_R-vel_L) #instantaneous center of curvature to mid of robot
        rotation = (vel_R-vel_L)/length #robot angular velocity around ICC
    ICC = [x-R*sin(theta), y-R*cos(theta)]


    rospy.Subscriber('speed', Vector3Stamped, callback)
    #rospy.Subscriber('joint_states', JointState, feedback)
    rospy.Subscriber('odom', Odometry, getback)
    rospy.spin()
    rate.sleep()

if __name__ == '__main__':
    try:
        controller()
    except rospy.ROSInterruptException:
        pass
