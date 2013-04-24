/* Generated from orogen/lib/orogen/templates/tasks/Task.cpp */

#include "TestVehicleActuators.hpp"
#include "Simulation.h"

using namespace base::actuators;
using namespace avalon_simulation;

TestVehicleActuators::TestVehicleActuators(std::string const& name, TaskCore::TaskState initial_state)
    : TestVehicleActuatorsBase(name, initial_state)
{
}

TestVehicleActuators::TestVehicleActuators(std::string const& name, RTT::ExecutionEngine* engine, TaskCore::TaskState initial_state)
    : TestVehicleActuatorsBase(name, engine, initial_state)
{
}

TestVehicleActuators::~TestVehicleActuators()
{
}



/// The following lines are template definitions for the various state machine
// hooks defined by Orocos::RTT. See TestVehicleActuators.hpp for more detailed
// documentation about them.

// bool TestVehicleActuators::configureHook()
// {
//     if (! TestVehicleActuatorsBase::configureHook())
//         return false;
//     return true;
// }

bool TestVehicleActuators::startHook()
{
    if (! TestVehicleActuatorsBase::startHook())
        return false;
    if(!Simulation::isInitialized())
        throw std::runtime_error("Cannot start Actuators. The simulator is not running in the same process.");
    return true;
}
void TestVehicleActuators::updateHook()
{
    TestVehicleActuatorsBase::updateHook();

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
        pwm.push_back(command.target[0]);
        pwm.push_back(command.target[1]);
        pwm.push_back(command.target[2]);
        pwm.push_back(command.target[3]);
        pwm.push_back(command.target[4]);
        pwm.push_back(command.target[5]);
        Simulation::getAvalonPlugin()->setTestVehicleTarget(pwm);


        // write actuator status
        base::actuators::Status status;
        status.time = base::Time::now();
        status.index = 0; //TODO: ?
        status.resize(6);
        status.states[0].pwm = -command.target[0];
        status.states[1].pwm = -command.target[1];
        status.states[2].pwm = -command.target[2];
        status.states[3].pwm = -command.target[3];
        status.states[4].pwm = -command.target[4];
        status.states[5].pwm = -command.target[5];
        // TODO: Motorstate: current,position,positionExtern
        _status.write(status);
    }

    base::samples::RigidBodyState pose;
    Eigen::Quaterniond q;
    Eigen::Vector3d p;
    Simulation::getAvalonPlugin()->getTestVehiclePose(p,q);

    Eigen::Vector3d lin_vel;
    Eigen::Vector3d ang_vel;

    Simulation::getAvalonPlugin()->getTestVehicleVelocities(lin_vel, ang_vel);

    lin_vel = q.inverse() * lin_vel;
    ang_vel = q.inverse() * ang_vel;

    pose.position = p;
    pose.time = base::Time::now();
    pose.orientation = q;
    pose.velocity = lin_vel;
    pose.angular_velocity = ang_vel;

    _pose_samples.write(pose);

}
void TestVehicleActuators::errorHook()
{
    TestVehicleActuatorsBase::errorHook();
}

// void TestVehicleActuators::stopHook()
// {
//     TestVehicleActuatorsBase::stopHook();
// }
// void TestVehicleActuators::cleanupHook()
// {
//     TestVehicleActuatorsBase::cleanupHook();
// }

