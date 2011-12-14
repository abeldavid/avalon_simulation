/* Generated from orogen/lib/orogen/templates/tasks/Task.cpp */

#include "SonarRear.hpp"
#include "Simulation.h"

using namespace avalon_simulation;

SonarRear::SonarRear(std::string const& name, TaskCore::TaskState initial_state)
    : SonarRearBase(name, initial_state)
{
}

SonarRear::SonarRear(std::string const& name, RTT::ExecutionEngine* engine, TaskCore::TaskState initial_state)
    : SonarRearBase(name, engine, initial_state)
{
}

SonarRear::~SonarRear()
{
}



/// The following lines are template definitions for the various state machine
// hooks defined by Orocos::RTT. See SonarRear.hpp for more detailed
// documentation about them.

// bool SonarRear::configureHook()
// {
//     if (! SonarRearBase::configureHook())
//         return false;
//     return true;
// }
// 
bool SonarRear::startHook()
{
    if (! SonarRearBase::startHook())
        return false;

    if(!Simulation::isInitialized())
        throw std::runtime_error("Cannot get sonar data. The simulator is not running in the same process.");
    
    return true;
}

void SonarRear::updateHook()
{
    SonarRearBase::updateHook();

    if(Simulation::getAvalonPlugin()->getRearSonarData(sonar_beam)){
      if (_sonar_beam.connected())
	_sonar_beam.write(sonar_beam);
    }
    else{
      sonar_beam.beam.clear();
    }
}
// void SonarRear::errorHook()
// {
//     SonarRearBase::errorHook();
// }
// void SonarRear::stopHook()
// {
//     SonarRearBase::stopHook();
// }
// void SonarRear::cleanupHook()
// {
//     SonarRearBase::cleanupHook();
// }

