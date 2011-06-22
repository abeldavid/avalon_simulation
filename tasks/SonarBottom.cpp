/* Generated from orogen/lib/orogen/templates/tasks/Task.cpp */

#include "SonarBottom.hpp"

using namespace avalon_simulation;

SonarBottom::SonarBottom(std::string const& name, TaskCore::TaskState initial_state)
    : SonarBottomBase(name, initial_state)
{
}

SonarBottom::SonarBottom(std::string const& name, RTT::ExecutionEngine* engine, TaskCore::TaskState initial_state)
    : SonarBottomBase(name, engine, initial_state)
{
}

SonarBottom::~SonarBottom()
{
}

bool SonarBottom::startHook()
{
    if (!SonarBottomBase::startHook())
        return false;

    if(!Simulation::isInitialized())
        throw std::runtime_error("Cannot get sonar data. The simulator is not running in the same process.");
    
    return true;
}

void SonarBottom::updateHook()
{
  SonarBottomBase::updateHook();
  
    if(Simulation::getAvalonPlugin()->getBottomSonarData(&sonarscan)){
      
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