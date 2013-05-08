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


# Avalon
    actuators = TaskContext.get 'avalon_actuators'
    actuators.node_name = "avalon"
    actuators.amount_of_actuators = 6
    
    v01 = actuators.maximum_thruster_force    
    0.upto(5) do
        v01.push 10.0
    end
    actuators.maximum_thruster_force = v01
    
    v02 = actuators.thruster_position
    v02.push Eigen::Vector3.new(0.0, -0.157, 0.0)
    v02.push Eigen::Vector3.new(0.0, 0.157, 0.0)
    v02.push Eigen::Vector3.new(0.0, 0.0, 0.0)
    v02.push Eigen::Vector3.new(-0.688, 0.0820, 0.0)
    v02.push Eigen::Vector3.new(0.0, 0.0, 0.0)
    v02.push Eigen::Vector3.new(-0.5955, 0.0, 0.0)
    actuators.thruster_position = v02
    
    v03 = actuators.thruster_direction
    v03.push Eigen::Vector3.new(1.0, 0.0, 0.0)
    v03.push Eigen::Vector3.new(1.0, 0.0, 0.0)
    v03.push Eigen::Vector3.new(0.0, 1.0, 0.0)
    v03.push Eigen::Vector3.new(0.0, -1.0, 0.0)
    v03.push Eigen::Vector3.new(0.0, 0.0, 1.0)
    v03.push Eigen::Vector3.new(0.0, 0.0, 1.0)
    actuators.thruster_direction = v03
    
    actuators.configure
    actuators.start
    writer = actuators.command.writer
    
# ASV
    asv_actuators = TaskContext.get 'asv_actuators'
    asv_actuators.node_name = "asv"
    asv_actuators.amount_of_actuators = 4
    
    v11 =  asv_actuators.maximum_thruster_force
    v11.push 10.0
    v11.push 10.0
    v11.push 3.0
    v11.push 3.0
    asv_actuators.maximum_thruster_force = v11

    v12 = asv_actuators.thruster_position
    v12.push Eigen::Vector3.new(-0.5, -0.5, 0.0)
    v12.push Eigen::Vector3.new(-0.5, 0.5, 0.0)
    v12.push Eigen::Vector3.new(0.5, 0.0, 0.0)
    v12.push Eigen::Vector3.new(-0.5, 0.0, 0.0)
    asv_actuators.thruster_position = v12

    v13 = asv_actuators.thruster_direction
    v13.push Eigen::Vector3.new(1.0, 0.0, 0.0)
    v13.push Eigen::Vector3.new(1.0, 0.0, 0.0)
    v13.push Eigen::Vector3.new(0.0, 1.0, 0.0)
    v13.push Eigen::Vector3.new(0.0, 1.0, 0.0)
    asv_actuators.thruster_direction = v13
    
    asv_actuators.thruster_direction[3] = Eigen::Vector3.new(0.0, 1.0, 0.0)
    asv_actuators.configure
    asv_actuators.start
    asv_writer = asv_actuators.command.writer
    
# Testfahrzeug
    test_vehicle_actuators = TaskContext.get 'test_vehicle_actuators'
    test_vehicle_actuators.node_name = "test_vehicle"
    test_vehicle_actuators.amount_of_actuators = 6
    
    v21 = test_vehicle_actuators.maximum_thruster_force
    0.upto(5) do
        v21.push 10.0
    end
    test_vehicle_actuators.maximum_thruster_force = v21
    
    v22 = test_vehicle_actuators.thruster_position
    v22.push Eigen::Vector3.new(0.5, 0.0, 0.0)
    v22.push Eigen::Vector3.new(0.0, 0.5, 0.0)
    v22.push Eigen::Vector3.new(-0.5, 0.0, 0.0)
    v22.push Eigen::Vector3.new(0.0, -0.5, 0.0)
    v22.push Eigen::Vector3.new(0.0, 0.0, 0.5)
    v22.push Eigen::Vector3.new(0.0, 0.0, -0.5)
    test_vehicle_actuators.thruster_position= v22
    
    v23 = test_vehicle_actuators.thruster_direction
    v23.push Eigen::Vector3.new(0.0, 0.0, 1.0)
    v23.push Eigen::Vector3.new(1.0, 0.0, 0.0)
    v23.push Eigen::Vector3.new(0.0, 0.0, 1.0)
    v23.push Eigen::Vector3.new(1.0, 0.0, 0.0)
    v23.push Eigen::Vector3.new(0.0, 1.0, 0.0)
    v23.push Eigen::Vector3.new(0.0, 1.0, 0.0)
    test_vehicle_actuators.thruster_direction = v23
    
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