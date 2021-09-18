# xrobot_ws
一、auto_driver

智能无人系统功能包，和前后端配合，室外自动导航功能

运行mux.launch和后端进行tcp和udp连接，tcp为控制连接，udp和后端进行环境数据通信

二、simple_instruction

目标实时监测功能包，从连接在MLU220上的摄像头实时获取图片，检测直行、左转、右转、停止的标志并输出结果。

将检测结果映射为相应指令与小车通信，发送指令。运行move_instruction.launch。


simple_navigation_goals

小车协同追踪功能包。具体见我之前发的pdf文件说明

主车运行tracking.launch，从车运行coordination.launch
