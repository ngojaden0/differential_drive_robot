#! /usr/bin/env python
import rospy
from geometry_msgs.msg import Vector3Stamped
from nav_msgs.msg import Odometry
from tf import TransformBroadcaster

def callback(speed_msg):
    rospy.loginfo("encoder ticks: "+str(speed_msg.vector.x)+"    "+str(speed_msg.vector.y))

def controller():
    rospy.init_node('drive_controller')
    rate = rospy.Rate(10)
    rospy.Subscriber('speed', Vector3Stamped, callback)
    rospy.spin()
    rate.sleep()

if __name__ == '__main__':
    try:
        controller()
    except rospy.ROSInterruptException:
        pass
