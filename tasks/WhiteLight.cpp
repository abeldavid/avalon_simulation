/* Generated from orogen/lib/orogen/templates/tasks/Task.cpp */

#include "WhiteLight.hpp"
#include "Simulation.h"


using namespace avalon_simulation;

WhiteLight::WhiteLight(std::string const& name, TaskCore::TaskState initial_state)
    : WhiteLightBase(name, initial_state)
{
}

WhiteLight::WhiteLight(std::string const& name, RTT::ExecutionEngine* engine, TaskCore::TaskState initial_state)
    : WhiteLightBase(name, engine, initial_state)
{
}

WhiteLight::~WhiteLight()
{
}

void WhiteLight::switchLight()
{
  
}

/// The following lines are template definitions for the various state machine
// hooks defined by Orocos::RTT. See WhiteLight.hpp for more detailed
// documentation about them.

// bool WhiteLight::configureHook()
// {
//     if (! WhiteLightBase::configureHook())
//         return false;
//     return true;
// }
bool WhiteLight::startHook()
{
    if (! WhiteLightBase::startHook())
        return false;
    return true;
}
void WhiteLight::updateHook()
{
    WhiteLightBase::updateHook();
    
    Simulation::getAvalonPlugin()->switchWhiteLight();
}
// void WhiteLight::errorHook()
// {
//     WhiteLightBase::errorHook();
// }
// void WhiteLight::stopHook()
// {
//     WhiteLightBase::stopHook();
// }
// void WhiteLight::cleanupHook()
// {
//     WhiteLightBase::cleanupHook();
// }

