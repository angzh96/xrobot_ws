#!/usr/bin/env python
# -*- coding: UTF-8 -*-
import socket
import time
import rospy
import threading
from std_msgs.msg import String


rospy.init_node('listener', anonymous=True)
destination = None
gps_data =None
send_mulgps = rospy.Publisher('recv_gps', String, queue_size=10)
send_joy = rospy.Publisher('recv_joy', String, queue_size=10)


def callback(data):
    global gps_data
    gps_data = data.data
    print(data.data)
    print("gps_data:"+gps_data)

def listener():
 
    rospy.Subscriber("GPS", String, callback)
    rospy.spin()

if __name__ == "__main__":
    global gps_data 

    t = threading.Thread(target=listener)
    t.start()

    fd = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    fd.connect(("175.24.59.243", 7002))
    #fd.connect(("127.0.0.1", 7002))
    fd.send(bytes("1"+"\n").encode("utf-8"))
    
    while True:
        destination = fd.recv(1024)
        print(destination)
        if len(destination)>1:
            print(str(gps_data)+","+str(destination))
            send_mulgps.publish(str(gps_data)+","+str(destination))
            destination =None
     
        elif len(destination)==1:
            send_joy.publish(str(destination))
            destination =None
          
        
    
    fd.close()
