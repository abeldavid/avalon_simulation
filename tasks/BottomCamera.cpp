/* Generated from orogen/lib/orogen/templates/tasks/Task.cpp */

#include "BottomCamera.hpp"
#include "Simulation.h"

using namespace avalon_simulation;

BottomCamera::BottomCamera(std::string const& name, TaskCore::TaskState initial_state)
    : BottomCameraBase(name, initial_state)
{
    frame.reset(new base::samples::frame::Frame());
}

BottomCamera::BottomCamera(std::string const& name, RTT::ExecutionEngine* engine, TaskCore::TaskState initial_state)
    : BottomCameraBase(name, engine, initial_state)
{
    frame.reset(new base::samples::frame::Frame);
}

BottomCamera::~BottomCamera()
{
}
/// The following lines are template definitions for the various state machine
// hooks defined by Orocos::RTT. See BottomCamera.hpp for more detailed
// documentation about them.

// bool BottomCamera::configureHook()
// {
//     if (! BottomCameraBase::configureHook())
//         return false;
//     return true;
// }
bool BottomCamera::startHook()
{
    if (! BottomCameraBase::startHook())
        return false;

    if(!Simulation::isInitialized())
        throw std::runtime_error("Cannot start Camera. The simulator is not running in the same process.");

    Simulation::getAvalonPlugin()->activateBottomCamera(true);

    return true;
}
void BottomCamera::updateHook()
{
    BottomCameraBase::updateHook();
    base::samples::frame::Frame *pframe = frame.write_access();
    if(Simulation::getAvalonPlugin()->getBottomCameraFrame(*pframe))
    {
        frame.reset(pframe);
       _frame.write(frame);
    }
    else
        frame.reset(pframe);
}

// void BottomCamera::errorHook()
// {
//     BottomCameraBase::errorHook();
// }

void BottomCamera::stopHook()
{
    Simulation::getAvalonPlugin()->activateBottomCamera(false);
    BottomCameraBase::stopHook();
}

// void BottomCamera::cleanupHook()
// {
//     BottomCameraBase::cleanupHook();
// }

