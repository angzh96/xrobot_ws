#!/usr/bin/env python
#coding:utf-8
import rospy
import websocket
import time
import threading
import math
from std_msgs.msg import String

destination = [None]  #目标点
send_mulgps = rospy.Publisher('recv_gps', String, queue_size=10)
gps_data = [None]
mul_gps = [None]
last_message = [None]*2

def cal_dis(lat1,lon1,lat2,lon2):
	latitude1 = (math.pi/180)*lat1
	latitude2 = (math.pi/180)*lat2
	longitude1 = (math.pi/180)*lon1
	longitude2= (math.pi/180)*lon2
	#因此AB两点的球面距离为:{arccos[sinb*siny+cosb*cosy*cos(a-x)]}*R
	#地球半径
	R = 6378.137
	d = math.acos(math.sin(latitude1)*math.sin(latitude2)+ math.cos(latitude1)*math.cos(latitude2)*math.cos(longitude2-longitude1))*R
	return d

def callback(data):
	gps_data[0] = "2,"+data.data
	if destination[0] != None:
		send_mulgps.publish(str(data.data+","+destination[0]))
		mul_gps[0] = str("2,"+data.data+","+destination[0])
		#print(mul_gps)

 
def listener():
	rospy.init_node('listener', anonymous=True)#声明一个订阅者节点
	rospy.Subscriber("GPS", String, callback)#订阅一个话题，收到话题数据后并做出相应的反应
 
	# spin() simply keeps python from exiting until this node is stopped
	rospy.spin()
 
def on_message(ws, message):  # 服务器有数据更新时，主动推送过来的数据

	real_message = message.split(",")
	if len(real_message)==3 and float(real_message[0])==1 and last_message[0] == None:
		destination[0] = str(real_message[1]+","+real_message[2])
		last_message[0] = float(real_message[1])
		last_message[1] = float(real_message[2])

	elif len(real_message)==3 and float(real_message[0])==1 and last_message[0] != None:
		distance = cal_dis(float(last_message[0]),float(last_message[1]),float(real_message[1]),float(real_message[2])) * 1000
		print("distance"+str(distance))
		if distance >= 5:
			destination[0] = str(real_message[1]+","+real_message[2])
			last_message[0] = float(real_message[1])
			last_message[1] = float(real_message[2])

def on_error(ws, error):  # 程序报错时，就会触发on_error事件
	print(error)

def on_close(ws):
	print("Connection closed ……")

def on_open(ws):  # 连接到服务器之后就会触发on_open事件，这里用于send数据

	while True:
		if gps_data[0] != None:
			ws.send(gps_data[0])
		if destination[0] != None and mul_gps[0] != None:
			ws.send(mul_gps[0])
			destination[0] = None

		time.sleep(10)

def send_websocket():
    #websocket.enableTrace(True)
	ws = websocket.WebSocketApp("ws://101.200.127.148:10086/websocket",
								on_message=on_message,
								on_error=on_error,
								on_close=on_close,
								on_open = on_open)
	ws.run_forever(ping_timeout=30)

def recv_websocket():
	#websocket.enableTrace(True)
	ws = websocket.WebSocketApp("ws://101.200.127.148:10086/websocket",
								on_message=on_message,
								on_error=on_error,
								on_close=on_close)
	ws.run_forever(ping_timeout=30)


if __name__ == '__main__':
	send_websocket = threading.Thread(target=send_websocket,name='websocket',args=())
	send_websocket.start()
	recv_websocket = threading.Thread(target=recv_websocket,name='websocket',args=())
	recv_websocket.start()

	listener()