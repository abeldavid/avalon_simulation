/* Generated from orogen/lib/orogen/templates/tasks/Task.cpp */

#include "GroundDistance.hpp"
#include "Simulation.h"

using namespace avalon_simulation;

GroundDistance::GroundDistance(std::string const& name, TaskCore::TaskState initial_state)
    : GroundDistanceBase(name, initial_state)
{
}

GroundDistance::GroundDistance(std::string const& name, RTT::ExecutionEngine* engine, TaskCore::TaskState initial_state)
    : GroundDistanceBase(name, engine, initial_state)
{
}

GroundDistance::~GroundDistance()
{
}



/// The following lines are template definitions for the various state machine
// hooks defined by Orocos::RTT. See GroundDistance.hpp for more detailed
// documentation about them.

// bool GroundDistance::configureHook()
// {
//     if (! GroundDistanceBase::configureHook())
//         return false;
//     return true;
// }
//
bool GroundDistance::startHook()
{
    if (! GroundDistanceBase::startHook())
        return false;

    if(!Simulation::isInitialized())
        throw std::runtime_error("Cannot get sonar data. The simulator is not running in the same process.");

    return true;
}

void GroundDistance::updateHook()
{
    GroundDistanceBase::updateHook();

    base::samples::RigidBodyState gdist;
    double d = Simulation::getAvalonPlugin()->getGroundDistance();
    Eigen::Vector3d pos = Eigen::Vector3d(0.0,0.0,d);
    gdist.position = pos;
    _ground_distance.write(gdist);

}
// void GroundDistance::errorHook()
// {
//     GroundDistanceBase::errorHook();
// }
// void GroundDistance::stopHook()
// {
//     GroundDistanceBase::stopHook();
// }
// void GroundDistance::cleanupHook()
// {
//     GroundDistanceBase::cleanupHook();
// }

