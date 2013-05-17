/* Generated from orogen/lib/orogen/templates/tasks/Task.cpp */

#include "StateEstimator.hpp"
#include "Simulation.h"

using namespace avalon_simulation;

StateEstimator::StateEstimator(std::string const& name)
    : StateEstimatorBase(name)
{
}

StateEstimator::StateEstimator(std::string const& name, RTT::ExecutionEngine* engine)
    : StateEstimatorBase(name, engine)
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

	node_name = _node_name.get();

    return true;
}

void StateEstimator::updateHook()
{
    StateEstimatorBase::updateHook();

    if(!Simulation::isInitialized())
        throw std::runtime_error("Cannot start StateEstimator. The simulator is not running in the same process.");

    base::samples::RigidBodyState pose;
    Eigen::Quaterniond q;
    Eigen::Vector3d p;
    Simulation::getAvalonPlugin()->getPose(node_name, p,q);

    Eigen::Vector3d lin_vel;
    Eigen::Vector3d ang_vel;

    Simulation::getAvalonPlugin()->getVelocities(node_name, lin_vel, ang_vel);

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

