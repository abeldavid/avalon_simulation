/* Generated from orogen/lib/orogen/templates/tasks/Task.hpp */

#ifndef AVALON_SIMULATION_ACTUATORS_TASK_HPP
#define AVALON_SIMULATION_ACTUATORS_TASK_HPP

#include "simulation/MarsPlugin.hpp"
#include "avalon_simulation/ActuatorsBase.hpp"

namespace simulation {
    class ThrusterPlugin : public MarsPlugin {
	public:

		ThrusterPlugin();
		ThrusterPlugin(std::string node_name, unsigned int amountOfActuators,
						std::vector <double> max_thruster_force,
						std::vector <mars::utils::Vector> thruster_pos,
						std::vector <mars::utils::Vector> thruster_dir);

		bool getPose(Eigen::Vector3d &pos, Eigen::Quaterniond &orientation);

		bool getVelocities(Eigen::Vector3d &lin_vel, Eigen::Vector3d &ang_vel);

		void setTarget(const std::vector<double> &target);

		void update(double time);


	private:
		// Initial
		unsigned long vehicle_id;
		const unsigned int amountOfActuators;
		std::vector <double> max_thruster_force;
		std::vector <mars::utils::Vector> thruster_pos;
		std::vector <mars::utils::Vector> thruster_dir;
		std::vector <mars::interfaces::sReal> thruster_force;
		pthread_mutex_t* node_update_mutex;

		unsigned int RATE;
	};
}

namespace avalon_simulation {

    class Actuators : public ActuatorsBase
    {
	friend class ActuatorsBase;

    protected:
		simulation::ThrusterPlugin* thruster_plugin;


    public:
        Actuators(std::string const& name = "avalon_simulation::Actuators", TaskCore::TaskState initial_state = Stopped);
        Actuators(std::string const& name, RTT::ExecutionEngine* engine, TaskCore::TaskState initial_state = Stopped);

	~Actuators();


        /** This hook is called by Orocos when the state machine transitions
         * from Stopped to Running. If it returns false, then the component will
         * stay in Stopped. Otherwise, it goes into Running and updateHook()
         * will be called.
         */
         bool startHook();

        /** This hook is called by Orocos when the component is in the Running
         * state, at each activity step. Here, the activity gives the "ticks"
         * when the hook should be called.
         *
         * The error(), exception() and fatal() calls, when called in this hook,
         * allow to get into the associated RunTimeError, Exception and
         * FatalError states.
         *
         * In the first case, updateHook() is still called, and recover() allows
         * you to go back into the Running state.  In the second case, the
         * errorHook() will be called instead of updateHook(). In Exception, the
         * component is stopped and recover() needs to be called before starting
         * it again. Finally, FatalError cannot be recovered.
         */
         void updateHook();

        /** This hook is called by Orocos when the component is in the
         * RunTimeError state, at each activity step. See the discussion in
         * updateHook() about triggering options.
         *
         * Call recover() to go back in the Runtime state.
         */
         void errorHook();

        private:
		unsigned int amount_of_actuators;
    };
}

#endif
