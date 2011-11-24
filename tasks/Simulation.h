#ifndef __SIMULATION_H__
#define __SIMULATION_H__

#include <mars_sim/SimulatorInterface.h>
#include <avalon-plugin/AvalonPlugin.h>

class Simulation
{
    private:
        static SimulatorInterface *simulator_interface;
        static AvalonPlugin *avalon_plugin;

    public:
        Simulation();
        
        static void setSimulatorInterface(SimulatorInterface* interface)
        {simulator_interface = interface;};

        static SimulatorInterface* getSimulatorInterface()
        {return simulator_interface;};

        static void setAvalonPlugin(AvalonPlugin* plugin)
        {avalon_plugin = plugin;};

        static AvalonPlugin* getAvalonPlugin()
        {return avalon_plugin;};

        static bool isInitialized()
        {return (avalon_plugin && simulator_interface);};
};

#endif
