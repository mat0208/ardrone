#include "ros/ros.h"
#include "std_msgs/Float32.h"
#include "vbat.h"

#include <sstream>

int main(int argc, char **argv)
{
  ros::init(argc, argv, "vbat_publisher");
  ros::NodeHandle n;

  vbat_struct vbat;

  if(vbat_init(&vbat)) return 1;

  ros::Publisher batter_pub = n.advertise<std_msgs::Float32>("battery_voltage", 1000);

  ros::Rate loop_rate(1.0);

  while (ros::ok())
  {
    std_msgs::Float32 msg;
  
    vbat_read(&vbat);
    msg.data=vbat.vbat;
  
    batter_pub.publish(msg);


    ros::spinOnce();

    loop_rate.sleep();
  }

  return 0;
}
