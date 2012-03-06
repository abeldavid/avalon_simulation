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
        throw std::runtime_error("Cannot get sonar data. The simulator is not running in the same process.");
    
    return true;
}

void SonarTop::updateHook()
{
    SonarTopBase::updateHook();
    
    simulation::SonarConfig config;
    config.start_angle = _start_angle.get();
    config.end_angle = _end_angle.get();
    config.max_distance = _maximum_distance.get();
    config.distance_resolution = _resolution.get();
    config.ping_pong_mode = _ping_pong_mode.get();
    Simulation::getAvalonPlugin()->configureTopSonar(config);

    if(Simulation::getAvalonPlugin()->getTopSonarData(sonar_beam)){
       if (_sonar_beam.connected())
           _sonar_beam.write(sonar_beam);
    }
    else{
        sonar_beam.beam.clear();
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

