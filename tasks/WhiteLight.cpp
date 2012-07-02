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
    Simulation::getAvalonPlugin()->switchWhiteLight();
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
    
    lastUpdate = base::Time::now();
    diff_ms = 0;

    return true;
}
void WhiteLight::updateHook()
{
    WhiteLightBase::updateHook();
    
    if(_interval_mode == 0){
      // do nothing
    } else { 
      base::Time diffTime = base::Time::now() - lastUpdate;
      diff_ms += diffTime.toMilliseconds();
      std::cout <<diff_ms << std::endl;
      
      if(_interval_mode==1) {
        if(diff_ms>=_constantInterval){
          Simulation::getAvalonPlugin()->switchWhiteLight();
          diff_ms=0;
        }
      } else {
        double random = getRandomValue(_randomInterval_min,_randomInterval_max);
        
        if(diff_ms>=random){
          Simulation::getAvalonPlugin()->switchWhiteLight();
          diff_ms=0;
        }
      }
      
      lastUpdate = base::Time::now();
    }
    
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

