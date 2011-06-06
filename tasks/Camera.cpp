/* Generated from orogen/lib/orogen/templates/tasks/Task.cpp */

#include "Camera.hpp"
#include "Simulation.h"

using namespace avalon_simulation;

Camera::Camera(std::string const& name, TaskCore::TaskState initial_state)
    : CameraBase(name, initial_state),simulator_interface(NULL)
{
}

Camera::Camera(std::string const& name, RTT::ExecutionEngine* engine, TaskCore::TaskState initial_state)
    : CameraBase(name, engine, initial_state),simulator_interface(NULL)
{
}

Camera::~Camera()
{
}

/// The following lines are template definitions for the various state machine
// hooks defined by Orocos::RTT. See Camera.hpp for more detailed
// documentation about them.

// bool Camera::configureHook()
// {
//     if (! CameraBase::configureHook())
//         return false;
//     return true;
// }

bool Camera::startHook()
{
    if (! CameraBase::startHook())
        return false;

    if(!Simulation::isInitialized())
        throw std::runtime_error("Cannot start Camera. The simulator is not running in the same process.");

    return true;
}

void Camera::updateHook()
{
    CameraBase::updateHook();

}

// void Camera::errorHook()
// {
//     CameraBase::errorHook();
// }
// void Camera::stopHook()
// {
//     CameraBase::stopHook();
// }
// void Camera::cleanupHook()
// {
//     CameraBase::cleanupHook();
// }

