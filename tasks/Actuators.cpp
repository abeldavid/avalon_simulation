/* Generated from orogen/lib/orogen/templates/tasks/Task.cpp */

#include "Actuators.hpp"
#include "Simulation.h"

using namespace avalon_simulation;
using namespace base::actuators;

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
// hooks defined by Orocos::RTT. See Actuators.hpp for more detailed
// documentation about them.

// bool Actuators::configureHook()
// {
//     if (! ActuatorsBase::configureHook())
//         return false;
//     return true;
// }
bool Actuators::startHook()
{
    if (! ActuatorsBase::startHook())
        return false;

    if(!Simulation::isInitialized())
        throw std::runtime_error("Cannot start Actuators. The simulator is not running in the same process.");
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
        pwm.push_back(-command.target[2]);
        pwm.push_back(-command.target[3]);
        pwm.push_back(-command.target[5]);
        pwm.push_back(-command.target[4]*0.2);
        pwm.push_back(-command.target[1]);
        pwm.push_back(command.target[0]);
        
        assert(_foce_multiplier.get().size() == 6);
        for(int i=0;i<6;i++){
            pwm[i] *= _foce_multiplier.get()[i];
        }
        
        Simulation::getAvalonPlugin()->setTarget(pwm);
        
        
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

// void Actuators::stopHook()
// {
//     ActuatorsBase::stopHook();
// }
// void Actuators::cleanupHook()
// {
//     ActuatorsBase::cleanupHook();
// }

