#!/usr/bin/env python
# -*- coding: utf-8 -*-

#发布方向数据
import rospy
import time
import math
import serial
import py_qmc5883l
import urllib
import hashlib
import json
import GPS

from std_msgs.msg import String

def GY85():
	sensor = py_qmc5883l.QMC5883L()
	sensor.calibration = [[ 1.030,0.026,- 227.799 ],[ 0.0255,1.021,1016.442 ],[ 0.0,0.0,1.0 ]]
	sensor.get_bearing()
	return sensor.get_bearing_raw()


def send_gy85():
	pub = rospy.Publisher('GY85', String, queue_size=10)
	rospy.init_node('talker', anonymous=True)
	rate = rospy.Rate(10) # 10hz
	while not rospy.is_shutdown():
		#gy85_data = 276
		gy85_data = GY85()
		pub.publish(str(gy85_data))
		#print(gy85_data)
		rate.sleep()
 
if __name__ == '__main__':
	try:
		send_gy85()
	except rospy.ROSInterruptException:
		pass
