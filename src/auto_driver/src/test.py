#!/usr/bin/env python
# -*- coding: utf-8 -*-

import rospy
from std_msgs.msg import String
from sensor_msgs.msg import LaserScan
 
def callback(data):
    rospy.loginfo("I heard %s", data.data)#将数据打印在屏幕上，注意data.后面必须是消息中定义的消息名称
 
def listener():
    rospy.init_node('listener', anonymous=True)#声明一个订阅者节点
    rospy.Subscriber("GPS", String, callback)#订阅一个话题，收到话题数据后并做出相应的反应
 
    # spin() simply keeps python from exiting until this node is stopped
    rospy.spin()
 
if __name__ == '__main__':
    listener()
