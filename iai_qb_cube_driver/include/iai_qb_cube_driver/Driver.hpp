#ifndef IAI_QB_CUBE_DRIVER_DRIVER_HPP
#define IAI_QB_CUBE_DRIVER_DRIVER_HPP

#include <qbmove_communications.h>
#include <ros/ros.h>
#include <sensor_msgs/JointState.h>

#include <pthread.h>


namespace iai_qb_cube_driver
{
  class Driver
  {
    public:
      Driver(const ros::NodeHandle& nh);
      ~Driver(); 

      // TODO: make run threaded
      void run();
  
      bool isRunning() const { return running_; }

    private:
      // Communication stuff
      ros::NodeHandle nh_;
      comm_settings cube_comm_;
      ros::Publisher pub_;
      sensor_msgs::JointState msg_;      

      // Internal flags
      bool running_;

      // Internal data members
      std::vector<std::string> joint_names_;
      std::map<std::string, int> cube_id_map_;
      std::string port_;
      double publish_frequency_;

      bool startCommunication(const std::string& port);
      bool startPublisher(const std::vector<std::string>& joint_names);
      void stopCommunication();
      void readMeasurements();
      bool readParameters(); 
      // TODO: add a callback for commands

      //Variables containing the commands that go out to the joints
      std::vector<float> joint_eqpoints;
      std::vector<float> joint_stiffness;


      //Things for the realtime thread
      double timeout_;
      bool exitRequested_;

      //members to stop and start the rt thread
      bool start_rt_thread(double timeout);
      void stop_rt_thread();

      //actual function running in the rt thread
      void* rt_run();

      //helper function to get the right pointer
      static void* run_s(void *ptr) { return ((Driver *) ptr)->rt_run(); }

      pthread_t thread_;
      pthread_mutex_t mutex_;

      //TODO: Need here variables for in-state and out-state



  };
}

#endif // IAI_QB_CUBE_DRIVER_DRIVER_HPP