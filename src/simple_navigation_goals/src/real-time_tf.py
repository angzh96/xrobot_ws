#!/usr/bin/env python
#coding=utf-8
import rospy

from sensor_msgs.msg import LaserScan
from geometry_msgs.msg import Twist
from tf_conversions import transformations
from math import pi
from std_msgs.msg import String
import socket
import tf
import math

class Robot:
    def __init__(self):
        self.tf_listener = tf.TransformListener()
        try:
            self.tf_listener.waitForTransform('/map', '/base_link', rospy.Time(), rospy.Duration(1.0))
        except (tf.Exception, tf.ConnectivityException, tf.LookupException):
            return

    def get_pos(self):
        try:
            (trans, rot) = self.tf_listener.lookupTransform('/map', '/base_link', rospy.Time(0))
        except (tf.LookupException, tf.ConnectivityException, tf.ExtrapolationException):
            rospy.loginfo("tf Error")
            return None
        euler = transformations.euler_from_quaternion(rot)
        #print euler[2] / pi * 180
        x = trans[0]
        y = trans[1]
        th = euler[2] / pi * 180
        return (x, y, th)

    def get_scan(self):
        msg = rospy.wait_for_message("/scan", LaserScan, timeout=None)
        self.scan_filter = []
        for i in range(360):
            if i <=195 and i >165:
                if msg.ranges[i] >= 0.05:
                    self.scan_filter.append(msg.ranges[i])
        #rospy.loginfo('distance of the obstacle : %f', min(self.scan_filter))
        return min(self.scan_filter)

def callback(data):
    robot = Robot()

    s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    s.setsockopt(socket.SOL_SOCKET, socket.SO_BROADCAST, 1)
    PORT = 10086
    network = '<broadcast>'
    if(not robot.get_pos() is None):
        x,y,z = robot.get_pos()
        #print robot.get_pos()
        rospy.loginfo('selfPosition:x=%s,y=%s,z=%s',x,y,z)
        k = math.atan2(y,x)
        rospy.loginfo('angle=%f',k)

        dist = robot.get_scan()
        rospy.loginfo('obstacleDist:%f',dist)
        x = x + dist*(math.cos(k))
        y = y + dist*(math.sin(k))
        rospy.loginfo('obstaclePosition:x=%f,y=%f,z=%s',x,y,z)

        s.sendto(str(x)+","+str(y)+","+str(z)+",", (network, PORT))

        
if __name__ == "__main__":
    rospy.init_node('get_pos_demo',anonymous=True)
    rospy.Subscriber("navigation_1H8", String, callback)
    rospy.spin()
    #r = rospy.Rate(100)#rospy.Rate(hz)，可以保持一定的速率来进行循环
    #r.sleep()
    #while not rospy.is_shutdown():#rospy.is_shutdown()用于检测程序是否退出，是>否按Ctrl-C或其他
    #    print robot.get_pos()
    #    r.sleep()
