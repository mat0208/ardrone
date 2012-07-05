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
      msg.orientation_mag[0]=nav.mag_x;
      msg.orientation_mag[1]=nav.mag_y;
      msg.orientation_mag[2]=nav.mag_z;

      for (int i=0;i<3;i++) { 
        raw.acc[i]=nav.acc[i];
        raw.gyro[i]=nav.gyro[i];
        raw.mag[i]=nav.mag[i];
      }

      raw.unk1=nav.unk1;
      raw.unk2=nav.unk2;
      raw.unk3=nav.unk3;
      raw.unk4=nav.unk4;
      raw.unk5=nav.unk5;
      raw.unk6=nav.unk6;
      raw.unk7=nav.unk7;
      raw.unk8=nav.unk8;
      
      msg.height_us=nav.h;
      msg.height_fresh=nav.h_meas;
      
      raw.us_echo_high_bit=nav.us_echo >> 15;
      raw.us_echo=nav.us_echo & 0x7fff;
      
      raw.us_echo_start=nav.us_echo_start;
      raw.us_echo_end=nav.us_echo_end;
      
      raw.us_association_echo=nav.us_association_echo;
      raw.us_distance_echo=nav.us_distance_echo;
      raw.us_courbe_temps=nav.us_courbe_temps;
      raw.us_courbe_valeur=nav.us_courbe_valeur;
      raw.us_number_echo=nav.us_number_echo;
      raw.us_sum_echo=(nav.us_sum_echo_2 <<16) + nav.us_sum_echo_1;
      
      msg.us_initialized=nav.us_initialized;
      raw.us_initialized=nav.us_initialized;
      
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
