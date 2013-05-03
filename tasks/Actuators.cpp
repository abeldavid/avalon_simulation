/* Generated from orogen/lib/orogen/templates/tasks/Task.cpp */

#include "Actuators.hpp"
#include "Simulation.h"
#include <mars/interfaces/sim/NodeManagerInterface.h>

using namespace avalon_simulation;
using namespace base::actuators;
using namespace simulation;
using namespace mars::utils;



namespace simulation {
	ThrusterPlugin::ThrusterPlugin(std::string node_name, int amountOfActuators,
					std::vector <double> max_thruster_force,
					std::vector <mars::utils::Vector> thruster_pos,
					std::vector <mars::utils::Vector> thruster_dir)
		: amountOfActuators(amountOfActuators), max_thruster_force(max_thruster_force), thruster_pos(thruster_pos), thruster_dir(thruster_dir)
	{
		vehicle_id = control->nodes->getID(node_name);
		if( !vehicle_id )
			throw std::runtime_error("There is no node by the name of " + node_name + " in the scene");

		std::string groupName, dataName;
		control->nodes->getDataBrokerNames(vehicle_id, &groupName, &dataName);
		control->dataBroker->registerTimedReceiver(this, groupName, dataName, "mars_sim/simTimer", RATE,vehicle_id);
		RATE = 10;
		for (unsigned int i=0; i<amountOfActuators; i++) {
			thruster_force[i] = 0.0;
		}
	}

	bool ThrusterPlugin::getPose(Eigen::Vector3d &pos, Eigen::Quaterniond &orientation){
		pthread_mutex_lock(node_update_mutex);
		mars::utils::Vector vehicle_pos = control->nodes->getPosition(vehicle_id);
		mars::utils::Quaternion vehicle_rot = control->nodes->getRotation(vehicle_id);
		pos = Eigen::Vector3d(vehicle_pos.x(),vehicle_pos.y(),vehicle_pos.z());
		orientation = Eigen::Quaterniond(vehicle_rot.w(),vehicle_rot.x(),vehicle_rot.y(),vehicle_rot.z());
		pthread_mutex_unlock(node_update_mutex);
		return true;
	}

	bool ThrusterPlugin::getVelocities(Eigen::Vector3d &lin_vel, Eigen::Vector3d &ang_vel){
		pthread_mutex_lock(node_update_mutex);
		mars::utils::Vector vehicle_lin_vel = control->nodes->getLinearVelocity(vehicle_id);
		mars::utils::Vector vehicle_ang_vel = control->nodes->getAngularVelocity(vehicle_id);
		lin_vel = Eigen::Vector3d(vehicle_lin_vel.x(),vehicle_lin_vel.y(),vehicle_lin_vel.z());
		ang_vel = Eigen::Vector3d(vehicle_ang_vel.x(),vehicle_ang_vel.y(),vehicle_ang_vel.z());
		pthread_mutex_unlock(node_update_mutex);
		return true;
	}

	void ThrusterPlugin::setTarget(const std::vector<double> &target){
		if(target.size() != amountOfActuators) {
			char buffer[50];
			sprintf(buffer, "Object has %d motors!!!", amountOfActuators);
			throw std::runtime_error(buffer);
		}

		pthread_mutex_lock(node_update_mutex);
		for(unsigned int i=0;i<amountOfActuators;i++)
			thruster_force[i] = target[i];
		pthread_mutex_unlock(node_update_mutex);
	}

	void ThrusterPlugin::update(double time) {
		mars::utils::Quaternion vehicle_rot = control->nodes->getRotation(vehicle_id);
		mars::utils::Vector tmp1, tmp2;
		for(unsigned int i=0; i<amountOfActuators;++i) {
			tmp1 = vehicle_rot * thruster_pos[i];
			tmp1 += control->nodes->getPosition(vehicle_id);
			tmp2 = vehicle_rot *thruster_dir[i];
			tmp2 *= std::max(-max_thruster_force[i],std::min(max_thruster_force[i],thruster_force[i]*max_thruster_force[i]));
			control->nodes->applyForce(vehicle_id, tmp2, tmp1);
		}
	}
}



Actuators::Actuators(std::string const& name, TaskCore::TaskState initial_state)
    : ActuatorsBase(name, initial_state)
{
    std::vector<double> f;
    for(int i=0;i<6;i++) f.push_back(1.0);
    _foce_multiplier.set(f);
}


Actuators::Actuators(std::string const& name, RTT::ExecutionEngine* engine, TaskCore::TaskState initial_state)
    : ActuatorsBase(name, engine, initial_state)
{
    std::vector<double> f;
    for(int i=0;i<6;i++) f.push_back(1.0);
    _foce_multiplier.set(f);
}

Actuators::~Actuators()
{
}


/// The following lines are template definitions for the various state machine
bool Actuators::startHook()
{
    if (! ActuatorsBase::startHook())
        return false;

    if(!Simulation::isInitialized())
        throw std::runtime_error("Cannot start Actuators. The simulator is not running in the same process.");

	std::string node_name;
	int amount_of_actuators;
	std::vector <double> maximum_thruster_force;
	std::vector <mars::utils::Vector> thruster_position;
	std::vector <mars::utils::Vector> thruster_direction;

	node_name = _node_name.get();
	amount_of_actuators = _amount_of_actuators.get();
	maximum_thruster_force = _maximum_thruster_force.get();
	for (int i=0; i<amount_of_actuators; i++) {
		base::Vector3d thruster_pos = _thruster_position.get()[i];
		thruster_position[i].x() = thruster_pos(0,0);
		thruster_position[i].y() = thruster_pos(1,0);
		thruster_position[i].z() = thruster_pos(2,0);
		thruster_direction[i].x() = _thruster_direction.get()[i](0,0);
		thruster_direction[i].y() = _thruster_direction.get()[i](1,0);
		thruster_direction[i].z() = _thruster_direction.get()[i](2,0);
	}

	thruster_plugin = new simulation::ThrusterPlugin(node_name, amount_of_actuators, maximum_thruster_force, thruster_position, thruster_direction);

    return true;
}

void Actuators::updateHook()
{
    ActuatorsBase::updateHook();

    base::actuators::Command command;
    if (_command.readNewest(command) == RTT::NewData)
    {
        //we have to check it here otherwise the port
        //might be no longer triggeres
        if (!Simulation::getSimulatorInterface()->isSimRunning())
            return;

        //check we have six actuator commands
        if(command.mode.size() != 6 ||command.target.size() != 6)
            throw std::runtime_error("Simulation need a target and mode size of 6");

        //check that the drive mode is pwm
        std::vector<DRIVE_MODE>::iterator iter = command.mode.begin();
        for(;iter != command.mode.end();++iter)
            if(*iter != DM_PWM)
                throw std::runtime_error("Simulation does only support DM_PWM");

        //TODO
        //for now we are copping it because we do not want to change
        //the mapping for the simulation model
        //
        std::vector<double> pwm;
        for (unsigned int i=0; i<command.target.size(); i++) {
			pwm.push_back(command.target[i]);
        }

        assert(_foce_multiplier.get().size() == 6);
        for(unsigned int i=0;i<_foce_multiplier.get().size();i++){
            pwm[i] *= _foce_multiplier.get()[i];
        }

        thruster_plugin->setTarget(pwm);
        thruster_plugin->update(0.0);


        // write actuator status
        base::actuators::Status status;
        status.time = base::Time::now();
        status.index = 0; //TODO: ?
        status.resize(6);
        status.states[0].pwm = -command.target[2];
        status.states[1].pwm = -command.target[3];
        status.states[2].pwm = -command.target[5];
        status.states[3].pwm = -command.target[4]*0.2;
        status.states[4].pwm = -command.target[1];
        status.states[5].pwm = -command.target[0];
        // TODO: Motorstate: current,position,positionExtern
        _status.write(status);
    }
}

void Actuators::errorHook()
{
    ActuatorsBase::errorHook();
}
