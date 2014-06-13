#ifndef __SIMULATION_H__
#define __SIMULATION_H__

#include <mars/interfaces/sim/SimulatorInterface.h>
#include <avalon-plugin/AvalonPlugin.h>

class Simulation
{
    private:
        static mars::interfaces::SimulatorInterface *simulator_interface;
        static AvalonPlugin *avalon_plugin;

    public:
        Simulation();
        
        static void setSimulatorInterface(mars::interfaces::SimulatorInterface* interface)
        {simulator_interface = interface;};

        static mars::interfaces::SimulatorInterface* getSimulatorInterface()
        {return simulator_interface;};

        static void setAvalonPlugin(AvalonPlugin* plugin)
        {avalon_plugin = plugin;};

        static AvalonPlugin* getAvalonPlugin()
        {return avalon_plugin;};

        static bool isInitialized()
        {return (avalon_plugin && simulator_interface);};
};

#endif
