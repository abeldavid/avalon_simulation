/* Generated from orogen/lib/orogen/templates/tasks/Task.cpp */

#include "PoseEstimator.hpp"
#include "Simulation.h"

using namespace avalon_simulation;

PoseEstimator::PoseEstimator(std::string const& name, TaskCore::TaskState initial_state)
    : PoseEstimatorBase(name, initial_state)
{
}

PoseEstimator::PoseEstimator(std::string const& name, RTT::ExecutionEngine* engine, TaskCore::TaskState initial_state)
    : PoseEstimatorBase(name, engine, initial_state)
{
}

PoseEstimator::~PoseEstimator()
{
}



/// The following lines are template definitions for the various state machine
// hooks defined by Orocos::RTT. See PoseEstimator.hpp for more detailed
// documentation about them.

// bool PoseEstimator::configureHook()
// {
//     if (! PoseEstimatorBase::configureHook())
//         return false;
//     return true;
// }
//
bool PoseEstimator::startHook()
{
    if (! PoseEstimatorBase::startHook())
        return false;

    if(!Simulation::isInitialized())
        throw std::runtime_error("Cannot start Camera. The simulator is not running in the same process.");

    return true;
}
 void PoseEstimator::updateHook()
 {
     PoseEstimatorBase::updateHook();

     base::samples::RigidBodyState pose;
     Eigen::Quaterniond q;
     Eigen::Vector3d p;
     Simulation::getAvalonPlugin()->getPose(p,q);
     pose.position = p;
     pose.time = base::Time::now();
     pose.orientation = q;
     _pose_samples.write(pose);
 }
// void PoseEstimator::errorHook()
// {
//     PoseEstimatorBase::errorHook();
// }
// void PoseEstimator::stopHook()
// {
//     PoseEstimatorBase::stopHook();
// }
// void PoseEstimator::cleanupHook()
// {
//     PoseEstimatorBase::cleanupHook();
// }

