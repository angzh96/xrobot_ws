#include "xrobot_imu_bridge.h"

int main(int argc, char **argv)
{
  ros::init(argc, argv, "xrobot_imu_bridge");
  ros::NodeHandle nh, pnh("~");
  xrobotImuBridge xrobot_imu_bridge(nh, pnh);

  ros::spin();

  return 0;
}
