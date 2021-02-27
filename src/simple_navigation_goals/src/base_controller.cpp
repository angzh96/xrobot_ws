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
 
#define STATE_X_FORWARD = 0
#define STATE_X_BACK = 1
#define STATE_X_ZERO = 2
#define STATE_X_LEVEL =3
#define STATE_W_RIGHT = 4
#define STATE_W_LEFT = 5
#define STATE_W_MIDDLE = 6
#define STATE_W_LEVEL =7


struct socket_info
{
	int rect_w;//方框宽
	int rect_h;//方框高
	int rect_x;//方框中心点所处坐标x
	int rect_y;//方框中心点所处坐标y
};



 
int main(int argc, char **argv)
{
     //socket========================================
    struct socket_info rect_info;
    ///定义sockfd
    int server_sockfd = socket(AF_INET,SOCK_STREAM, 0);
    
    ///定义sockaddr_in
    struct sockaddr_in server_sockaddr;
    server_sockaddr.sin_family = AF_INET;
    server_sockaddr.sin_port = htons(10010);
    server_sockaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    
    ///bind，成功返回0，出错返回-1
    if(bind(server_sockfd,(struct sockaddr *)&server_sockaddr,sizeof(server_sockaddr))==-1)
    {
        perror("bind");
        exit(1);
    }
    
    ///listen，成功返回0，出错返回-1
    if(listen(server_sockfd,10) == -1)
    {
        perror("listen");
        exit(1);
    }
    
    ///客户端套接字
    char info[100];
    struct sockaddr_in client_addr;
    socklen_t length = sizeof(client_addr);
    
    printf("等待客户端连接\n");
    //=====================================================

    ros::init(argc, argv, "base_controller");//turtle1/
    ros::NodeHandle n;
 
    ros::Publisher cmd_pub = n.advertise<geometry_msgs::Twist>("cmd_vel",50);
    ros::Publisher navigation_pub = n.advertise<std_msgs::String>("navigation_1H8",50);
    //ros::Rate loop_rate(10);
    int i = 0;

    while(1){


        geometry_msgs::Twist msg;
        std_msgs::String sign;
        int conn = accept(server_sockfd, (struct sockaddr*)&client_addr, &length);
        if(conn<0)
        {
            perror("connect");
            exit(1);
        }
        msg.linear.x = 0;
        msg.angular.z = 0;
        memset(info,0,sizeof(info));
        int len = recv(conn, info, sizeof(info),0);
        memcpy(&rect_info,info,sizeof(rect_info));
        // msg.linear.x = 0.3;                     //随机函数
        // msg.angular.z = 0;
        if(rect_info.rect_w>0){
            std::stringstream ss;
            ss << "1";
            sign.data = ss.str();
            //ROS_INFO("%s", msg.data.c_str());
            navigation_pub.publish(sign);
        }

        if(rect_info.rect_w<=200&&rect_info.rect_w>0){
            
            msg.linear.x = 0.3;  
        }else if(rect_info.rect_w>=300){
            msg.linear.x = -0.3;
        }else{
            msg.linear.x = 0;
        }

        if(rect_info.rect_x<=900&&rect_info.rect_x>0){
            msg.angular.z = 0.3;
        }else if(rect_info.rect_x>=1100){
            msg.angular.z = -0.3;
        }else{
            msg.angular.z = 0;
        }

        if(i>=10){
            msg.angular.z = 0.3;
        }

        if(msg.linear.x!=0|| msg.angular.z!=0){
            cmd_pub.publish(msg);
            //ROS_INFO("msg.linear.x:%f ,  msg.angular.z: %f",msg.linear.x,msg.angular.z);
            printf("%d  %d  %d  %d\n",rect_info.rect_w,rect_info.rect_h,rect_info.rect_x,rect_info.rect_w*rect_info.rect_h);
            //loop_rate.sleep();
        }
        close(conn);
    }
    close(server_sockfd);
    return 0;
}