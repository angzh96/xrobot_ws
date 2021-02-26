#include "ros/ros.h"
#include "std_msgs/String.h"
#include <boost/thread.hpp>
#include "sensor_msgs/LaserScan.h"
#include<iostream>
#include "math.h"
#include <string.h>
#include<list>
#include <boost/algorithm/string.hpp>
#include"geometry_msgs/Twist.h"
#include <iostream>
#include <iterator>
#include <valarray>

using namespace std;

#define RAD2DEG(x) ((x)*180./M_PI)
#define pi 3.1415926535897932384626433832795
#define EARTH_RADIUS 6378.137 //地球半径 KM

struct latlon
{
    float lat;
    float lon;
};


double rad(double d)
{
    return d * pi /180.0;
}

//距离
double RealDistance(double lat1,double lng1,double lat2,double lng2)//lat1第一个点纬度,lng1第一个点经度,lat2第二个点纬度,lng2第二个点经度
{
	
	double a;
   	double b;
   	double radLat1 = rad(lat1);
   	double radLat2 = rad(lat2);
   	a = radLat1 - radLat2;
   	b = rad(lng1) - rad(lng2);
   	double s = 2 * asin(sqrt(pow(sin(a/2),2) + cos(radLat1)*cos(radLat2)*pow(sin(b/2),2)));
    s = s * EARTH_RADIUS;
    s = s * 1000;
    return s;
}

//方向
int get_angle(double lat1, double lng1, double lat2, double lng2)
{
	double lat1_rad = lat1 * pi /180;
	double lng1_rad = lng1 * pi /180;
	double lat2_rad = lat2 * pi /180;
	double lng2_rad = lng2 * pi /180;

	double x = sin(lng2_rad - lng1_rad) * cos(lat2_rad);
	double y = cos(lat1_rad) * sin(lat2_rad) - sin(lat1_rad) * cos(lat2_rad) * cos(lng2_rad - lng1_rad);

	float brng = atan2(x,y) * 180 / pi;
	int fangxiang  = brng + 360;

	return fangxiang % 360;
}


class multiThreadListener
{
    public:
        multiThreadListener()
        {	
            cmd_pub = n.advertise<geometry_msgs::Twist>("cmd_vel",50);  
            GY85 = n.subscribe("GY85", 50, &multiThreadListener::get_gy85,this);
            Lidar = n.subscribe("scan", 1000, &multiThreadListener::get_lidar,this);
            car_direction = -1;
        }

        void get_gy85(const std_msgs::String::ConstPtr& msg);
        void get_lidar(const sensor_msgs::LaserScan::ConstPtr& scan);
        void move();

    private:
        ros::NodeHandle n;
        ros::Subscriber GY85;
        ros::Subscriber Lidar;
        ros::Publisher cmd_pub;

        float car_direction ; //实时方向
        float new_liadr[90]; //雷达数据

   
};


/*获取GY85车头实时方向数据*/
void multiThreadListener::get_gy85(const std_msgs::String::ConstPtr& msg)
{
    //ROS_INFO("I heard: [%s]", msg->data.c_str());
    int direction = atof(msg->data.c_str()) - 25;
    if(direction >=0){
        car_direction = direction;
    }else{
        car_direction = direction + 360;
    }
    // printf("%d\n",car_direction);
}

/*获取雷达实时数据*/
void multiThreadListener::get_lidar(const sensor_msgs::LaserScan::ConstPtr& scan)
{
    for(int i = 0; i < 90; i++) {
        new_liadr[i] = scan->ranges[i+135];
        //printf("%f \n",new_liadr[i]);
    }
}


void multiThreadListener::move(){
    
    int i = 0;
    int last_min = -1;
    geometry_msgs::Twist msg; 

    while(true){
    
       int move_direction = 285;

       float min = *min_element(new_liadr,new_liadr + sizeof(new_liadr)/sizeof(new_liadr[0]));
       int min_index = distance(new_liadr, min_element(new_liadr, new_liadr + sizeof(new_liadr)/sizeof(new_liadr[0])));
       if(min <= 0.4){
            if(min_index >=0 && min_index <=45 && (last_min == -1 || (last_min >= 0 && last_min <=45)) && i < 2){
                msg.linear.x = 0;
                msg.angular.z = 0.4;
            }else if(min_index > 45 && min_index < 90 && (last_min == -1 || (last_min > 45 && last_min < 90)) && i < 2){
                msg.linear.x = 0;
                msg.angular.z = -0.4;
            }else if(min_index >=0 && min_index <=45 && (last_min > 45 && last_min < 90) && i < 2){
                msg.linear.x = 0;
				msg.angular.z = 0.4;
				i++;
            }else if(min_index > 45 && min_index < 90 && (last_min >= 0 && last_min <= 45) && i < 2){
                msg.linear.x = 0;
				msg.angular.z = -0.4;
				i++;
            }else if(i >= 2){
                msg.linear.x = 0;
				msg.angular.z = -0.4;
            }
            last_min = min_index;
       }
       else{
            i = 0;
            last_min = -1;
            if((car_direction - move_direction <= 150 && car_direction - move_direction > 1) || (car_direction - move_direction +360 <= 150 && car_direction - move_direction +360 > 1)){
                msg.linear.x = 0.2;
                msg.angular.z = 0.2;
            }else if((car_direction - move_direction < -1 && car_direction - move_direction >= -150) || (car_direction - move_direction -360 <= -1 && car_direction - move_direction -360 >= -150)){
                msg.linear.x = 0.2;
                msg.angular.z = -0.2;
            }else if(car_direction - move_direction == 0){
                msg.linear.x = 0.3;
                msg.angular.z = 0;
            }else{
                msg.linear.x = 0;
                msg.angular.z = -0.4;
            }

       }
       printf("%f  %f  %f\n",car_direction,msg.linear.x,msg.angular.z);
       cmd_pub.publish(msg);
       sleep(1);
    }
}


int main(int argc, char **argv)
{

    ros::init(argc, argv, "multi_sub");

    multiThreadListener listener_obj;
    
    ros::AsyncSpinner spinner(4); // Use 2 threads
    spinner.start();
    listener_obj.move();
    ros::waitForShutdown();

    return 0;
}