/* Generated from orogen/lib/orogen/templates/tasks/Task.cpp */

#include "SonarTop.hpp"
#include "Simulation.h"

using namespace avalon_simulation;

SonarTop::SonarTop(std::string const& name, TaskCore::TaskState initial_state)
    : SonarTopBase(name, initial_state)
{
}

SonarTop::SonarTop(std::string const& name, RTT::ExecutionEngine* engine, TaskCore::TaskState initial_state)
    : SonarTopBase(name, engine, initial_state)
{
}

SonarTop::~SonarTop()
{
}

bool SonarTop::startHook()
{
    if (!SonarTopBase::startHook())
        return false;

    if(!Simulation::isInitialized())
        throw std::runtime_error("Cannot start sonar data. The simulator is not running in the same process.");
    
    return true;
}

void SonarTop::updateHook()
{
    SonarTopBase::updateHook();

    Simulation::getAvalonPlugin()->configureTopSonar(_leftlimit.get(),_rightlimit.get(),_numberofbins.get(),
            _adinterval.get(),_cont.get());

    if(Simulation::getAvalonPlugin()->getTopSonarData(&sonarscan)){
       if (_sonarscan.connected())
           _sonarscan.write(sonarscan);
    }
    else{
        sonarscan.scanData.clear();
    }
}

/// The following lines are template definitions for the various state machine
// hooks defined by Orocos::RTT. See SonarTop.hpp for more detailed
// documentation about them.

// bool SonarTop::configureHook()
// {
//     if (! SonarTopBase::configureHook())
//         return false;
//     return true;
// }

// void SonarTop::updateHook()
// {
//     SonarTopBase::updateHook();
// }
// void SonarTop::errorHook()
// {
//     SonarTopBase::errorHook();
// }
// void SonarTop::stopHook()
// {
//     SonarTopBase::stopHook();
// }
// void SonarTop::cleanupHook()
// {
//     SonarTopBase::cleanupHook();
// }

