#include "ros/ros.h"
#include "std_msgs/String.h"
#include<iostream>
#include <string.h>
#include "geometry_msgs/Twist.h"
#include <iterator>
#include <valarray>
#include <boost/thread.hpp>

using namespace std;


class multiThreadListener
{
public:
	multiThreadListener()
	{	
		cmd_pub = n.advertise<geometry_msgs::Twist>("cmd_vel",50);
        route_pub = n.advertise<std_msgs::String>("send_route",50);
        joy_sub = n.subscribe("joy_vel", 10, &multiThreadListener::chatterCallback1,this);
        auto_sub = n.subscribe("auto_vel", 10, &multiThreadListener::chatterCallback2,this);
	}
	void chatterCallback1(const geometry_msgs::Twist::ConstPtr& joy_vel);
	void chatterCallback2(const geometry_msgs::Twist::ConstPtr& auto_vel);

private:
	ros::NodeHandle n;
    ros::Publisher cmd_pub;
    ros::Publisher route_pub;
    ros::Subscriber joy_sub;
    ros::Subscriber auto_sub;
  
};


/*摇杆控制车*/
 void multiThreadListener::chatterCallback1(const geometry_msgs::Twist::ConstPtr& joy_vel)
{
    geometry_msgs::Twist send_msg; 
    /*自动驾驶path设为空，自动驾驶停止*/
    std_msgs::String path;
    std::stringstream ss;
    ss << "";
    path.data = ss.str();
    route_pub.publish(path);
    /*joy摇杆控制为主要控制*/
    send_msg.linear.x = joy_vel->linear.x;
    send_msg.angular.z = joy_vel->angular.z;
    cmd_pub.publish(send_msg);
    
}


/*自动驾驶控制车*/
void multiThreadListener::chatterCallback2(const geometry_msgs::Twist::ConstPtr& auto_vel)
{
    geometry_msgs::Twist send_msg;
    send_msg.linear.x = auto_vel->linear.x;
    send_msg.angular.z = auto_vel->angular.z;
    cmd_pub.publish(send_msg); 
}

  

int main(int argc, char **argv)
{

  ros::init(argc, argv, "multi_sub");

  multiThreadListener listener_obj;
  
  ros::AsyncSpinner spinner(2); // Use 2 threads
  spinner.start();
  ros::waitForShutdown();

  return 0;
}