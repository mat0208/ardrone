#include "ros/ros.h"
#include "navboard.h"
#include "navdata_publisher/Navdata.h"
#include "navdata_publisher/RawNavdata.h"

#include <sstream>
#include <iostream>

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
  ros::Publisher rawNavdata_pub = n.advertise<RawNavdata>("rawNavdata", 100);

  // loop rate is gouverned by navboard (200 hz)
  ros::Rate loop_rate(200.0);


  while (ros::ok())
  {
    rc = nav_GetSample(&nav);
    if(rc) {
      printf("ERROR: nav_GetSample return code=%d\n",rc); 
    } else {
      Navdata msg;
      RawNavdata raw;

      msg.navboard_seq=nav.seq;
      raw.navboard_seq=nav.seq;
      
      msg.acceleration[0]=nav.ax;
      msg.acceleration[1]=nav.ay;
      msg.acceleration[2]=nav.az;
      msg.gyro[0]=nav.gx;
      msg.gyro[1]=nav.gy;
      msg.gyro[2]=nav.gz;

      for (int i=0;i<3;i++) { 
        raw.acc[i]=nav.acc[i];
        raw.gyro[i]=nav.gyro[i];
      }

      raw.unk1=nav.unk1;
      raw.unk2=nav.unk2;
      
      msg.height_us=nav.h;
      msg.height_fresh=nav.h_meas;
      
      raw.us_echo_high_bit=nav.us_echo >> 15;
      raw.us_echo=nav.us_echo & 0x7fff;
      
      raw.us_echo_start=nav.us_echo_start;
      raw.us_echo_end=nav.us_echo_end;

      msg.accelTemperature=nav.ta;
      msg.gyroTemperature=nav.tg;
      
      raw.acc_temp=nav.acc_temp;
      raw.gyro_temp=nav.gyro_temp;
      
      raw.vrefEpson=nav.vrefEpson;
      raw.vrefIDG=nav.vrefIDG;
      
      
      raw.us_association_echo=nav.us_association_echo;
      raw.us_distance_echo=nav.us_distance_echo;
      raw.us_courbe_temps=nav.us_courbe_temps;
      raw.us_courbe_valeur=nav.us_courbe_valeur;
      raw.us_courbe_ref=nav.us_courbe_ref;
      
      for(int i=0;i<7;i++) {
        msg.newFloat[i]=nav.newStuff[i]*1.0;
        raw.newUint16[i]=nav.newStuff[i];
      }
  
      navdata_pub.publish(msg);
      rawNavdata_pub.publish(raw);
    }
    
    std::cout << ".";
    std::cout.flush();
      ros::spinOnce();
//      loop_rate.sleep();

  }
  nav_Close();

  return 0;
}
