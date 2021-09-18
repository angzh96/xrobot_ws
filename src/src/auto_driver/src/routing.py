#!/usr/bin/env python
#coding=utf-8
import rospy
import time
import math
import serial
import urllib
import hashlib
import json
import requests
import numpy
from std_msgs.msg import String



class route:
	
	def __init__(self):

		self.sub = rospy.Subscriber("recv_gps", String, self.get_urt)
		self.pub = rospy.Publisher('send_route', String, queue_size=10)

	def get_urt(self, data):
		
		gps_data = data.data.split(',')
		#print(gps_data)
	#def get_urt(self,gps_first,gps_destination):
		# 以get请求为例http://api.map.baidu.com/geocoder/v2/?address=百度大厦&output=json&ak=你的ak
		#queryStr = '/geocoding/v3/?address=%s&output=json&ak=F6hU3hYMC7G0nIqi8DinyVeaCveTNUOD' % address
		queryStr = '/direction/v2/riding?origin='+gps_data[0]+','+gps_data[1]+'&destination='+gps_data[3]+','+gps_data[2]+'&ak=F6hU3hYMC7G0nIqi8DinyVeaCveTNUOD'
		#queryStr = '/direction/v2/riding?origin='+gps_data[0]+','+gps_data[1]+'&destination=30.317174,120.349711&ak=F6hU3hYMC7G0nIqi8DinyVeaCveTNUOD'
					
		# 对queryStr进行转码，safe内的保留字符不转换
		encodedStr = urllib.quote(queryStr, safe="/:=&?#+!$,;'@()*[]")
		# 在最后直接追加上yoursk
		rawStr = encodedStr + 'fqB3atRGbBU4GXysiBMPRPZXq7fzDgWf'
		# 计算sn
		sn = (hashlib.md5(urllib.quote_plus(rawStr).encode("utf8")).hexdigest())
		# 由于URL里面含有中文，所以需要用parse.quote进行处理，然后返回最终可调用的url
		url = urllib.quote("http://api.map.baidu.com" + queryStr + "&sn=" + sn, safe="/:=&?#+!$,;'@()*[]")
		baidu_data = urllib.urlopen(url);
		hjson = json.loads(baidu_data.read());
		if hjson['status'] == 0:
			result = hjson['result'];
			routes = result['routes'];
			steps = routes[0]['steps']
			path1 = steps[0]['path'];
			real_path = []
			for i in range(len(steps)):
				path1 = steps[i]['path']
				path2 = path1.split(';')
				real_path.append(str(path2[len(path2)-1].split(',')[0])+','+str(path2[len(path2)-1].split(',')[1]))
				#real_path.append(path2[len(path2)-1].split(','))
			if real_path != None:
				print("real_path: "+str(real_path))
				self.pub.publish(",".join(real_path))


if __name__ == '__main__':

	rospy.init_node('route', anonymous=False)
	
	route = route()
	
	rospy.spin()
	