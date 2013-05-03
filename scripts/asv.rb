require 'vizkit'
include Orocos

Orocos.initialize

widget = Vizkit.load "simulator.ui"

Orocos.run "AvalonSimulation" ,:wait => 10000, :valgrind => false, :valgrind_options => ['--undef-value-errors=no'] do 
    simulation = TaskContext.get 'avalon_simulation'
    simulation.scenefile = "#{ENV['AUTOPROJ_PROJECT_BASE']}/simulation/orogen/avalon_simulation/configuration/avalon.scn"

    simulation.debug_sonar = false 
    simulation.use_osg_ocean = false 
    simulation.enable_gui = true
    simulation.configure
    simulation.start
    
    
    asv_actuators = TaskContext.get 'actuators'
    asv_actuators.node_name = "asv"
    asv_actuators.amount_of_actuators = 4
    
    asv_actuators.maximum_thruster_force.insert 0.0
    asv_actuators.maximum_thruster_force.insert 0.0
    asv_actuators.maximum_thruster_force.insert 2.0
    asv_actuators.maximum_thruster_force.insert 2.0
    
    asv_actuators.thruster_position.insert Eigen::Vector3.new(-0.5, -0.5, 0.0)
    asv_actuators.thruster_position.insert Eigen::Vector3.new(-0.5, 0.5, 0.0)
    asv_actuators.thruster_position.insert Eigen::Vector3.new(0.5, 0.0, 0.0)
    asv_actuators.thruster_position.insert Eigen::Vector3.new(-0.5, 0.0, 0.0)
    
    asv_actuators.thruster_direction.insert Eigen::Vector3.new(1.0, 0.0, 0.0)
    asv_actuators.thruster_direction.insert Eigen::Vector3.new(1.0, 0.0, 0.0)
    asv_actuators.thruster_direction.insert Eigen::Vector3.new(0.0, 1.0, 0.0)
    asv_actuators.thruster_direction.insert Eigen::Vector3.new(0.0, 1.0, 0.0)
    asv_actuators.configure
    asv_actuators.start
    asv_writer = asv_actuators.command.writer

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

#    Vizkit.display simulation
#    Vizkit.display asv_actuactors
    widget.show 
    Vizkit.exec


end

