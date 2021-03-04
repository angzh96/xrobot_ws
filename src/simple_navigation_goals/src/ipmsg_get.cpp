#include <ros/ros.h>
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




class multiThreadListener
{
public:
     multiThreadListener(){
         pub = nh.advertise<std_msgs::String>("ipMsg", 10);
         sub = nh.subscribe("navigation_1H8", 1, &multiThreadListener::subCallback,this);
     }
     void subCallback(const std_msgs::String::ConstPtr& msg);

private:
    ros::NodeHandle nh;
    ros::Publisher pub;
    ros::Subscriber sub;
    
};
void multiThreadListener::subCallback(const std_msgs::String::ConstPtr& msg){
    socklen_t iAddrLength;
    char rgMessage[2014]; 
    int iOptval = 1;     
    int iFd;  
    struct sockaddr_in Addr;      
    std_msgs::String ipmsg;   

    if ((iFd = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
    {
        printf("socket fail\n");
        return ;
    }   
  
    if (setsockopt(iFd, SOL_SOCKET, SO_REUSEADDR, &iOptval, sizeof(int)) < 0)
    {
        printf("setsockopt failed!\n");
    }
    memset(&Addr, 0, sizeof(struct sockaddr_in));  
    Addr.sin_family = AF_INET;  
    Addr.sin_addr.s_addr = INADDR_ANY;
    Addr.sin_port = htons(10011);  
    iAddrLength = sizeof(Addr); 
    if (bind(iFd, (struct sockaddr *)&Addr, sizeof(Addr)) == -1)
    {
        printf("bind failed!\n");
    }        

    if (recvfrom(iFd, rgMessage, sizeof(rgMessage), 0, (struct sockaddr *)&Addr, &iAddrLength) == -1)
    {
        printf("recv failed!\n");
    }else{
        string s(rgMessage);
        //cout<<"I get ip:"<<s<<endl;
        std::stringstream ss;
        ss << s;
        ipmsg.data = ss.str();
        pub.publish(ipmsg); 
    }                
    close(iFd);
}

int main(int argc, char **argv){

    ros::init(argc, argv, "ipMsgGet");

    multiThreadListener listener_obj;

    ros::AsyncSpinner spinner(4); // Use 2 threads
    spinner.start();
    ros::waitForShutdown();

    return 0;
}
