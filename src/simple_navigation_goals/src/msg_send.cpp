#include <string.h>
#include "ros/ros.h"
#include "std_msgs/String.h"
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <cstring>

using namespace std;

int main(int argc, char **argv)
{
    ros::init(argc, argv, "ipSend");
    socklen_t iAddrLength;  
    char rgMessage[2014]; 
    int iOptval = 1;     
    int iFd;  
    struct sockaddr_in Addr; 
    char buf[512];
    struct ifconf ifconf;
    struct ifreq *ifreq;
    ifconf.ifc_len = 512;
    ifconf.ifc_buf = buf;
    const char *send_buf;

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
    //Addr.sin_addr.s_addr = INADDR_ANY;
    Addr.sin_port = htons(10011);  
    Addr.sin_addr.s_addr = inet_addr("192.168.0.106");
    iAddrLength = sizeof(Addr); 
    string s0 = "192.168.0." ;
    string s1;
    ioctl(iFd, SIOCGIFCONF, &ifconf);
    ifreq = (struct ifreq*)ifconf.ifc_buf;
    for(int i = (ifconf.ifc_len/sizeof(struct ifreq)); i>0; i--){
        if(ifreq->ifr_flags == AF_INET){//ipv4
        s1 = inet_ntoa(((struct sockaddr_in*)&(ifreq->ifr_addr))->sin_addr);
           if(s0.compare(0,10,s1,0,10) == 0){
               //ss << s1;
               send_buf = s1.c_str();
               //send_buf = (char*)s1.data();
               cout<<"=== ip: "<<s1<<" ====  "<<endl;
           }
        }
        ifreq++;
    }
    
    
    if (bind(iFd, (struct sockaddr *)&Addr, sizeof(Addr)) == -1)
    {
        printf("bind failed!\n");
    }        

    /*if (recvfrom(iFd, rgMessage, sizeof(rgMessage), 0, (struct sockaddr *)&Addr, &iAddrLength) == -1)
    {
            printf("recv failed!\n");
    } */          

    //string s(rgMessage);
   // const char *send_buf;
    while(1){
        //send_buf = s1.c_str();
        if(sendto(iFd, send_buf, 20, 0, (struct sockaddr *)&Addr, iAddrLength) == -1){
            printf("send failed!\n");
        }
        //cout<<"i have sent ip:"<<sizeof(send_buf)<<endl;
    }
    close(iFd);
    return 0;
}
