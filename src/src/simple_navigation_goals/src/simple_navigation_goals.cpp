#include <ros/ros.h>
#include <move_base_msgs/MoveBaseAction.h>
#include <actionlib/client/simple_action_client.h>
#include "std_msgs/String.h"
#include <string>
#include <stdio.h>  
#include <stdlib.h>   
#include <string.h>  
#include <sys/socket.h>  
#include <netinet/in.h>  
#include <arpa/inet.h>  
#include <netdb.h> 
#include <unistd.h> 
using namespace std;
typedef actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction> MoveBaseClient;


int main(int argc, char **argv) {  
    ros::init(argc, argv, "simple_navigation");
    ros::NodeHandle n;
    socklen_t iAddrLength;  
    char rgMessage[2014]; 
    int iOptval = 1;     
    int iFd;  
    double x_now = 0;
    double y_now = 0;
    double w_now = 0;
    struct sockaddr_in Addr;  

    if ((iFd = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
    {
        printf("socket fail\n");
        return -1;
    }   
  
    if (setsockopt(iFd, SOL_SOCKET, SO_REUSEADDR, &iOptval, sizeof(int)) < 0)
    {
        printf("setsockopt failed!\n");
    }
    memset(&Addr, 0, sizeof(struct sockaddr_in));  
    Addr.sin_family = AF_INET;  
    Addr.sin_addr.s_addr = INADDR_ANY;
    Addr.sin_port = htons(10086);  
    iAddrLength = sizeof(Addr);  
    
    if (bind(iFd, (struct sockaddr *)&Addr, sizeof(Addr)) == -1)
    {
        printf("bind failed!\n");
    }        
    int j = 0;
    while (1)  
    {  
        j++;
        if (recvfrom(iFd, rgMessage, sizeof(rgMessage), 0, (struct sockaddr *)&Addr, &iAddrLength) == -1)
        {
            printf("recv failed!\n");
        }           
        string s(rgMessage);
        cout<<s<<s.length()<<endl;

        double x,y,w;
        double x_now,y_now,w_now;
        int start = 0;
        int count = 1;
        for (int i = 0;i < s.length();i++){

            if(s[i] == ',' && count == 1){
                x = atof(s.substr(start,i).c_str());
                count += 1;
                start = i+1;
            }
            else if(s[i] == ',' && count == 2){
                y = atof(s.substr(start,i).c_str());
                start = i+1;
                count += 1;
            }
            if(count == 3){
                w =  atof(s.substr(start,s.length()).c_str());
                break;
            }
        }
        x = x - 0.5;
        cout<< "========================================="<<endl;
        cout<<"x = "<<x<<endl;
        cout<<"y = "<<y<<endl;
        cout<<"w = "<<w<<endl;
        cout<< "========================================="<<endl;
        MoveBaseClient ac("move_base", true);
        //wait for the action server to come up
        while(!ac.waitForServer(ros::Duration(5.0))){
            ROS_INFO("Waiting for the move_base action server to come up");
         }

        move_base_msgs::MoveBaseGoal goal;

        //we'll send a goal to the robot to move 1 meter forward
        goal.target_pose.header.frame_id = "map"; //base_link
        goal.target_pose.header.stamp = ros::Time::now();

        if(j>=10&&(x-x_now)*(x-x_now)+(y-y_now)*(y-y_now)>1){
            goal.target_pose.pose.position.x = x;
            goal.target_pose.pose.position.y = y;
            goal.target_pose.pose.orientation.w = w;

            x_now = x;
            y_now = y;
            w_now = w;

            ROS_INFO("Sending goal");
            ac.sendGoal(goal);
            j = 0;
        }
        // if((x-x_now)*(x-x_now)+(y-y_now)*(y-y_now)>1){
        //     goal.target_pose.pose.position.x = x;
        //     goal.target_pose.pose.position.y = y;
        //     goal.target_pose.pose.orientation.w = w;

        //     x_now = x;
        //     y_now = y;
        //     w_now = w;
        //     ROS_INFO("Sending goal");
        //     ac.sendGoal(goal);
        // }
        // ROS_INFO("Sending goal");
        // ac.sendGoal(goal);

       // ac.waitForResult();

        if(ac.getState() == actionlib::SimpleClientGoalState::SUCCEEDED)
            cout<<"get the goal ,zzy"<<endl;
        else
            cout<<"can't go"<<endl;
    }  
    close(iFd);   
    return 0;  
}
