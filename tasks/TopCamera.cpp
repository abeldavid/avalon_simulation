/* Generated from orogen/lib/orogen/templates/tasks/Task.cpp */

#include "TopCamera.hpp"
#include "Simulation.h"

using namespace avalon_simulation;

TopCamera::TopCamera(std::string const& name, TaskCore::TaskState initial_state)
    : TopCameraBase(name, initial_state)
{
  frame.reset(new base::samples::frame::Frame());
}

TopCamera::TopCamera(std::string const& name, RTT::ExecutionEngine* engine, TaskCore::TaskState initial_state)
    : TopCameraBase(name, engine, initial_state)
{
  frame.reset(new base::samples::frame::Frame());
}

TopCamera::~TopCamera()
{
}
/// The following lines are template definitions for the various state machine
// hooks defined by Orocos::RTT. See TopCamera.hpp for more detailed
// documentation about them.

// bool TopCamera::configureHook()
// {
//     if (! TopCameraBase::configureHook())
//         return false;
//     return true;
// }
bool TopCamera::startHook()
{
    if (! TopCameraBase::startHook())
        return false;
    
    if(!Simulation::isInitialized())
        throw std::runtime_error("Cannot start Camera. The simulator is not running in the same process.");

    Simulation::getAvalonPlugin()->activateTopCamera(true);

    return true;
}

void TopCamera::updateHook()
{
    TopCameraBase::updateHook();
    
    base::samples::frame::Frame *pframe = frame.write_access();
    if(Simulation::getAvalonPlugin()->getTopCameraFrame(*pframe))
    {
        frame.reset(pframe);
       _frame.write(frame);
    }
    else
        frame.reset(pframe);
}

// void TopCamera::errorHook()
// {
//     TopCameraBase::errorHook();
// }

void TopCamera::stopHook()
{
    Simulation::getAvalonPlugin()->activateTopCamera(false);
    TopCameraBase::stopHook();
}

// void TopCamera::cleanupHook()
// {
//     TopCameraBase::cleanupHook();
// }

