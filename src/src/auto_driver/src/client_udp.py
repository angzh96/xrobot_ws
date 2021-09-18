#!/usr/bin/env python
# -*- coding: utf-8 -*-
import socket
import os
import datetime
import time
import json
import random
import rospy
from std_msgs.msg import String

ip = '175.24.59.243'
port = 10002

s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

def callback(data):
    gps_data = data.data.split(',')
    currentTime = time.strftime("%Y-%m-%d %H:%M:%S", time.localtime(time.time()))
    longitude = float(gps_data[1])
    latitude = float(gps_data[0])
    temperature = round(random.uniform(20, 40), 2)
    wind = round(random.uniform(0, 15), 2)
    humidity = round(random.uniform(0, 20), 2)
    sendData = {
            "time": currentTime,
            'code': 2,
            'longitude': longitude,
            'latitude': latitude,
            'temperature': temperature,
            'wind': wind,
            'humidity': humidity
    }
    print(sendData)
    location = str(sendData)
    s.sendto(location, (ip, port))


def listener():
 
    rospy.init_node('listener', anonymous=True)
 
    rospy.Subscriber("GPS", String, callback)
 
    
    rospy.spin()

if __name__ == "__main__":
  
    listener()
    
    s.close()