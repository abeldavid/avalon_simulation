/* Generated from orogen/lib/orogen/templates/tasks/Task.cpp */

#include "ASVActuators.hpp"
#include "Simulation.h"

using namespace base::actuators;
using namespace avalon_simulation;

ASVActuators::ASVActuators(std::string const& name, TaskCore::TaskState initial_state)
    : ASVActuatorsBase(name, initial_state)
{
}

ASVActuators::ASVActuators(std::string const& name, RTT::ExecutionEngine* engine, TaskCore::TaskState initial_state)
    : ASVActuatorsBase(name, engine, initial_state)
{
}

ASVActuators::~ASVActuators()
{
}



/// The following lines are template definitions for the various state machine
// hooks defined by Orocos::RTT. See ASVActuators.hpp for more detailed
// documentation about them.

// bool ASVActuators::configureHook()
// {
//     if (! ASVActuatorsBase::configureHook())
//         return false;
//     return true;
// }

bool ASVActuators::startHook()
{
    if (! ASVActuatorsBase::startHook())
        return false;
    if(!Simulation::isInitialized())
        throw std::runtime_error("Cannot start Actuators. The simulator is not running in the same process.");
    return true;
}
void ASVActuators::updateHook()
{
    ASVActuatorsBase::updateHook();
    
    base::actuators::Command command;
    if (_command.readNewest(command) == RTT::NewData)
    {
        //we have to check it here otherwise the port
        //might be no longer triggeres
        if (!Simulation::getSimulatorInterface()->isSimRunning()) 
            return;

        //check we have six actuator commands
        if(command.mode.size() != 4 ||command.target.size() != 4)
            throw std::runtime_error("Simulation need a target and mode size of 4");

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
        pwm.push_back(command.target[0]);
        pwm.push_back(command.target[1]);
        pwm.push_back(command.target[2]);
        pwm.push_back(command.target[3]);
        Simulation::getAvalonPlugin()->setASVTarget(pwm);
        
        
        // write actuator status
        base::actuators::Status status;
        status.time = base::Time::now();
        status.index = 0; //TODO: ?
        status.resize(4);
        status.states[0].pwm = -command.target[0];
        status.states[1].pwm = -command.target[1];
        status.states[2].pwm = -command.target[2];
        status.states[3].pwm = -command.target[3];
        // TODO: Motorstate: current,position,positionExtern
        _status.write(status);
    }
    
    base::samples::RigidBodyState pose;
    Eigen::Quaterniond q;
    Eigen::Vector3d p;
    Simulation::getAvalonPlugin()->getASVPose(p,q);

    Eigen::Vector3d lin_vel;
    Eigen::Vector3d ang_vel;

    Simulation::getAvalonPlugin()->getASVVelocities(lin_vel, ang_vel);

    lin_vel = q.inverse() * lin_vel;
    ang_vel = q.inverse() * ang_vel;

    pose.position = p;
    pose.time = base::Time::now();
    pose.orientation = q;
    pose.velocity = lin_vel;
    pose.angular_velocity = ang_vel;

    _pose_samples.write(pose);

}
void ASVActuators::errorHook()
{
    ASVActuatorsBase::errorHook();
}

// void ASVActuators::stopHook()
// {
//     ASVActuatorsBase::stopHook();
// }
// void ASVActuators::cleanupHook()
// {
//     ASVActuatorsBase::cleanupHook();
// }

