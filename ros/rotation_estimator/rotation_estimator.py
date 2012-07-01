#!/usr/bin/python

import roslib
roslib.load_manifest('rotation_estimator')
import rospy

from navdata_publisher.msg import Navdata


def rotation_estimator():
  pose=[0,0,0]
  k=0
  state=[pose,k]

  def navdata_arrived(data):
    pose=state[0]
    k=state[1]

    gyro=data.gyro
    for i in range(3):
      pose[i]+=(gyro[i]/200.0)
      if pose[i] > 360:
        pose[i]-=360
      if pose[i] < 0:
        pose[i]+=360
      
    k+=1
    if k > 50:
      print "%6.2f %6.2f %6.2f"%(pose[0],pose[1],pose[2])
      k=0
      
    state[0]=pose
    state[1]=k


  rospy.init_node('rotation_estimator',anonymous=True)
  rospy.Subscriber('/navdata', Navdata, navdata_arrived)
  rospy.spin()
  
  
if __name__ == '__main__':
  rotation_estimator()  
  