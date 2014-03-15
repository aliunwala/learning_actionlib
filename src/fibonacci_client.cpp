#include <ros/ros.h>
#include <actionlib/client/simple_action_client.h>
#include <actionlib/client/terminal_state.h>
#include <learning_actionlib/FibonacciAction.h>


int main(int argc, char ** argv){
        ros::init( argc, argv, "test_fibonacci");

        actionlib::SimpleActionClient<learning_actionlib::FibonacciAction> ac("fibonacci", true);

        ROS_INFO("wating");
        ac.waitForServer();

        learning_actionlib::FibonacciGoal goal;
        goal.order = 20;
        ac.sendGoal(goal);

        bool finishedInTime = ac.waitForResult( ros::Duration(22.0));

        if(finishedInTime){
            actionlib::SimpleClientGoalState state = ac.getState();
            //learning_actionlib::FibonacciResult 
            std::vector<int> path = ac.getResult()->sequence;
            //ROS_INIFO("done %s", res.toString().c_str());
            for( std::vector<int>::const_iterator i = path.begin(); i != path.end(); ++i)
                    std::cout << *i << ' ';
        }
        else{
            ROS_INFO("failed");
        }
        return 0;
    }
