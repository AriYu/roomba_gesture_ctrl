#include <ros/ros.h>

// #include <joy/Joy.h>
#include <sensor_msgs/Joy.h>
//#include <roomba_500driver_meiji/RoombaCtrl.h>
#include "/home/yuta/catkin_ws/src/roomba_robot/roomba_500driver_meiji/msg_gen/cpp/include/roomba_500driver_meiji/RoombaCtrl.h"
#include <geometry_msgs/PointStamped.h>
#include <tf/transform_listener.h>


#include <boost/thread.hpp>
boost::mutex cntl_mutex_;

#include <iostream>
#include <math.h>
using namespace std;

ros::Publisher pub_state;

void gesture_callback(const tf::TransformListener& listener){
    tf::StampedTransform transform_r;
    tf::StampedTransform transform_l;
    try{
      listener.lookupTransform("/right_hand", "/torso", ros::Time(0), transform_r);
      listener.lookupTransform("/left_hand",  "/torso", ros::Time(0), transform_l);
      ROS_INFO("right x=%f", transform_r.getOrigin().x());
      ROS_INFO("left  x=%f", transform_l.getOrigin().x());
    } catch (tf::TransformException ex) {
      ROS_ERROR("%s", ex.what());
      ROS_INFO("NONE");
    }
}

int main(int argc, char** argv) {
	
    ros::init(argc, argv, "gesture_drive");
    ros::NodeHandle n;
    tf::TransformListener listener;
    roomba_500driver_meiji::RoombaCtrl ctrl;
    pub_state=n.advertise<roomba_500driver_meiji::RoombaCtrl>("/roomba/control", 100);
  
    ros::Rate loop_rate(10);

    double theta    = 0;
    double velocity = 0;

    double pos_right = 0;
    double pos_left  = 0;

    while(ros::ok())
    {
        tf::StampedTransform transform_r;
        tf::StampedTransform transform_l;
        try{
            listener.lookupTransform("/right_hand_1", "/torso_1", ros::Time(0), transform_r);
            listener.lookupTransform("/left_hand_1",  "/torso_1", ros::Time(0), transform_l);
            pos_right = -transform_l.getOrigin().y();
            pos_left  = -transform_r.getOrigin().y();
            velocity = (pos_right + pos_left)/2.0;
            theta    = -(pos_right - pos_left)/2.0;
            // ROS_INFO("Left %f", pos_left);
            // ROS_INFO("Right %f\n", pos_right);
            ROS_INFO("Veloc %f", velocity);
            ROS_INFO("Theta %f\n", theta);
            ctrl.cntl.linear.x  = velocity;
            ctrl.cntl.angular.z = theta;
            ctrl.mode           = roomba_500driver_meiji::RoombaCtrl::DRIVE_DIRECT;
            pub_state.publish(ctrl);

        } catch (tf::TransformException ex) {
            ROS_ERROR("%s", ex.what());
            ROS_INFO("NONE");
            velocity = 0;
            theta    = 0;
        }
        ros::spinOnce();
        loop_rate.sleep();
    }
    
    return 0;
}

