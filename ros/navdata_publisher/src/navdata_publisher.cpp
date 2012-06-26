#include "ros/ros.h"
#include "navboard.h"
#include "navdata_publisher/Navdata.h"

#include <sstream>

using namespace navdata_publisher;

int main(int argc, char **argv)
{
  ros::init(argc, argv, "navdata_publisher");
  ros::NodeHandle n;

  struct nav_struct nav;
  int rc = nav_Init(&nav);
  if(rc) {printf("Failed init: retcode=%d\r\n",rc); return rc;}

  rc=nav_FlatTrim();
  if(rc) {
    printf("Failed flattrim: retcode=%d\r\n",rc);
  }


  ros::Publisher navdata_pub = n.advertise<Navdata>("navdata", 100);
  

  ros::Rate loop_rate(10.0);



  while (ros::ok())
  {
    rc = nav_GetSample(&nav);
    if(rc) {
      printf("ERROR: nav_GetSample return code=%d\n",rc); 
    } else {
      Navdata msg;

      msg.navboard_seq=nav.seq;
      msg.acceleration[0]=nav.ax;
      msg.acceleration[1]=nav.ay;
      msg.acceleration[2]=nav.az;
      msg.gyro[0]=nav.gx;
      msg.gyro[1]=nav.gy;
      msg.gyro[2]=nav.gz;
      msg.accelTemperature=nav.ta;
      msg.gyroTemperature=nav.tg;
      msg.vrefEpson=nav.vrefEpson*1.0;
      msg.vrefIDG=nav.vrefIDG*1.0;
      msg.height_us=nav.h;
      msg.us_echo_start=nav.us_echo_start;
      msg.us_echo_end=nav.us_echo_end;
      msg.us_association_echo=nav.us_association_echo;
      msg.us_distance_echo=nav.us_distance_echo;
      msg.us_courbe_temps=nav.us_courbe_temps;
      msg.us_courbe_valeur=nav.us_courbe_valeur;
      msg.us_courbe_ref=nav.us_courbe_ref;
      for(int i=0;i<7;i++) {
        msg.newFloat[i]=nav.newStuff[i]*1.0;
        msg.newUint16[i]=nav.newStuff[i];
      }
  
      navdata_pub.publish(msg);
    }

      ros::spinOnce();
      loop_rate.sleep();

  }


  return 0;
}