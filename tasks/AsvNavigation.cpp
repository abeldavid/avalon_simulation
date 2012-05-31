/* Generated from orogen/lib/orogen/templates/tasks/Task.cpp */

#include "AsvNavigation.hpp"
#include "Simulation.h"

using namespace avalon_simulation;

AsvNavigation::AsvNavigation(std::string const& name, TaskCore::TaskState initial_state)
    : AsvNavigationBase(name, initial_state)
{
}

AsvNavigation::AsvNavigation(std::string const& name, RTT::ExecutionEngine* engine, TaskCore::TaskState initial_state)
    : AsvNavigationBase(name, engine, initial_state)
{
}

AsvNavigation::~AsvNavigation()
{
}



/// The following lines are template definitions for the various state machine
// hooks defined by Orocos::RTT. See AsvNavigation.hpp for more detailed
// documentation about them.

// bool AsvNavigation::configureHook()
// {
//     if (! AsvNavigationBase::configureHook())
//         return false;
//     return true;
// }
bool AsvNavigation::startHook()
{
    if (! AsvNavigationBase::startHook())
        return false;
    return true;
}

void AsvNavigation::updateHook()
{
    AsvNavigationBase::updateHook();
}

// void AsvNavigation::errorHook()
// {
//     AsvNavigationBase::errorHook();
// }
void AsvNavigation::stopHook()
{
    AsvNavigationBase::stopHook();
}
// void AsvNavigation::cleanupHook()
// {
//     AsvNavigationBase::cleanupHook();
// }

void AsvNavigation::addWaypoint(double x, double y){
  
  Simulation::getAvalonPlugin()->addWaypoint(x,y);
}



void AsvNavigation::clearWaypoints(){
  Simulation::getAvalonPlugin()->clearWaypoints();
}

