#!/usr/bin/env python
# -*- coding: utf-8 -*-

#GPS数据获取
import rospy
import math
import serial 
#import pynmea2
import urllib
import hashlib
import requests
import json
import time

from std_msgs.msg import String

def get_GPS():
	ser = serial.Serial('/dev/gps',9600)
	if ser.is_open == 0:
		ser.open()
	while ser.is_open:
		line = str(str(ser.readline())[3:])
		#print line
		#if line.startswith('PRMC'):
		if line.startswith('RMC'):
			#print('接收的数据：'+str(line))
			line = str(line).split(',')
			lat = GPS_rev(float(line[3])) #纬度30	
			lon = GPS_rev(float(line[5])) #经度120
			lon,lat =  wgs84tobd09(lon,lat)
			gps_data = str(lat)+","+str(lon)
			#print(gps_data)
			return gps_data
	
			# elif line.startswith('NRMC'):
			# 	#print('接收的数据：'+str(line))
			# 	line = str(line).split(',')
			# 	lat = GPS_rev(float(line[3])) #纬度30	
			# 	lon = GPS_rev(float(line[5])) #经度120
			# 	lon1,lat1 =  wgs84tobd09(lon,lat)
			# 	gps_data[0] = lat1
			# 	gps_data[1] = lon1

def wgs84tobd09(lon,lat):
		queryStr = '/geoconv/v1/?coords='+str(lon)+','+str(lat)+'&from=1&to=5&ak=F6hU3hYMC7G0nIqi8DinyVeaCveTNUOD'
		#print(queryStr)
		# 对queryStr进行转码，safe内的保留字符不转换
		encodedStr = urllib.quote(queryStr, safe="/:=&?#+!$,;'@()*[]")

		# 在最后直接追加上yoursk
		rawStr = encodedStr + 'fqB3atRGbBU4GXysiBMPRPZXq7fzDgWf'

		# md5计算出的sn值 
		my_sn = hashlib.md5(urllib.quote_plus(rawStr)).hexdigest()
		# print my_sn
		url = 'http://api.map.baidu.com' + queryStr + "&sn=" + my_sn
		# print url

		res = requests.get(url)
		# print '*' * 10
		# get收到的内容
		json_str = res.content
		# print json_str
		dictData = json.loads(json_str)
		# print dictData["result"][0]["x"]
		# print dictData["result"][0]["y"]
		return dictData["result"][0]["x"],dictData["result"][0]["y"]

def GPS_rev(gps):
	shi = math.modf(gps)[1]//100
	fen = (math.modf(gps)[1]%100)/60
	miao = math.modf(gps)[0]/60
	#print 'fen'+str(fen)
	return shi+fen+miao


def send_gps():
	pub = rospy.Publisher('GPS', String, queue_size=10)
	rospy.init_node('talker', anonymous=True)
	rate = rospy.Rate(1) # 10hz
	while not rospy.is_shutdown():
		gps_data = "30.321093,120.349618"
		#data = get_GPS()
		pub.publish(gps_data)
		#pub.publish(str(data))
		#print(gps_data)
		rate.sleep()
 
if __name__ == '__main__':
	try:
		send_gps()
	except rospy.ROSInterruptException:
		pass