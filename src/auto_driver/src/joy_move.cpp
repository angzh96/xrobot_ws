/******************************************************************
基于串口通信的ROS小车基础控制器，功能如下：
1.实现ros控制数据通过固定的格式和串口通信，从而达到控制小车的移动
2.订阅了/cmd_vel主题，只要向该主题发布消息，就能实现对控制小车的移动
3.发布里程计主题/odm

串口通信说明：
1.写入串口
（1）内容：左右轮速度，单位为mm/s
（2）格式：１０字节,[右轮速度４字节][左轮速度４字节][结束符"\r\n"２字节]
2.读取串口
（1）内容：小车x,y坐标，方向角，线速度，角速度，单位依次为：mm,mm,rad,mm/s,rad/s
（2）格式：２１字节，[Ｘ坐标４字节][Ｙ坐标４字节][方向角４字节][线速度４字节][角速度４字节][结束符"\n"１字节]
*******************************************************************/
#include"ros/ros.h"
#include"geometry_msgs/Twist.h" 
#include "std_msgs/String.h"                            //包含geometry_msgs::Twist消息头文件
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/time.h>
#include <unistd.h>
 
class Joy_controller
{
public:
 Joy_controller(){ 
    cmd_pub = n.advertise<geometry_msgs::Twist>("joy_vel",50);
    joy_sub = n.subscribe("recv_joy", 10, &Joy_controller::chatterCallback,this);
 }
 void chatterCallback(const std_msgs::String::ConstPtr& recv_msg){
    geometry_msgs::Twist send_msg;
    
    if(recv_msg->data=="w"){
        send_msg.linear.x = 0.3; 
    }else if(recv_msg->data=="s"){
        send_msg.linear.x = -0.3; 
    }

    if(recv_msg->data=="a"){
        send_msg.angular.z = 0.3;
    }else if(recv_msg->data=="d"){
        send_msg.angular.z = -0.3;
    }
    if(send_msg.linear.x!=0|| send_msg.angular.z!=0){
        cmd_pub.publish(send_msg);
        ROS_INFO("msg.linear.x:%f ,  msg.angular.z: %f",send_msg.linear.x,send_msg.angular.z);
    }
 }

private:
    ros::NodeHandle n;
    ros::Publisher cmd_pub;
    ros::Subscriber joy_sub;
};



int main(int argc, char **argv)
{

    ros::init(argc, argv, "joy_move");//turtle1/

    Joy_controller Joy_controller;
    
    ros::spin();
    return 0;
}