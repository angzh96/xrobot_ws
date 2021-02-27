#include <string.h>
#include "ros/ros.h"
#include "std_msgs/String.h"
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/if.h>

using namespace std;

int main(int argc, char **argv)
{
    ros::init(argc, argv, "msg_send");
    ros::NodeHandle nh;
    ros::Publisher pub = nh.advertise<std_msgs::String>("ipMsg", 20);
    std_msgs::String msg;
    string s0 = "192.168.0." ;
    string s1;
    struct ifconf ifconf;
    struct ifreq *ifreq;
    int iOptval = 1;
    int iFd;
    char buf[512];
    ifconf.ifc_len = 512;
    ifconf.ifc_buf = buf;
    std::stringstream ss;

    if ((iFd = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
    {
        printf("socket fail\n");
        return -1;
    }   
  
    if (setsockopt(iFd, SOL_SOCKET, SO_REUSEADDR, &iOptval, sizeof(int)) < 0)
    {
        printf("setsockopt failed!\n");
    }
    ioctl(iFd, SIOCGIFCONF, &ifconf);
    ifreq = ifconf.ifc_req;
    //cout<<"----111---"<<ifconf.ifc_len/sizeof(struct ifreq)<<endl;
    for(int i = (ifconf.ifc_len/sizeof(struct ifreq)); i>0; i--){
        //cout<<"----222---"<<endl;
        if(ifreq->ifr_flags == AF_INET){//ipv4
        //cout<<"----333---"<<endl;
           ioctl(iFd, SIOCGIFADDR, &ifconf);
           s1 = inet_ntoa(((struct sockaddr_in*)&(ifreq->ifr_addr))->sin_addr);
           cout<<"ip::::"<<s1<<endl;
           if(s0.compare(0,10,s1,0,10) == 0){
              ss << s1;
           }
        }
        ifreq++;
    }
    
    //std::stringstream ss("192.168.0.101");
    msg.data = ss.str();
    pub.publish(msg);
    int n=0;
    while (1)
    {
        if(n == 0){
            pub.publish(msg);
            //cout<<"msg:"<<msg.data<<endl;
        }
        n++;
        if(n==50){
            n = 0;
        }
    }
    
    return 0;
}
