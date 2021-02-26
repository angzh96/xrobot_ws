#!/usr/bin/env python

import rospy
from sensor_msgs.msg import LaserScan

def listener():
    #rospy.init_node('get_laser')
    #rospy.sleep(2)
    msg = rospy.wait_for_message("/scan", LaserScan, timeout=None)
    
    scan_filter = []
    for i in range(360):
        if i <=195 and i >165:
            if msg.ranges[i] >= 0.05:
                scan_filter.append(msg.ranges[i])
    
    rospy.loginfo('distance of the obstacle : %f', min(scan_filter))
    
   
if __name__ == '__main__':
    rospy.init_node('get_laser')
    rospy.sleep(2)
    listener()