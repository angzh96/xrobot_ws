#!/usr/bin/env python
#coding:utf-8
import socket
import rospy
from std_msgs.msg import String

def callback(data):
    #s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    #port = 10086
    #network = "<broadcast>"
    #s.sendto("car1_send", (network, port))
    #pub = rospy.Publisher("ipMsg", string, )
    rospy.loginfo("ip = %s", data.data)
    ipList = []
    ipList.append(data.data)
    for i in range(len(ipList)):
        rospy.loginfo("ipList = %s  i===%s", ipList[i], i)
#def callback1(data):
 #   rospy.loginfo("lll ====== %s", data.data)

if __name__ == '__main__':
    rospy.init_node('iplistener', anonymous=True)
    rospy.loginfo("====hhhh====")
    rospy.Subscriber("ipMsg", String, callback)
   # rospy.Subscriber("test", String, callback1)
    rospy.spin()
