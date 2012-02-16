/* Generated from orogen/lib/orogen/templates/tasks/Task.cpp */

#include "Task.hpp"
#include <unistd.h>
#include "Simulation.h"
#include <avalon-plugin/AvalonPlugin.h>
#include <marsusim/OPlugin.h>
#include <mars_app/GraphicsTimer.h>

using namespace avalon_simulation;

Task::Task(std::string const& name)
    : TaskBase(name),avalon(NULL)
{
    _enable_gui.set(true);
}

Task::~Task()
{
}

bool Task::setPosition(double x, double y, double z)
{
    avalon->setPosition(x,y,z);
    simulatorInterface->sceneHasChanged(true);

    return true;
}

bool Task::setOrientation(double x, double y, double z, double w)
{
  avalon->setOrientation(x,y,z,w);
  simulatorInterface->sceneHasChanged(true);

  return true;
}


bool Task::setYaw(double yaw)
{
    base::Vector3d euler(yaw, 0.0, 0.0);
    Eigen::Quaternion<double> o;
    o = Eigen::AngleAxis<double>(euler.x(), base::Vector3d::Unit(2));

    avalon->setOrientation(o.x(), o.y(), o.z(), o.w());
    simulatorInterface->sceneHasChanged(true);

    return true;
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

    delete avalon;

    //During loading it seems better to stop the viz
    Mars::graphicsTimer->stop();
    Mars::graphicsTimer->runOnce();

    avalon = new AvalonPlugin(libManager, _scenefile.get(),_debug_sonar.get(),_use_osg_ocean.get());
    Simulation::setSimulatorInterface(simulatorInterface);
    libManager->addLibrary(avalon);
    
    Mars::graphicsTimer->runOnce();

    
    int cnt=0;
    //Try for 10 seconds to get avalon running
    while(!avalon->isRunning()){
        usleep(10000);
        if(cnt++ == 1000){
            std::cerr << "Could not get an runnning instance of avalon" << std::endl;
            return false;
        }
        
    }

    Mars::graphicsTimer->runOnce();

    if(_use_osg_ocean.get()){
        osg = new OPlugin(libManager);
        libManager->addLibrary(osg);
    }
    Simulation::setAvalonPlugin(avalon);

    //Starting viz    
    Mars::graphicsTimer->run();

    //_initial_position.get(), Eigen::Quaterniond(Eigen::AngleAxisd(_initial_yaw.get(), Eigen::Vector3d::UnitZ())));
    //
    setPosition(_initial_x.get(), 
            _initial_y.get(), 
            _initial_z.get());
    setYaw(_initial_yaw.get());

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

    if (!simulatorInterface->isSimRunning())
        return;

    Eigen::Vector3d position;
    Eigen::Quaterniond orientation;

    avalon->getPose(position, orientation);

    _auv_position.write(position);
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

