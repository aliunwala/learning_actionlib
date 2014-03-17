#include <ros/ros.h>
#include <actionlib/client/simple_action_client.h>
#include <actionlib/client/terminal_state.h>
#include <move_base_msgs/MoveBaseAction.h>
#include <map_mux/ChangeMap.h>

int main(int argc, char ** argv){
    const float a = sqrt(2);
    geometry_msgs::PoseStamped outsideElevPose;
    outsideElevPose.header.seq = 4; // Arbitriray?
    outsideElevPose.header.stamp.sec= 491;// Arbitriray?
    outsideElevPose.header.frame_id = "map"; 
    outsideElevPose.pose.position.x = 24;
    outsideElevPose.pose.position.y = 11;
    outsideElevPose.pose.position.z = 0;
    outsideElevPose.pose.orientation.x = 0;
    outsideElevPose.pose.orientation.y = 0;
    outsideElevPose.pose.orientation.z = -a;
    outsideElevPose.pose.orientation.w = a;

    geometry_msgs::PoseStamped insideElevPose;
    insideElevPose.header.seq = 4;
    insideElevPose.header.stamp.sec= 492;
    insideElevPose.header.frame_id = "map"; 
    insideElevPose.pose.position.x = 24;
    insideElevPose.pose.position.y = 8;
    insideElevPose.pose.position.z = 0;
    insideElevPose.pose.orientation.x = 0;
    insideElevPose.pose.orientation.y = 0;
    insideElevPose.pose.orientation.z = a;
    insideElevPose.pose.orientation.w = a;

    geometry_msgs::PoseStamped outsideElevPoseFloor2;
    outsideElevPoseFloor2.header.seq = 4; // Arbitriray?
    outsideElevPoseFloor2.header.stamp.sec= 491;// Arbitriray?
    outsideElevPoseFloor2.header.frame_id = "map"; 
    outsideElevPoseFloor2.pose.position.x = 24;
    outsideElevPoseFloor2.pose.position.y = 11;
    outsideElevPoseFloor2.pose.position.z = 0;
    outsideElevPoseFloor2.pose.orientation.x = 0;
    outsideElevPoseFloor2.pose.orientation.y = 0;
    outsideElevPoseFloor2.pose.orientation.z = -a;
    outsideElevPoseFloor2.pose.orientation.w = a;

     ros::init( argc, argv, "move_robot");
     ros::NodeHandle n;

    boost::shared_ptr<actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction> > robot_controller_;

    ROS_INFO("wating");
    robot_controller_.reset(
    new actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction>("move_base", true));
    robot_controller_->waitForServer();

   move_base_msgs::MoveBaseGoal goal;
   goal.target_pose = outsideElevPose;
   robot_controller_->sendGoal(goal);
   robot_controller_->waitForResult();
   
   goal.target_pose = insideElevPose;
   robot_controller_->sendGoal(goal);
   robot_controller_->waitForResult();
// 1) Wait for user input as to what floor I am on
// 2) switch map
    ros::ServiceClient client = n.serviceClient<map_mux::ChangeMap>("change_map");
    map_mux::ChangeMap srv ;
    srv.request.data = 2;
    if (client.call(srv)){
    }
    else{
        ROS_ERROR("service ChangeMap change_map failed");
    }

//give outside pose again
   goal.target_pose = outsideElevPoseFloor2;
   robot_controller_->sendGoal(goal);
   robot_controller_->waitForResult();

   //actionlib::SimpleClientGoalState state = robot_controller_->getState();
        return 0;
    }

// 1) init node
// 2) make simple action client
// 3) wait for server to comeup
// 4) send/make goal to action  (goal result feedback)
// 5) Wait for timeout
// 6) Catch resuly/feedback
// 7) Catch problem
//

