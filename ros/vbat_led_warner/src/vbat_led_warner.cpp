#include "ros/ros.h"
#include "std_msgs/Float32.h"
#include "gpio.h"
#include <stdlib.h>

#define LIMIT_GREEN 12.0
#define LIMIT_ORANGE 11.0
#define LIMIT_RED 10.0

#define LED_GPIO_1 180
#define LED_GPIO_2 181

enum BottomLedColor {
  Green,
  Orange,
  Red,
  Off
};

void setBottomLedColor(enum BottomLedColor c)
{
  switch(c) {
    case Green:
      ROS_INFO("LED -> Green");
      gpio_set(LED_GPIO_1,1);
      gpio_set(LED_GPIO_2,0);
    break;
    case Orange:
      ROS_INFO("LED -> Orange");
      gpio_set(LED_GPIO_1,1);
      gpio_set(LED_GPIO_2,1);
    break;
    case Red:
      ROS_INFO("LED -> Red");
      gpio_set(LED_GPIO_1,0);
      gpio_set(LED_GPIO_2,1);
    break;
    case Off:
      ROS_INFO("LED -> Off");
      gpio_set(LED_GPIO_1,0);
      gpio_set(LED_GPIO_2,0);
    break;
  }

}



void vbatCallback(const std_msgs::Float32::ConstPtr& msg)
{
  float voltage=msg->data;
  ROS_INFO("Voltage received: [%f]", voltage);
  
  if(voltage > LIMIT_GREEN) {
    setBottomLedColor(Green);
  } else if(voltage > LIMIT_ORANGE) {
    setBottomLedColor(Orange);
  } else if(voltage > LIMIT_RED) {
      setBottomLedColor(Red);
  } else {
    setBottomLedColor(Off);
    ROS_ERROR("Shutting down");
    system("poweroff");
  }
  
  
  
  
}

int main(int argc, char **argv)
{
  ros::init(argc, argv, "vbat_led_warner");
  ros::NodeHandle n;

  ros::Subscriber sub = n.subscribe("battery_voltage", 1000, vbatCallback);
  ros::spin();

  return 0;
}
