#!/usr/bin/env python
#coding:utf-8
import rospy
import websocket
import time
import threading
from std_msgs.msg import String

destination = [None]  #目标点
send_mulgps = rospy.Publisher('recv_gps', String, queue_size=10)
gps_data = [None]
mul_gps = [None]

def callback(data):
	gps_data[0] = "1,"+data.data
	if destination[0] != None:
		send_mulgps.publish(str(data.data+","+destination[0]))
		print(str(data.data+","+destination[0]))
		mul_gps[0] = str("1,"+data.data+","+destination[0])
		#print(mul_gps)

 
def listener():
	rospy.init_node('listener', anonymous=True)#声明一个订阅者节点
	rospy.Subscriber("GPS", String, callback)#订阅一个话题，收到话题数据后并做出相应的反应
 
	# spin() simply keeps python from exiting until this node is stopped
	rospy.spin()
 
def on_message(ws, message):  # 服务器有数据更新时，主动推送过来的数据
	#destination[0] = message
	print("message"+message)

def on_error(ws, error):  # 程序报错时，就会触发on_error事件
	print(error)

def on_close(ws):
	print("Connection closed ……")

def on_open(ws):  # 连接到服务器之后就会触发on_open事件，这里用于send数据

	while True:
		print(destination[0])
		if gps_data[0] != None:
			ws.send(gps_data[0])
		if destination[0] != None and mul_gps[0] != None:
			ws.send(mul_gps[0])
			destination[0] = None

		time.sleep(10)

def init_websocket():
	#websocket.enableTrace(True)
	ws = websocket.WebSocketApp("ws://101.200.127.148:10086/websocket",
								on_message=on_message,
								on_error=on_error,
								on_close=on_close,
								on_open = on_open)
	ws.run_forever(ping_timeout=30)


if __name__ == '__main__':
	destination[0] = "30.317629,120.349707"
	send_websocket = threading.Thread(target=init_websocket,name='websocket',args=())
	send_websocket.start()

	listener()