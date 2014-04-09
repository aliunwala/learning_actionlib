#include <ros/ros.h>
#include <actionlib/client/simple_action_client.h>
#include <actionlib/client/terminal_state.h>
#include <move_base_msgs/MoveBaseAction.h>
#include <map_mux/ChangeMap.h>
#include <geometry_msgs/PoseWithCovarianceStamped.h>
#include <segbot_simulation_apps/DoorHandlerInterface.h>

int main(int argc, char ** argv){
    const float a = sqrt(2);
    geometry_msgs::PoseStamped outsideElevPose;
    outsideElevPose.header.seq = 0; // Arbitriray?
    outsideElevPose.header.stamp.sec= 0;// Arbitriray?
    outsideElevPose.header.frame_id = "map"; 
    outsideElevPose.pose.position.x = 10.3;
    outsideElevPose.pose.position.y = 50.12;
    outsideElevPose.pose.position.z = 0;
    outsideElevPose.pose.orientation.x = 0;
    outsideElevPose.pose.orientation.y = 0;
    outsideElevPose.pose.orientation.z = 1;
    outsideElevPose.pose.orientation.w = 0;

    geometry_msgs::PoseStamped insideElevPose;
    insideElevPose.header.seq = 0;
    insideElevPose.header.stamp.sec= 0;
    insideElevPose.header.frame_id = "map"; 
    insideElevPose.pose.position.x = 6.97;
    insideElevPose.pose.position.y = 50.42;
    insideElevPose.pose.position.z = 0;
    insideElevPose.pose.orientation.x = 0;
    insideElevPose.pose.orientation.y = 0;
    insideElevPose.pose.orientation.z = 0;
    insideElevPose.pose.orientation.w = 1;

    geometry_msgs::PoseStamped outsideElevPoseFloor2;
    outsideElevPoseFloor2.header.seq = 0; // Arbitriray?
    outsideElevPoseFloor2.header.stamp.sec= 0;// Arbitriray?
    outsideElevPoseFloor2.header.frame_id = "map"; 
    outsideElevPoseFloor2.pose.position.x = 24.17;
    outsideElevPoseFloor2.pose.position.y = 32.3;
    outsideElevPoseFloor2.pose.position.z = 0;
    outsideElevPoseFloor2.pose.orientation.x = 0;
    outsideElevPoseFloor2.pose.orientation.y = 0;
    outsideElevPoseFloor2.pose.orientation.z = -a;
    outsideElevPoseFloor2.pose.orientation.w = a;

    geometry_msgs::PoseWithCovarianceStamped initalPositionFloor2;
    initalPositionFloor2.header.seq = 0;
    initalPositionFloor2.header.stamp.sec = 0;
    initalPositionFloor2.header.stamp.nsec = 0;
    initalPositionFloor2.header.frame_id = "map";
    initalPositionFloor2.pose.pose.position.x = 24.46  ;
    initalPositionFloor2.pose.pose.position.y = 29.62;
    initalPositionFloor2.pose.pose.position.z = 0 ;
    initalPositionFloor2.pose.pose.orientation.x = 0;
    initalPositionFloor2.pose.pose.orientation.y = 0;
    initalPositionFloor2.pose.pose.orientation.z = 0.76631578005;
    initalPositionFloor2.pose.pose.orientation.w = 0.642464104247;
    initalPositionFloor2.pose.covariance = {};// {0.25, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.25, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.06853891945200942} ;




    ros::init( argc, argv, "move_robot");
    ros::NodeHandle n;
    //ros::ServiceClient client_opendoors = n.serviceClient<segbot_simulation_apps::DoorHandlerInterface>("update_doors");
    //segbot_simulation_apps::DoorHandlerInterface open_door;
    //open_door.request.door = "" ;
    //open_door.request.open = true;
    //open_door.request.all_doors= true;
    //if (client_opendoors.call(open_door)){
        //ROS_INFO("running the service");
    //}
    //else{
        //ROS_ERROR("Failed the service");
    //}

    ros::ServiceClient client = n.serviceClient<map_mux::ChangeMap>("change_map");
    map_mux::ChangeMap srv ;


    boost::shared_ptr<actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction> > robot_controller_;
    ros::Publisher initialPosePub= n.advertise<geometry_msgs::PoseWithCovarianceStamped>("initialpose", 1000);

    ROS_INFO("wating");
    robot_controller_.reset( new actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction>("move_base", true));
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
    srv.request.data = 2;
    if (client.call(srv)){
    }
    else{
        ROS_ERROR("service ChangeMap change_map failed");
    }

// reset inital position after new map put into place
    initialPosePub.publish(initalPositionFloor2);

      ros::Duration(3).sleep();
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

