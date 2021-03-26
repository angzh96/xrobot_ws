#!/usr/bin/env python

import rospy
import random
import socket
from nlink_parser.msg import LinktrackTagframe0

LSB_M_TO_LAT_LONG = 0.000008993216059
LAT_A = 30.317806
LONG_A = 120.350125


ip = '172.20.10.6'
port = 8001

s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

def tagframe0Callback(msg):
    #rospy.loginfo(msg)
    pos_data = msg.pos_3d
    posX = pos_data[0]
    posY = pos_data[1]

    latitude = LAT_A + LSB_M_TO_LAT_LONG * posY
    longitude = LONG_A + LSB_M_TO_LAT_LONG * posX

    currentTime = time.strftime("%Y-%m-%d %H:%M:%S", time.localtime(time.time()))
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

def main():
    rospy.init_node('linktrack_example',anonymous=True)
    rospy.Subscriber("nlink_linktrack_tagframe0",LinktrackTagframe0,tagframe0Callback)
    rospy.spin()

if __name__ == '__main__':
    main()




