/* Generated from orogen/lib/orogen/templates/tasks/Task.cpp */

#include "FrontCamera.hpp"
#include "Simulation.h"

using namespace avalon_simulation;

FrontCamera::FrontCamera(std::string const& name, TaskCore::TaskState initial_state)
    : FrontCameraBase(name, initial_state)
{
  frame.reset(new base::samples::frame::Frame());
}

FrontCamera::FrontCamera(std::string const& name, RTT::ExecutionEngine* engine, TaskCore::TaskState initial_state)
    : FrontCameraBase(name, engine, initial_state)
{
  frame.reset(new base::samples::frame::Frame);
}

FrontCamera::~FrontCamera()
{
}

bool FrontCamera::startHook()
{
    if (!FrontCameraBase::startHook())
        return false;

    if(!Simulation::isInitialized())
        throw std::runtime_error("Cannot start Camera. The simulator is not running in the same process.");

    return true;
}

void FrontCamera::updateHook()
{
    FrontCameraBase::updateHook();
    base::samples::frame::Frame *pframe = frame.write_access();
    if(Simulation::getAvalonPlugin()->getFrontCameraFrame(*pframe))
    {
        frame.reset(pframe);
       _frame.write(frame);
    }
    else
        frame.reset(pframe);
}

/// The following lines are template definitions for the various state machine
// hooks defined by Orocos::RTT. See FrontCamera.hpp for more detailed
// documentation about them.

// bool FrontCamera::configureHook()
// {
//     if (! FrontCameraBase::configureHook())
//         return false;
//     return true;
// }
// bool FrontCamera::startHook()
// {
//     if (! FrontCameraBase::startHook())
//         return false;
//     return true;
// }
// void FrontCamera::updateHook()
// {
//     FrontCameraBase::updateHook();
// }
// void FrontCamera::errorHook()
// {
//     FrontCameraBase::errorHook();
// }
// void FrontCamera::stopHook()
// {
//     FrontCameraBase::stopHook();
// }
// void FrontCamera::cleanupHook()
// {
//     FrontCameraBase::cleanupHook();
// }

