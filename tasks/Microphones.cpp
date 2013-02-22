/* Generated from orogen/lib/orogen/templates/tasks/Task.cpp */

#include "Microphones.hpp"
#include <simulation/MarsPlugin.hpp>
#include <mars/sim/SimNode.h>
#include <mars/sim/NodeManager.h>
#include <base/pose.h>
#include "Simulation.h"
#include <math.h>


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


    sample.sample_frequency = 44100;
    sample.timestamp = base::Time::now();
    sample.left_channel.resize(44100);
    sample.right_channel.resize(44100);
    
    return true;
    
}



void Microphones::updateHook()
{

	MicrophonesBase::updateHook();
	Eigen::Vector3d wPos = mp->getPosition();
	double  yaw = base::getYaw(mp->getOrientation());
	double c = _base_line/2;
	double a,b,c1, c2, alpha, beta;
	int diff;
	Eigen::Quaterniond quarter;
	Eigen::Vector3d relPos1, relPos2, ASVpos;

	Simulation::getAvalonPlugin()->getASVPose(ASVpos,quarter);

	alpha = M_PI - yaw;
	beta = (M_PI/2) - alpha;
	a = c * cos(beta);
	b = sqrt(pow(c,2) - pow(a,2)); 
	

	relPos1(0) = (wPos(0)-b);
	relPos1(1) = (wPos(1)-a);
	relPos1(2) = (wPos(2));

	relPos2(0) = (wPos(0)+b);
	relPos2(1) = (wPos(1)+a);
	relPos2(2) = (wPos(2));
	



	relPos1 = ASVpos - relPos1;
	relPos2 = ASVpos - relPos2;


	c1 = sqrt(pow(relPos1(0),2) + pow(relPos1(1),2));
	c2 = sqrt(pow(relPos2(0),2) + pow(relPos2(1),2));
	


	diff = (int)(44100.0 * ((c1 - c2)/_sound_velocity));
	

	if(fabs(yaw) > (M_PI/2)){
	  diff = diff*(-1);
	}

	for(int i=0; i<sample.left_channel.size(); i++){
		sample.right_channel[i] = 0;
		sample.left_channel[i] = 0;
	}

	if (diff >0.0){

		for(int i=0; i<(sample.left_channel.size()/100)-4; i+=4){
			sample.left_channel[i] = 1;
			sample.left_channel[i+2] = -1;
		}

		for(int i=diff; i<(sample.right_channel.size()/100)-4; i+=4){
			sample.right_channel[i] = 1;
			sample.right_channel[i+2] = -1;
		}
	}
	else if(diff < 0.0){

		for(int i=abs(diff); i<sample.left_channel.size()/100-4; i+=4){
			sample.left_channel[i] = 1;
			sample.left_channel[i+2] = -1;
		}

		for(int i=0; i<(sample.right_channel.size()/100)-4; i+=4){
			sample.right_channel[i] = 1;
			sample.right_channel[i+2] = -1;
		}
	}
	else{
		for(int i=0; i<(sample.right_channel.size()/100)-4; i+=4){
					sample.right_channel[i] = 1;
					sample.right_channel[i+2] = -1;

					sample.left_channel[i] = 1;
					sample.left_channel[i+2] = -1;

				}
	}


	_stereo_output.write(sample);

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

