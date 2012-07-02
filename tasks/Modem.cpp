/* Generated from orogen/lib/orogen/templates/tasks/Task.cpp */

#include "Modem.hpp"

using namespace avalon_simulation;

Modem::Modem(std::string const& name, TaskCore::TaskState initial_state)
    : ModemBase(name, initial_state)
{
}

Modem::Modem(std::string const& name, RTT::ExecutionEngine* engine, TaskCore::TaskState initial_state)
    : ModemBase(name, engine, initial_state)
{
}

Modem::~Modem()
{
}



/// The following lines are template definitions for the various state machine
// hooks defined by Orocos::RTT. See Modem.hpp for more detailed
// documentation about them.

// bool Modem::configureHook()
// {
//     if (! ModemBase::configureHook())
//         return false;
//     return true;
// }
// bool Modem::startHook()
// {
//     if (! ModemBase::startHook())
//         return false;
//     return true;
// }
// void Modem::updateHook()
// {
//     ModemBase::updateHook();
// }
// void Modem::errorHook()
// {
//     ModemBase::errorHook();
// }
// void Modem::stopHook()
// {
//     ModemBase::stopHook();
// }
// void Modem::cleanupHook()
// {
//     ModemBase::cleanupHook();
// }

