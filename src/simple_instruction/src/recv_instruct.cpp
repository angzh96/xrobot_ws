#include"ros/ros.h"
#include"geometry_msgs/Twist.h" 
#include "std_msgs/String.h"
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

struct socket_info
{
    char category[10];
    float confident;
    int xmin;
    int xmax;
    int ymin;
    int ymax;
}

//left,right,stop,go ahead 

int main(int argc, char **argv)
{
     //socket========================================
     struct socket_info recv_info;
    ///定义sockfd
    int server_sockfd = socket(AF_INET,SOCK_STREAM, 0);
    
    ///定义sockaddr_in
    struct sockaddr_in server_sockaddr;
    server_sockaddr.sin_family = AF_INET;
    server_sockaddr.sin_port = htons(10012);
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
    //ros::Rate loop_rate(10);
    int i = 0;

    while(1){

        geometry_msgs::Twist msg;
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
        memcpy(&recv_info,info,sizeof(recv_info));
        if(recv_info.category=="stop"){
            msg.linear.x = 0;
        }else if(recv_info.category=="go ahead"){
            msg.linear.x = 0.3;
        }else if(recv_info.category=="left"){
            msg.angular.z = -0.3;
        }else if(recv_info.category=="right"){
            msg.angular.z = 0.3;
        }else {
            msg.linear.x = 0;
            msg.angular.z = 0;
        }

        if(msg.linear.x!=0|| msg.angular.z!=0){
            cmd_pub.publish(msg);
            //ROS_INFO("msg.linear.x:%f ,  msg.angular.z: %f",msg.linear.x,msg.angular.z);
            printf("Instruction recevied:%s\n",recv_info.category);
            //loop_rate.sleep();
        }
        close(conn);
    }
    close(server_sockfd);
    return 0;
}