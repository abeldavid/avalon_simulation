require 'vizkit'
include Orocos

Orocos.initialize

widget = Vizkit.load "simulator.ui"

Orocos.run "AvalonSimulation" ,:wait => 10000, :valgrind => false, :valgrind_options => ['--undef-value-errors=no'] do
    simulation = TaskContext.get 'avalon_simulation'
    simulation.scenefile = "#{ENV['AUTOPROJ_CURRENT_ROOT']}/simulation/orogen/avalon_simulation/configuration/avalon.scn"

    simulation.debug_sonar = false
    simulation.use_osg_ocean = false
    simulation.enable_gui = true
    simulation.configure
    simulation.start


    state_estimator = TaskContext.get 'state_estimator'
    state_estimator.node_name = 'asv'
    state_estimator.configure
    state_estimator.start

# Konfiguration der Aktuatoren für alle Fahrzeuge:    
require 'actuators'
values = ActuatorsConfig.new()
# Avalon
    actuators = TaskContext.get 'avalon_actuators'
    actuators.node_name = "avalon"
    actuators.amount_of_actuators = values.avalon_amount_of_actuators
    actuators.maximum_thruster_force = values.avalon_maximum_thruster_force    
    actuators.thruster_position = values.avalon_thruster_position    
    actuators.thruster_direction = values.avalon_thruster_direction    
    actuators.configure
    actuators.start
    writer = actuators.command.writer
    
# ASV
    asv_actuators = TaskContext.get 'asv_actuators'
    asv_actuators.node_name = "asv"
    asv_actuators.amount_of_actuators = values.asv_amount_of_actuators  
    asv_actuators.maximum_thruster_force = values.asv_maximum_thruster_force
    asv_actuators.thruster_position = values.asv_thruster_position
    asv_actuators.thruster_direction = values.asv_thruster_direction    
    asv_actuators.configure
    asv_actuators.start
    asv_writer = asv_actuators.command.writer
    
# Testfahrzeug
    test_vehicle_actuators = TaskContext.get 'test_vehicle_actuators'
    test_vehicle_actuators.node_name = "test_vehicle"
    test_vehicle_actuators.amount_of_actuators = values.test_vehicle_amount_of_actuators
    test_vehicle_actuators.maximum_thruster_force = values.test_vehicle_maximum_thruster_force    
    test_vehicle_actuators.thruster_position= values.test_vehicle_thruster_position    
    test_vehicle_actuators.thruster_direction = values.test_vehicle_thruster_direction    
    test_vehicle_actuators.configure
    test_vehicle_actuators.start
    test_vehicle_writer = test_vehicle_actuators.command.writer
    
    
    

    widget.joystick1.connect(SIGNAL('axisChanged(double,double)'))do |x,y|
        sample = asv_writer.new_sample
        sample.time = Time.now
        0.upto(3) do
            sample.mode << :DM_PWM
            sample.target << 0;
        end
        sample.target[0] = x
        sample.target[1] = x
        sample.target[2] = -y
        sample.target[3] = y
        asv_writer.write sample
    end

    widget.joystick2.connect(SIGNAL('axisChanged(double,double)'))do |x,y|
        sample = asv_writer.new_sample
        sample.time = Time.now
        0.upto(3) do
            sample.mode << :DM_PWM
            sample.target << 0;
        end
        sample.target[2] = -y
        sample.target[3] = -y
        asv_writer.write sample
    end

#    Vizkit.display simulation
#    Vizkit.display asv_actuactors
    widget.show
    Vizkit.exec


end