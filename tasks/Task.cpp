/* Generated from orogen/lib/orogen/templates/tasks/Task.cpp */

#include "Task.hpp"
#include <unistd.h>
#include "Simulation.h"

using namespace avalon_simulation;

Task::Task(std::string const& name)
    : TaskBase(name),avalon(NULL)
{
}

Task::~Task()
{
}

/// The following lines are template definitions for the various state machine
// hooks defined by Orocos::RTT. See Task.hpp for more detailed
// documentation about them.

bool Task::configureHook()
{
    int stat = putenv("LANG=en_EN.UTF-8");
    stat += putenv("LANGUAGE=en_EN.UTF-8");
    stat += putenv("LC_ALL=en_EN.UTF-8");
    stat += putenv("LC_ALL=C");
    if (stat != 0)
        std::cout << "failed to define one or more environment variables" << std::endl; 
	
    if (! TaskBase::configureHook())
        return false;

    //check if the environment was sourced more than once
    int pos = _scenefile.get().rfind(":/");
    if(pos != _scenefile.get().size()-1)
        _scenefile.set(_scenefile.get().substr(pos+1));
    
    //test if the scene file can be accessed
    if(0!=access(_scenefile.get().c_str(),R_OK))
        throw std::runtime_error(std::string("Can not access scene file: ") + _scenefile.get());

     // Loading the robot itself
     if(!avalon)
        avalon = new AvalonPlugin(simulatorInterface->getControlCenter(), _scenefile.get(), _with_manipulator_gui.get());
     pluginStruct avalon_plugin;
     avalon_plugin.name = "AvalonPlugin";
     avalon_plugin.p_interface = avalon;
     avalon_plugin.p_destroy = NULL;
     simulatorInterface->addPlugin(avalon_plugin);

     Simulation::setSimulatorInterface(simulatorInterface);
     Simulation::setAvalonPlugin(avalon);

    return true;
}
bool Task::startHook()
{
    if (! TaskBase::startHook())
        return false;
    return true;
}
void Task::updateHook()
{
    TaskBase::updateHook();

    if (!simulatorInterface->isRunning()) 
        return;

}
// void Task::errorHook()
// {
//     TaskBase::errorHook();
// }
// void Task::stopHook()
// {
//     TaskBase::stopHook();
// }
// void Task::cleanupHook()
// {
//     TaskBase::cleanupHook();
// }

