/* Generated from orogen/lib/orogen/templates/tasks/Task.cpp */

#include "StateEstimator.hpp"
#include "Simulation.h"

using namespace avalon_simulation;

StateEstimator::StateEstimator(std::string const& name, TaskCore::TaskState initial_state)
    : StateEstimatorBase(name, initial_state)
{
}

StateEstimator::StateEstimator(std::string const& name, RTT::ExecutionEngine* engine, TaskCore::TaskState initial_state)
    : StateEstimatorBase(name, engine, initial_state)
{
}

StateEstimator::~StateEstimator()
{
}



/// The following lines are template definitions for the various state machine
// hooks defined by Orocos::RTT. See StateEstimator.hpp for more detailed
// documentation about them.

// bool StateEstimator::configureHook()
// {
//     if (! StateEstimatorBase::configureHook())
//         return false;
//     return true;
// }

bool StateEstimator::startHook()
{
    if (! StateEstimatorBase::startHook())
        return false;

    return true;
}

void StateEstimator::updateHook()
{
    StateEstimatorBase::updateHook();

    if(!Simulation::isInitialized())
        throw std::runtime_error("Cannot start Camera. The simulator is not running in the same process.");

    base::samples::RigidBodyState pose;
    Eigen::Quaterniond q;
    Eigen::Vector3d p;
    Simulation::getAvalonPlugin()->getPose("avalon", p,q);

    Eigen::Vector3d lin_vel;
    Eigen::Vector3d ang_vel;

    Simulation::getAvalonPlugin()->getVelocities("avalon", lin_vel, ang_vel);

    lin_vel = q.inverse() * lin_vel;
    ang_vel = q.inverse() * ang_vel;

    pose.position = p;
    pose.time = base::Time::now();
    pose.orientation = q;
    pose.velocity = lin_vel;
    pose.angular_velocity = ang_vel;

    _pose_samples.write(pose);
}

// void StateEstimator::errorHook()
// {
//     StateEstimatorBase::errorHook();
// }
// void StateEstimator::stopHook()
// {
//     StateEstimatorBase::stopHook();
// }
// void StateEstimator::cleanupHook()
// {
//     StateEstimatorBase::cleanupHook();
// }

