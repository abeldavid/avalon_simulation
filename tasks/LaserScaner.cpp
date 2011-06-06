/* Generated from orogen/lib/orogen/templates/tasks/Task.cpp */

#include "LaserScaner.hpp"

using namespace avalon_simulation;

LaserScaner::LaserScaner(std::string const& name, TaskCore::TaskState initial_state)
    : LaserScanerBase(name, initial_state)
{
}

LaserScaner::LaserScaner(std::string const& name, RTT::ExecutionEngine* engine, TaskCore::TaskState initial_state)
    : LaserScanerBase(name, engine, initial_state)
{
}

LaserScaner::~LaserScaner()
{
}



/// The following lines are template definitions for the various state machine
// hooks defined by Orocos::RTT. See LaserScaner.hpp for more detailed
// documentation about them.

// bool LaserScaner::configureHook()
// {
//     if (! LaserScanerBase::configureHook())
//         return false;
//     return true;
// }
// bool LaserScaner::startHook()
// {
//     if (! LaserScanerBase::startHook())
//         return false;
//     return true;
// }
// void LaserScaner::updateHook()
// {
//     LaserScanerBase::updateHook();
// }
// void LaserScaner::errorHook()
// {
//     LaserScanerBase::errorHook();
// }
// void LaserScaner::stopHook()
// {
//     LaserScanerBase::stopHook();
// }
// void LaserScaner::cleanupHook()
// {
//     LaserScanerBase::cleanupHook();
// }

