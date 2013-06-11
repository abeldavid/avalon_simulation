/* Generated from orogen/lib/orogen/templates/tasks/Task.cpp */

#include "Microphones.hpp"
//#include <mars/sim/SimNode.h>
//#include <mars/sim/NodeManager.h>
#include <mars/interfaces/sim/NodeManagerInterface.h>
#include <base/pose.h>
#include "Simulation.h"
#include <math.h>
#include <boost/random.hpp>


using namespace avalon_simulation;



Microphones::Microphones(std::string const& name)
: MicrophonesBase(name)
{
}

Microphones::Microphones(std::string const& name, RTT::ExecutionEngine* engine)
: MicrophonesBase(name, engine)
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

	node_id =  0; //control->nodes->getID(_node_name); //TODO
	sample.sample_frequency = _sample_rate;
	sample.timestamp = base::Time::now();
	sample.left_channel.resize((int) _sample_rate);
	sample.right_channel.resize((int) _sample_rate);

	pingSample.sample_frequency = _sample_rate;
	pingSample.timestamp = base::Time::now();
	pingSample.left_channel.resize((int) ((_sample_rate / 100)+200));
	pingSample.right_channel.resize((int) ((_sample_rate / 100)+200) );

	return true;

}



void Microphones::updateHook()
{

	MicrophonesBase::updateHook();
	Eigen::Vector3d wPos = position;
	Eigen::Quaterniond  orient = orientation;
	double base_line = _base_line/2;
	int diff;
	int length_qoutient = 1000 / _ping_length;
	Eigen::Quaterniond rotation = base::Quaterniond(Eigen::AngleAxisd(-(base::getYaw(orient)),Eigen::Vector3d::UnitZ()));
	Eigen::Vector3d relPos1, relPos2, ASVpos;

	ASVpos = Simulation::getAvalonPlugin()->getPosition("asv");



	//Errechnung der Positionen der Microfone anhand der Rotation
	relPos1(0) = 0+base_line;
	relPos1(1) = 0;
	relPos1(2) = 0;

	relPos2(0) = 0-base_line;
	relPos2(1) = 0;
	relPos2(2) = 0;

	relPos1 = (rotation.conjugate()*relPos1) + wPos;
	relPos2 = (rotation.conjugate()*relPos2) + wPos;


	//relative Position
	relPos1 = ASVpos - relPos1;
	relPos2 = ASVpos - relPos2;


	//Errechnung der Indexverschiebung
	diff = (int)(_sample_rate * ((relPos2.norm() - relPos1.norm())/_sound_velocity));



	for(unsigned i=0; i<sample.left_channel.size(); i++){
		sample.right_channel[i] = 0;
		sample.left_channel[i] =  0;
	}

	double zeroCrossing = (_sample_rate / _pinger_frequency)/2; // gibt an wie viele indexe für 2 zerocrossings
															//	(eine Schwingung) benötigt werden

	//Signalerstellung

	sample.left_channel.resize((int) _sample_rate);
	sample.right_channel.resize((int) _sample_rate);

	pingSample.left_channel.resize((int) ((_sample_rate / length_qoutient)+200));
	pingSample.right_channel.resize((int) ((_sample_rate / length_qoutient)+200) );



	if (diff >0.0){

		for(unsigned i=0; i<(sample.left_channel.size()/length_qoutient); i++){
			sample.left_channel[i] = sin(M_PI*i/zeroCrossing)*_amplitude;
		}

		for(unsigned i=diff; i<(sample.right_channel.size()/length_qoutient)+diff; i++){
			sample.right_channel[i] = sin(M_PI*i/zeroCrossing)*_amplitude;
		}
	}
	else if(diff < 0.0){

		for(unsigned i=abs(diff); i<sample.left_channel.size()/length_qoutient+abs(diff); i++){
			sample.left_channel[i] = sin(M_PI*i/zeroCrossing)*_amplitude;
		}

		for(unsigned i=0; i<(sample.right_channel.size()/length_qoutient); i++){
			sample.right_channel[i] = sin(M_PI*i/zeroCrossing)*_amplitude;
		}
	}
	else{
		for(unsigned i=0; i<(sample.right_channel.size()/length_qoutient); i++){
			sample.right_channel[i] = sin(M_PI*i/zeroCrossing)*_amplitude;

			sample.left_channel[i] = sin(M_PI*i/zeroCrossing)*_amplitude;
		}
	}



	//white_noise Generator
	if(_white_noise){


		boost::mt11213b mt; // mersenne twister als zufallszahlgenerator
		boost::normal_distribution<float> normalverteilung((-1) *_noise_amplitude, _noise_amplitude); //normalverteilung

		boost::variate_generator<boost::mt11213b&, boost::normal_distribution<float> > //Generator
		get_random(mt, normalverteilung);

		for(unsigned i=0; i<sample.left_channel.size(); i++){
			sample.left_channel[i] += get_random();
			sample.right_channel[i] += get_random();
		}
	}



	//Ping-Ausschnitt



	for(unsigned i=0; i<pingSample.left_channel.size(); i++){
		pingSample.left_channel[i] = sample.left_channel[i];
		pingSample.right_channel[i] = sample.right_channel[i];
	}


	_ping_output.write(pingSample);

	_stereo_output.write(sample);

	RTT::TaskContext::updateHook();
}


void Microphones::update( double time )
{
	//mars::sim::SimNode *node = control->nodes->getSimNode(node_id);
	orientation = control->nodes->getRotation(node_id);
	position = control->nodes->getPosition(node_id);
}




void Microphones::errorHook()
{


	RTT::TaskContext::errorHook();
}



void Microphones::stopHook()
{
	RTT::TaskContext::stopHook();
}



void Microphones::cleanupHook()
{

    RTT::TaskContext::cleanupHook();
}

