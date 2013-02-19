/* Generated from orogen/lib/orogen/templates/tasks/Task.cpp */

#include "Microphones.hpp"
#include <simulation/MarsPlugin.hpp>
#include <mars/sim/SimNode.h>
#include <mars/sim/NodeManager.h>

using namespace avalon_simulation;

struct avalon_simulation::MicrophonePlugin: public simulation::MarsPlugin
{
    int node_id;
    Eigen::Quaterniond orientation;
    Eigen::Vector3d position;

    MicrophonePlugin()
        : node_id(0)
    {}

    void update( double time )
    {
        //mars::sim::SimNode *node = control->nodes->getSimNode(node_id);
        orientation = control->nodes->getRotation(node_id);
        position = control->nodes->getPosition(node_id);
    }

    void setMotorName( const std::string& name )
    {
        node_id = control->nodes->getID( name );
        if( !node_id)
            throw std::runtime_error("There is no Node by the name of " + name + " in the scene");
    }

    Eigen::Vector3d getPosition(){
        return position;
    }
    
    Eigen::Quaterniond getOrientation(){
        return orientation;
    }
};


Microphones::Microphones(std::string const& name, TaskCore::TaskState initial_state)
    : MicrophonesBase(name, initial_state)
{
}

Microphones::Microphones(std::string const& name, RTT::ExecutionEngine* engine, TaskCore::TaskState initial_state)
    : MicrophonesBase(name, engine, initial_state)
{
}

Microphones::~Microphones()
{
}



/// The following lines are template definitions for the various state machine
// hooks defined by Orocos::RTT. See Microphones.hpp for more detailed
// documentation about them.




bool Microphones::configureHook()
{
    
    if (! RTT::TaskContext::configureHook())
        return false;
    

    

    
    return true;
    
}



bool Microphones::startHook()
{
    
    if (! RTT::TaskContext::startHook())
        return false;
    
    mp= new MicrophonePlugin();
    mp->setMotorName( _node_name.value() );

    return true;
    
}



void Microphones::updateHook()
{
    
    RTT::TaskContext::updateHook();
}



void Microphones::errorHook()
{
    
    RTT::TaskContext::errorHook();
}



void Microphones::stopHook()
{
    delete mp;
    RTT::TaskContext::stopHook();
}



void Microphones::cleanupHook()
{
    
    RTT::TaskContext::cleanupHook();
}

