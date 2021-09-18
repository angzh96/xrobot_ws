#include "ros/ros.h"
#include "std_msgs/String.h"
#include <sstream>

#include "arch/linux/net_serial.h"
#include "user_protcol.h"

#include <geometry_msgs/TwistStamped.h>
#include <geometry_msgs/Point32.h>
#include <geometry_msgs/Pose.h>
#include "RawImu.h"
#include <sensor_msgs/Imu.h>

bool param_use_debug_imu;
bool param_use_debug_cmd;
bool param_use_debug_vel;
bool param_use_debug_arm;

/* serial */
std::string param_port_path_;
int param_baudrate_;
rp::hal::serial_rxtx* _rxtx = NULL;

void serial_open()
{
/*
* 	char port_path[] = "/dev/ttyUSB0";
* 	_u32 baudrate = 115200;
*	rp::hal::serial_rxtx* _rxtx = NULL;
*/
	int i = 10;
	if(_rxtx == NULL)
	{
		_rxtx = rp::hal::serial_rxtx::CreateRxTx();
	}
	while(i--)
	{
		if (!_rxtx->bind(param_port_path_.c_str(), param_baudrate_)  ||  !_rxtx->open()) 
		{
			ROS_ERROR("%s open ERROR", param_port_path_.c_str());
			sleep(5);
			continue;
		}
		_rxtx->flush(0);
		ROS_INFO("%s open OK, baudrate %d", param_port_path_.c_str(), param_baudrate_);
		return;
	}
	exit(0);
}
void serial_recv(uint8_t *recvBuffer, size_t recvSize)
{
	do
	{
		do
		{
			recvBuffer[0] = 0;
			if(rp::hal::serial_rxtx::ANS_OK != _rxtx->waitfordata(1 , -1, &recvSize))
			{
				ROS_ERROR("serial_rx waitfordata _PROTOCL_SYN_ERROR");
				exit(0);
				//if (!_rxtx->isOpened() ) serial_open();
				continue;
			}
			_rxtx->recvdata(&recvBuffer[0], 1);

			if(recvBuffer[0] != _SERIAL_SYN_CODE_START)
			{
				ROS_WARN("serial_rx _PROTOCL_SYN_START");
				continue;
			}
			break;
		}while (1);

		if(rp::hal::serial_rxtx::ANS_OK != _rxtx->waitfordata(sizeof(serialData) -1 , -1, &recvSize))
		{
			ROS_ERROR("serial_rx waitfordata DATA_ERROR");
			exit(0);
			//if (!_rxtx->isOpened() ) serial_open();
			continue;
		}

		_rxtx->recvdata(&recvBuffer[1], sizeof(serialData) - 1);
		if(recvBuffer[sizeof(serialData) - 2] != '\r'	&& recvBuffer[sizeof(serialData)-1] != '\n')
		{
			ROS_ERROR("serial_rx DATA_ERROR");
			continue;
		}
		return;
	}while(1);
}



/* callback */
void callback_cmd_vel(const geometry_msgs::Twist::ConstPtr& msg)
{
	if(param_use_debug_cmd)
	{
		// ROS_INFO("cmd_vel Line[%1.2f,%1.2f,%1.2f], Ang[%1.2f,%1.2f,%1.2f]",
		// 						msg->linear.x, msg->linear.y, msg->linear.z, 
		// 						msg->angular.x, msg->angular.y, msg->angular.z);
	}
	static serialData data;
	data.syn = 	_SERIAL_SYN_CODE_START;
	data.type = VAL_VEL;

	data.dat.vel.liner[0] = msg->linear.x;
	data.dat.vel.liner[1] = msg->linear.y;
	data.dat.vel.liner[2] = msg->linear.z;
	data.dat.vel.angular[0] = msg->angular.x;
	data.dat.vel.angular[1] = msg->angular.y;
	data.dat.vel.angular[2] = msg->angular.z;
	data.syn_CR = '\r';
	data.syn_LF = '\n';
	_rxtx->senddata((uint8_t*)&data, sizeof(serialData));					
}
void callback_cmd_pid(const geometry_msgs::Point32::ConstPtr& msg)
{
	if(param_use_debug_cmd)
	{
		ROS_INFO("cmd_pid [%1.1f,%1.1f,%1.1f]",	msg->x, msg->y, msg->z);
	}
	static serialData data;
	data.syn = 	_SERIAL_SYN_CODE_START;
	data.type = VAL_PID;
	data.dat.pid[0] = msg->x;
	data.dat.pid[1] = msg->y;
	data.dat.pid[2] = msg->z;
	data.syn_CR = '\r';
	data.syn_LF = '\n';
	_rxtx->senddata((uint8_t*)&data, sizeof(serialData));	
}

/**
 * This tutorial demonstrates simple sending of messages over the ROS system.
 */
int main(int argc, char *argv[])
{
	/* msg type */
	//geometry_msgs::Twist sub_msg_cmd_vel;
	geometry_msgs::TwistStamped pub_msg_pose;
	ros_arduino_msgs::RawImu pub_msg_imu_raw;
	sensor_msgs::Imu pub_msg_imu;
	geometry_msgs::Pose pub_msg_arm_pose;
	geometry_msgs::Pose pub_msg_arm_axis;
	/* ros */
	ros::init(argc, argv, "xrobot_serial_bridge_node");
	ros::NodeHandle n;
	ros::NodeHandle nh("~");


	nh.param<bool>("debug_imu", param_use_debug_imu, true);
	nh.param<bool>("debug_cmd", param_use_debug_cmd, false);
	nh.param<bool>("debug_vel", param_use_debug_vel, false);
	nh.param<bool>("debug_arm", param_use_debug_arm, false);
	nh.param<std::string>("port", param_port_path_, "/dev/xrobot_rosserial");
	nh.param<int>("baudrate", param_baudrate_, 115200);

	ros::Publisher pub_raw_pose = n.advertise<geometry_msgs::TwistStamped>("raw_vel", 50);
	ros::Publisher pub_raw_imu = n.advertise<ros_arduino_msgs::RawImu>("raw_imu", 50);
	ros::Publisher pub_imu = n.advertise<sensor_msgs::Imu>("imu/data_raw", 50);
	ros::Publisher pub_arm_pose = n.advertise<geometry_msgs::Pose>("xrobot/arm_pose", 50);
	ros::Publisher pub_arm_axis = n.advertise<geometry_msgs::Pose>("xrobot/arm_axis", 50);

	ros::Subscriber sub_cmd_vel = n.subscribe<geometry_msgs::Twist>("cmd_vel", 50, callback_cmd_vel);
	ros::Subscriber sub_cmd_pid = n.subscribe<geometry_msgs::Point32>("cmd_pid", 50, callback_cmd_pid);

	/* serial */
	serial_open();
	//ros::Rate loop_rate(1);
	/**
	 * A count of how many messages we have sent. This is used to create
	 * a unique string for each message.
	 */
	ROS_INFO("xrobot_serial_bridge Ready");
	serialData data;
	while (ros::ok())
	{
		//ROS_INFO("main loop");

		serial_recv((uint8_t*)&data, sizeof(serialData));
		// try
		// {
		// 	serial_recv((uint8_t*)&data, sizeof(serialData));
		// }
		// catch (boost::system::system_error ec)
		// {
		// 	ROS_ERROR("%s", ec.what());
		// 	break;
		// }
		if(data.type == VAL_ARM_AXIS)
		{

			pub_msg_arm_axis.orientation.x = data.dat.vel.angular[0];
			pub_msg_arm_axis.orientation.y = data.dat.vel.angular[1];
			pub_msg_arm_axis.orientation.z = data.dat.vel.angular[2];
			pub_msg_arm_axis.orientation.w = data.dat.vel.angular[3];
			pub_msg_arm_axis.position.x = data.dat.vel.liner[0];
			pub_msg_arm_axis.position.y = data.dat.vel.liner[1];
			pub_msg_arm_axis.position.z = data.dat.vel.liner[2];

			pub_arm_axis.publish(pub_msg_arm_axis);
			if(param_use_debug_arm)
			{
				ROS_INFO("ARM axis 1-5[%.2f, %.2f, %.2f, %.2f, %.2f] %0.2f，%0.2f", 
				data.dat.vel.angular[0], data.dat.vel.angular[1], data.dat.vel.angular[2], data.dat.vel.angular[3],	 data.dat.vel.liner[0], data.dat.vel.liner[1], data.dat.vel.liner[2]);
			}
		}
		if(data.type == VAL_ARM_POSE)
		{
			pub_msg_arm_pose.position.x = data.dat.vel.liner[0];
			pub_msg_arm_pose.position.y = data.dat.vel.liner[1];
			pub_msg_arm_pose.position.z = data.dat.vel.liner[2];
			pub_msg_arm_pose.orientation.x = data.dat.vel.angular[0];
			pub_msg_arm_pose.orientation.y = data.dat.vel.angular[1];
			pub_msg_arm_pose.orientation.z = data.dat.vel.angular[2];
			pub_msg_arm_pose.orientation.w = data.dat.vel.angular[3];
			pub_arm_pose.publish(pub_msg_arm_pose);
			if(param_use_debug_arm)
			{
				ROS_INFO("ARM RPY[%.2f, %.2f, %.2f， W%.2f]  XYZ[%.2f,%.2f,%.2f]", 
				data.dat.vel.liner[0], data.dat.vel.liner[1], data.dat.vel.liner[2],
				data.dat.vel.angular[0], data.dat.vel.angular[1], data.dat.vel.angular[2], data.dat.vel.angular[3]);
			}
		}
		if(data.type == VAL_POSE)
		{
			pub_msg_pose.header.stamp = ros::Time::now();
			pub_msg_pose.twist.linear.x = data.dat.vel.liner[0];
			pub_msg_pose.twist.linear.y = data.dat.vel.liner[1];
			pub_msg_pose.twist.linear.z = 0;
			pub_raw_pose.publish(pub_msg_pose);
			if(param_use_debug_vel)
			{
				ROS_INFO("VAL_POINT[%.1f,%.1f,%.1f]", pub_msg_pose.twist.linear.x
				, pub_msg_pose.twist.linear.y
				, pub_msg_pose.twist.linear.z);
			}

		}

		if(data.type == VAL_IMU_RAW)
		{
			pub_msg_imu_raw.header.stamp = ros::Time::now();
			pub_msg_imu_raw.header.frame_id = "imu_link";
			pub_msg_imu_raw.gyroscope = data.dat.imu.rot_ok;
			pub_msg_imu_raw.accelerometer = data.dat.imu.acc_ok;
			pub_msg_imu_raw.magnetometer = data.dat.imu.mag_ok;
			if(data.dat.imu.rot_ok)
			{
				pub_msg_imu_raw.raw_angular_velocity.x  = data.dat.imu.rot[0];
				pub_msg_imu_raw.raw_angular_velocity.y  = data.dat.imu.rot[1];
				pub_msg_imu_raw.raw_angular_velocity.z  = data.dat.imu.rot[2];
			}
			if(data.dat.imu.acc_ok)
			{
				pub_msg_imu_raw.raw_linear_acceleration.x = data.dat.imu.acc[0];
				pub_msg_imu_raw.raw_linear_acceleration.y = data.dat.imu.acc[1];
				pub_msg_imu_raw.raw_linear_acceleration.z = data.dat.imu.acc[2];
			}
			if(data.dat.imu.mag_ok)
			{
				pub_msg_imu_raw.raw_magnetic_field.x = data.dat.imu.mag[0];
				pub_msg_imu_raw.raw_magnetic_field.y = data.dat.imu.mag[1];
				pub_msg_imu_raw.raw_magnetic_field.z = data.dat.imu.mag[2];
			}
			pub_raw_imu.publish(pub_msg_imu_raw);
			if(param_use_debug_imu)
			{
				ROS_INFO("IMU ANG[%.1f,%.1f,%.1f] ACC[%.1f,%.1f,%.1f] MAG[%.1f,%.1f,%.1f]", 
				pub_msg_imu_raw.raw_angular_velocity.x, pub_msg_imu_raw.raw_angular_velocity.y, pub_msg_imu_raw.raw_angular_velocity.z, 
				pub_msg_imu_raw.raw_linear_acceleration.x, pub_msg_imu_raw.raw_linear_acceleration.y, pub_msg_imu_raw.raw_linear_acceleration.z
				, pub_msg_imu_raw.raw_magnetic_field.x, pub_msg_imu_raw.raw_magnetic_field.y, pub_msg_imu_raw.raw_magnetic_field.z);
			}	
		}

		if(data.type == VAL_IMU)
		{
			pub_msg_imu.header.stamp = ros::Time::now();
			pub_msg_imu.header.frame_id = "imu_link";
			
			pub_msg_imu.angular_velocity.x = data.dat.vel.angular[0];
			pub_msg_imu.angular_velocity.y = data.dat.vel.angular[1];
			pub_msg_imu.angular_velocity.z = data.dat.vel.angular[2];
			pub_imu.publish(pub_msg_imu);
			if(param_use_debug_imu)
			{
				ROS_INFO("IMU RPY[%f, %f, %f]XYZ", data.dat.vel.angular[0], data.dat.vel.angular[1], data.dat.vel.angular[2]);
			}
		}
		ros::spinOnce();

		//loop_rate.sleep();
	}
	_rxtx->close();
	rp::hal::serial_rxtx::ReleaseRxTx(_rxtx);
	ROS_INFO("xrobot_serial_bridge down");
	return 0;
}