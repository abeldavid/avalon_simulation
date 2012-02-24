/* Generated from orogen/lib/orogen/templates/tasks/Task.cpp */

#include "Task.hpp"
#include <unistd.h>
#include "Simulation.h"
#include <avalon-plugin/AvalonPlugin.h>
#include <marsusim/OPlugin.h>
#include <mars_app/GraphicsTimer.h>
#include <osgOcean/FFTOceanSurface>

using namespace avalon_simulation;

Task::Task(std::string const& name)
    : TaskBase(name),avalon(NULL)
{
    _enable_gui.set(true);
}

Task::~Task()
{
}

bool Task::setPosition(double x, double y, double z)
{
    avalon->setPosition(x,y,z);
    simulatorInterface->sceneHasChanged(true);

    return true;
}

bool Task::setOrientation(double x, double y, double z, double w)
{
  avalon->setOrientation(x,y,z,w);
  simulatorInterface->sceneHasChanged(true);

  return true;
}


bool Task::setYaw(double yaw)
{
    base::Vector3d euler(yaw, 0.0, 0.0);
    Eigen::Quaternion<double> o;
    o = Eigen::AngleAxis<double>(euler.x(), base::Vector3d::Unit(2));

    avalon->setOrientation(o.x(), o.y(), o.z(), o.w());
    simulatorInterface->sceneHasChanged(true);

    return true;
}


/// The following lines are template definitions for the various state machine
// hooks defined by Orocos::RTT. See Task.hpp for more detailed
// documentation about them.

bool Task::configureHook()
{
    int stat = putenv("LANG=en_EN.UTF-8");
    stat += putenv("LANGUAGE=en_EN.UTF-8");
    stat += putenv("LC_ALL=en_EN.UTF-8");
    stat += putenv("LC_ALL=C");
    if (stat != 0)
        std::cout << "failed to define one or more environment variables" << std::endl;

    if (! TaskBase::configureHook())
        return false;
    

    //check if the environment was sourced more than once
    int pos = _scenefile.get().rfind(":/");
    if(pos != _scenefile.get().size()-1)
        _scenefile.set(_scenefile.get().substr(pos+1));

    //test if the scene file can be accessed
    if(0!=access(_scenefile.get().c_str(),R_OK))
        throw std::runtime_error(std::string("Can not access scene file: ") + _scenefile.get());

    delete avalon;

    //During loading it seems better to stop the viz
    Mars::graphicsTimer->stop();
    Mars::graphicsTimer->runOnce();

    avalon = new AvalonPlugin(libManager, _scenefile.get(),_debug_sonar.get(),_use_osg_ocean.get());
    Simulation::setSimulatorInterface(simulatorInterface);
    libManager->addLibrary(avalon);
    
    Mars::graphicsTimer->runOnce();
osg=0;
    
    int cnt=0;
    //Try for 10 seconds to get avalon running
    while(!avalon->isRunning()){
        usleep(10000);
        if(cnt++ == 1000){
            std::cerr << "Could not get an runnning instance of avalon" << std::endl;
            return false;
        }
        
    }

    Mars::graphicsTimer->runOnce();

    if(_use_osg_ocean.get()){
        osg = new OPlugin(libManager);
        libManager->addLibrary(osg);
    }
    Simulation::setAvalonPlugin(avalon);

    //Starting viz    
    Mars::graphicsTimer->run();

    //_initial_position.get(), Eigen::Quaterniond(Eigen::AngleAxisd(_initial_yaw.get(), Eigen::Vector3d::UnitZ())));

    if(osg){
        osgOcean::FFTOceanSurface *surface = dynamic_cast<osgOcean::FFTOceanSurface*>(osg->getScene()->getOceanSurface());
        osgOcean::OceanScene *ocean = osg->getScene()->getOceanScene();
        if(!surface){
            std::cerr << "Could not get surface" << std::endl;
            return false;
        }
        
        waveScale = surface->getWaveScaleFactor();
        windSpeed = surface->getWindSpeed();
        endlessOcean = surface->isEndlessOceanEnabled();
        oceanHeight = ocean->getOceanSurfaceHeight();
        goodRays = ocean->areGodRaysEnabled();
        glare = ocean->isGlareEnabled();
        glareThreashold = ocean->getGlareThreshold();
        distortion = ocean->isDistortionEnabled();
        scattering = ocean->isUnderwaterScatteringEnabled();
        fogDensity = ocean->getUnderwaterFogDensity();
        osg::Vec4f v = ocean->getUnderwaterFogColor();
        fogColor = base::Vector4d(v[0],v[1],v[2],v[3]); 
        v = ocean->getUnderwaterDiffuse();
        diffuse = base::Vector4d(v[0],v[1],v[2],v[3]); 
        osg::Vec3f v3 = ocean->getUnderwaterAttenuation();
        attenuation = base::Vector3d(v3[0],v3[1],v3[2]);
        silt = ocean->isSiltEnabled(); 
        
        _waveScale.set(surface->getWaveScaleFactor());
        _windSpeed.set(surface->getWindSpeed());
        _endlessOcean.set(surface->isEndlessOceanEnabled());
        _oceanHeight.set(ocean->getOceanSurfaceHeight());
        _goodRays.set(ocean->areGodRaysEnabled());
        _glare.set(ocean->isGlareEnabled());
        _glareThreashold.set(ocean->getGlareThreshold());
        _distortion.set(ocean->isDistortionEnabled());
        _scattering.set(ocean->isUnderwaterScatteringEnabled());
        _fogDensity.set(ocean->getUnderwaterFogDensity());
        v = ocean->getUnderwaterFogColor();
        _fogColor.set(base::Vector4d(v[0],v[1],v[2],v[3]));
        v = ocean->getUnderwaterDiffuse();
        _diffuse.set(base::Vector4d(v[0],v[1],v[2],v[3]));
        v3 = ocean->getUnderwaterAttenuation();
        _attenuation.set(base::Vector3d(v3[0],v3[1],v3[2]));
        _silt.set(ocean->isSiltEnabled());
    }
    setPosition(_initial_x.get(), 
            _initial_y.get(), 
            _initial_z.get());
    setYaw(_initial_yaw.get());

    return true;
}
bool Task::startHook()
{
    if (! TaskBase::startHook())
        return false;
    return true;
}
void Task::updateHook()
{
    TaskBase::updateHook();

    if (!simulatorInterface->isSimRunning())
        return;

    checkAndApplyConfigChange();

    Eigen::Vector3d position;
    Eigen::Quaterniond orientation;

    avalon->getPose(position, orientation);

    _auv_position.write(position);
}
// void Task::errorHook()
// {
//     TaskBase::errorHook();
// }
// void Task::stopHook()
// {
//     TaskBase::stopHook();
// }
// void Task::cleanupHook()
// {
//     TaskBase::cleanupHook();
// }


void Task::checkAndApplyConfigChange(){
    if(osg){
        osgOcean::FFTOceanSurface *surface = dynamic_cast<osgOcean::FFTOceanSurface*>(osg->getScene()->getOceanSurface());
        osgOcean::OceanScene *ocean = osg->getScene()->getOceanScene();
        if(!surface){
            std::cerr << "Could not get surface" << std::endl;
        }
        
        static const bool dirty = true;

        if(_waveScale.get() != waveScale){
            waveScale = _waveScale.get();
            surface->setWaveScaleFactor(waveScale);
            printf("Wave scale changes\n");
        }
        if(_windSpeed.get() != windSpeed){
            windSpeed = _windSpeed.get();
            surface->setWindSpeed(windSpeed);
        }
        if(_endlessOcean.get() != endlessOcean){
            endlessOcean = _endlessOcean.get();
            surface->enableEndlessOcean(endlessOcean,dirty); 
        }
        if(_oceanHeight.get() != oceanHeight){
            oceanHeight = _oceanHeight.get();
            ocean->setOceanSurfaceHeight(oceanHeight);
        }
        if(_goodRays.get() != goodRays){
            goodRays = _goodRays.get();
            ocean->enableGodRays(goodRays);
        }
        if(_silt.get() != silt){
            silt = _silt.get();
            ocean->enableSilt(silt);
        }
        if(_glare.get() != glare){
            glare = _glare.get();
            ocean->enableGlare(glare);
        }
        if(_glareThreashold.get() != glareThreashold){
            glareThreashold = _glareThreashold.get();
            ocean->setGlareThreshold(glareThreashold);
        }
        if(_distortion.get() != distortion){
            distortion = _distortion.get();
            ocean->enableDistortion(distortion);
        }
        if(_scattering.get() != scattering){
            scattering = _scattering.get();
            ocean->enableUnderwaterScattering(scattering);
        }

        if(_fogDensity.get() != fogDensity || _fogColor.get() != fogColor){
            fogDensity = _fogDensity.get();
            fogColor = _fogColor.get();
            ocean->setUnderwaterFog(fogDensity,osg::Vec4f(fogColor[0],fogColor[1],fogColor[2],fogColor[3]));
        }
        if(_diffuse.get() != diffuse){
            diffuse = _diffuse.get();
            ocean->setUnderwaterDiffuse(osg::Vec4f(diffuse[0],diffuse[1],diffuse[2],diffuse[3]));
        }
        if(_attenuation.get() != attenuation){
            attenuation = _attenuation.get();
            ocean->setUnderwaterAttenuation(osg::Vec3f(attenuation[0],attenuation[1],attenuation[2]));
        }


        
        //_waveScale.set(surface->getWaveScaleFactor());
        //_windSpeed.set(surface->getWindSpeed());
        //_endlessOcean.set(surface->isEndlessOceanEnabled());
        //_oceanHeight.set(ocean->getOceanSurfaceHeight());
        //_goodRays.set(ocean->areGodRaysEnabled());
        //_glare.set(ocean->isGlareEnabled());
        //_glareThreashold.set(ocean->getGlareThreshold());
        //_distortion.set(ocean->isDistortionEnabled());
        //_scattering.set(ocean->isUnderwaterScatteringEnabled());
        //_fogDensity.set(ocean->getUnderwaterFogDensity());
        //osg::Vec4f v = ocean->getUnderwaterFogColor();
        //_fogColor.set(base::Vector4d(v[0],v[1],v[2],v[3]));
        //v = ocean->getUnderwaterDiffuse();
        //_diffuse.set(base::Vector4d(v[0],v[1],v[2],v[3]));
        //osg::Vec3f v3 = ocean->getUnderwaterAttenuation();
        //_attenuation.set(base::Vector3d(v3[0],v3[1],v3[2]));
    }

}
