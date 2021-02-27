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
import tf2_ros
#from tf2_geometry_msgs import PointStamped
from geometry_msgs.msg import PointStamped



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
            rospy.loginfo("tf1 Error")
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
    rospy.loginfo("ip = %s ", data.data)
    ipList = []
    if(len(data.data) > 3):
        rospy.loginfo("ipList === %s ", data.data)
        ipList.append(data.data)
    
    s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    s.setsockopt(socket.SOL_SOCKET, socket.SO_BROADCAST, 1)
    PORT = 10086
    network = '<broadcast>'
    num = 0
    if(not robot.get_pos() is None):
        while(num <= 5):
            if(num == 0):
               x,y,z = robot.get_pos()
               #print robot.get_pos()
               rospy.loginfo('selfPosition:x=%s,y=%s,z=%s',x,y,z)
               k = math.atan2(y,x)
               rospy.loginfo('angle=%f',k)
               dist = robot.get_scan()
               rospy.loginfo('obstacleDist:%f',dist)
               x = x + dist*(math.cos(k))
               y = y + dist*(math.sin(k))
               rospy.loginfo('obstacle:x=%f,y=%f,z=%s',x,y,z)
               s.sendto(str(x)+","+str(y)+","+str(z)+",0", (network, PORT))
               num +=1
            else:
                rospy.loginfo('========  transform line ========')
                #ipList = list(set(ipList))
                x,y,z = robot.get_pos()
                #print robot.get_pos()
                rospy.loginfo('selfPosition:x=%s,y=%s,z=%s',x,y,z)
                dist = robot.get_scan()

                #buffer = tf2_ros.Buffer()
                #listener = tf2_ros.TransformListener(buffer)
                n_len = len(ipList)
                rospy.loginfo("length of ipList: %s",n_len)

                for ip_num in range(len(ipList)):
                    dists = dist*2*math.sin(math.pi*(ip_num+1)/(len(ipList)+1))
                    y = dists*math.sin(math.pi*(ip_num+1)/(len(ipList)+1)-math.pi/2)
                    x = dists*math.cos(math.pi*(ip_num+1)/(len(ipList)+1)-math.pi/2)
                    rospy.loginfo("== calculate: x= %.2f y=%.2f  ",x,y)
                    try:
                       listener = tf.TransformListener()
                       listener.waitForTransform("/base_link", "/map", rospy.Time(0), rospy.Duration(4.0))
                       base_point = PointStamped()
                       base_point.header.frame_id = "base_link"
                       base_point.header.stamp = rospy.Time(0)
                       #base_point.header.stamp = rospy.Time.now()
                       base_point.point.x = x
                       base_point.point.y = y
                       base_point.point.z = z
                       p = listener.transformPoint("map", base_point)
                       #p = buffer.transform(base_point,"map",rospy.Duration(1.0))
                       rospy.loginfo("point transform:x= %.2f, y= %.2f, z= %.2f",p.point.x,p.point.y,p.point.z)
                       network = str(ipList[ip_num])
                       s.sendto(str(p.point.x)+","+str(p.point.y)+","+str(p.point.z)+",1", (network, PORT))
                    except (tf.LookupException, tf.ConnectivityException, tf.ExtrapolationException) as e:
                        rospy.loginfo("tf2 Error: %s",e)
                num +=1


        
    

if __name__ == "__main__":
    rospy.init_node('get_pos_demo',anonymous=True)
    rospy.Subscriber("ipMsg", String, callback)
    #rospy.Subscriber("navigation_1H8", String, callback)
    rospy.spin()
    #r = rospy.Rate(100)#rospy.Rate(hz)，可以保持一定的速率来进行循环
    #r.sleep()
    #while not rospy.is_shutdown():#rospy.is_shutdown()用于检测程序是否退出，是否按Ctrl-C或其他
    #    print robot.get_pos()
    #    r.sleep()