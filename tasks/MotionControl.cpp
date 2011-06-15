/* Generated from orogen/lib/orogen/templates/tasks/Task.cpp */

#include "MotionControl.hpp"
#include "Simulation.h"

using namespace avalon_simulation;

MotionControl::MotionControl(std::string const& name, TaskCore::TaskState initial_state)
    : MotionControlBase(name, initial_state)
{
}

MotionControl::MotionControl(std::string const& name, RTT::ExecutionEngine* engine, TaskCore::TaskState initial_state)
    : MotionControlBase(name, engine, initial_state)
{
}

MotionControl::~MotionControl()
{
}


/// The following lines are template definitions for the various state machine
// hooks defined by Orocos::RTT. See MotionControl.hpp for more detailed
// documentation about them.

// bool MotionControl::configureHook()
// {
//     if (! MotionControlBase::configureHook())
//         return false;
//     return true;
// }

bool MotionControl::startHook()
{
    if (! MotionControlBase::startHook())
        return false;

    if(!Simulation::isInitialized())
        throw std::runtime_error("Cannot start MotionControl. The simulator is not running in the same process.");

    return true;
}

void MotionControl::updateHook()
{
    MotionControlBase::updateHook();

    base::samples::RigidBodyState pose;
    if (_pose_samples.read(pose) == RTT::NoData)
        return state(WAITING_FOR_ORIENTATION);

    if (!Simulation::getSimulatorInterface()->isRunning()) 
        return;

    base::AUVMotionCommand commands;
    if(_motion_commands.readNewest(commands) == RTT::NewData)
    {
        double heading = base::getYaw(pose.orientation);
        std::vector<double> pmw;
        pmw.push_back(commands.x_speed);                              //x
        pmw.push_back(commands.x_speed);                              //x
        pmw.push_back(commands.y_speed);                              //y
        pmw.push_back(fmod((commands.heading - heading),M_PI)*10);         // heading
        pmw.push_back(commands.z-pose.position.z());                  //z
        pmw.push_back(0);                                             //pitch 
        if (!Simulation::getSimulatorInterface()->isRunning()) 
            return;
        Simulation::getAvalonPlugin()->setTarget(pmw);
    }
}

// void MotionControl::errorHook()
// {
//     MotionControlBase::errorHook();
// }
// void MotionControl::stopHook()
// {
//     MotionControlBase::stopHook();
// }
// void MotionControl::cleanupHook()
// {
//     MotionControlBase::cleanupHook();
// }

