/* Generated from orogen/lib/orogen/templates/tasks/Task.cpp */

#include "PingerSearch.hpp"
#include "Simulation.h"

#include "base/angle.h"

using namespace avalon_simulation;

PingerSearch::PingerSearch(std::string const& name, TaskCore::TaskState initial_state)
    : PingerSearchBase(name, initial_state)
{
}

PingerSearch::PingerSearch(std::string const& name, RTT::ExecutionEngine* engine, TaskCore::TaskState initial_state)
    : PingerSearchBase(name, engine, initial_state)
{
}

PingerSearch::~PingerSearch()
{
}



/// The following lines are template definitions for the various state machine
// hooks defined by Orocos::RTT. See PingerSearch.hpp for more detailed
// documentation about them.

// bool PingerSearch::configureHook()
// {
//     if (! PingerSearchBase::configureHook())
//         return false;
//     return true;
// }

 bool PingerSearch::startHook()
 {
    if (! PingerSearchBase::startHook())
        return false;

    if(!Simulation::isInitialized())
        throw std::runtime_error("Cannot get sonar data. The simulator is not running in the same process.");
    
    return true;
 }

 void PingerSearch::updateHook()
 {
    PingerSearchBase::updateHook();
    
     
    //if(Simulation::getAvalonPlugin()->getRearSonarData(sonar_beam)){
		//if (_sonar_beam.connected())
		//_sonar_beam.write(sonar_beam);
    //} else {
		//sonar_beam.beam.clear();
    //}
     
     _angle_to_pinger.write(base::Angle::fromRad(Simulation::getAvalonPlugin()->getAngleToPinger()));
     
     return;
 }
// void PingerSearch::errorHook()
// {
//     PingerSearchBase::errorHook();
// }
// void PingerSearch::stopHook()
// {
//     PingerSearchBase::stopHook();
// }
// void PingerSearch::cleanupHook()
// {
//     PingerSearchBase::cleanupHook();
// }

