#include"ros/ros.h"
#include"geometry_msgs/Twist.h" 
#include "std_msgs/String.h"                              //包含geometry_msgs::Twist消息头文件
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

    ros::init(argc, argv, "base_controller");
    ros::NodeHandle n;
 
    ros::Publisher cmd_pub = n.advertise<geometry_msgs::Twist>("turtle1/cmd_vel",50);
    ros::Publisher navigation_pub = n.advertise<std_msgs::String>("navigation_1H8",50);
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

        if(rect_info.rect_w<=120&&rect_info.rect_w>0){
            msg.linear.x = 0.3;  
        }else if(rect_info.rect_w>=160){
            msg.linear.x = -0.3;
        }else{
            msg.linear.x = 0;
        }

        if(rect_info.rect_x<=800&&rect_info.rect_x>0){
            msg.angular.z = 0.3;
        }else if(rect_info.rect_x>=1000){
            msg.angular.z = -0.3;
        }else{
            msg.angular.z = 0;
        }

        if(rect_info.rect_w==0){
             i++;
        }else{
            i=0;
        }

        if(i>=10){
            msg.angular.z = 0.3;
        }

        if(msg.linear.x!=0|| msg.angular.z!=0){
            cmd_pub.publish(msg);
            printf("%d  %d  %d  %d\n",rect_info.rect_w,rect_info.rect_h,rect_info.rect_x,rect_info.rect_w*rect_info.rect_h);
            //loop_rate.sleep();
        }
        close(conn);
    }
    close(server_sockfd);
    return 0;
}