/* Generated from orogen/lib/orogen/templates/tasks/LineLaser.cpp */

#include "LineLaser.hpp"
#include <unistd.h>
#include <mars/interfaces/sim/NodeManagerInterface.h>
#include <mars/interfaces/graphics/GraphicsManagerInterface.h>
#include <mars/interfaces/sim/MotorManagerInterface.h>
#include "Simulation.h"
#include <simulation/MarsPlugin.hpp>

using namespace avalon_simulation;

LineLaser::LineLaser(std::string const& name)
    : LineLaserBase(name)
{
    _laserPos.set(base::Vector3d(0.0,0.0,-0.1));
    _laserColor.set(base::Vector3d(0.0, 1.0, 0.0));  
  
}

LineLaser::LineLaser(std::string const& name, RTT::ExecutionEngine* engine)
    : LineLaserBase(name, engine)
{
  
    _laserPos.set(base::Vector3d(0.0,0.0,-0.1));
    _laserColor.set(base::Vector3d(1.0, 0.0, 0.0));  
  
}


LineLaser::~LineLaser()
{
}


/// The following lines are template definitions for the various state machine
// hooks defined by Orocos::RTT. See LineLaser.hpp for more detailed
// documentation about them.

bool LineLaser::configureHook()
{
    if (! LineLaserBase::configureHook())
        return false;
    return true;
}
bool LineLaser::startHook()
{
    if (! LineLaserBase::startHook())
        return false;    
    
    return true;
}
void LineLaser::updateHook()
{
    LineLaserBase::updateHook();

}
// void LineLaser::errorHook()
// {
//     LineLaserBase::errorHook();
// }
// void LineLaser::stopHook()
// {
//     LineLaserBase::stopHook();
// }
// void LineLaser::cleanupHook()
// {
//     LineLaserBase::cleanupHook();
// }

void LineLaser::update(double time ){

    if(control && control->sim->isSimRunning()) {
        // get positions
	mars::utils::Vector auv_pos = control->nodes->getPosition(control->nodes->getID(_nodeName));
	mars::utils::Quaternion auv_rot = control->nodes->getRotation(control->nodes->getID(_nodeName));
	
	if(_laserActive){
	
	  //Set Line-laser
	  control->graphics->setExperimentalLineLaser(auv_pos + (auv_rot * _laserPos.get()),
						      auv_rot * mars::utils::Vector(1.0 * std::cos(_frontAngle),0.0 , 1.0 * std::sin(_frontAngle)),
						      _laserColor.get(),
						      auv_rot * mars::utils::Vector(1.0 * std::sin(_frontAngle),0.0, -1.0 * std::cos(_frontAngle) ),
						      _openingAngle);
	}
    }
}