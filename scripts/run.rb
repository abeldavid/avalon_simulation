require 'vizkit'
include Orocos

Orocos.initialize

widget = Vizkit.load "simulator.ui"

#Orocos.run "AvalonSimulation" ,:wait => 10000, :valgrind => false, :valgrind_options => ['--undef-value-errors=no'] do 
Orocos.run "AvalonSimulation" ,:wait => 10000, :valgrind => false, :valgrind_options => ['--undef-value-errors=no'] do 
    simulation = TaskContext.get 'avalon_simulation'
    front_cam = TaskContext.get 'front_camera'
    bottom_cam = TaskContext.get 'bottom_camera'
    top_cam = TaskContext.get 'top_camera'
    
    pinger_search = TaskContext.get 'pingersearch_simulation'
    asv_navigation = TaskContext.get 'asv_navigation'
#    white_light = TaskContext.get 'white_light'


#     white_light.interval_mode = 1
#     white_light.constantInterval = 3000
#      white_light.interval_mode = 2
#      white_light.randomInterval_min = 1;
#      white_light.randomInterval_max = 5000;
      
    
 #   simulation.scenefile = "#{ENV['AUTOPROJ_PROJECT_BASE']}/simulation/orogen/avalon_simulation/configuration/demo.scn"    
simulation.scenefile = "#{ENV['AUTOPROJ_PROJECT_BASE']}/simulation/orogen/avalon_simulation/configuration/avalon.scn"

    simulation.debug_sonar = false 
    simulation.use_osg_ocean = false 
    simulation.enable_gui = true
    simulation.configure
    simulation.start
    front_cam.start
    bottom_cam.start
    top_cam.start
    pinger_search.start
    asv_navigation.start
#    white_light.start

# Konfiguration der Aktuatoren für alle Fahrzeuge:    

	# Avalon
    actuators = TaskContext.get 'actuators'
    actuators.node_name = "avalon"
    actuators.amount_of_actuators = 6
    0.upto(5) do
        actuators.maximum_thruster_force << 10.0
        actuators.thruster_position << Eigen::Vector3.new(0.0, 0.0, 0.0)
        actuators.thruster_direction << Eigen::Vector3.new(0.0, 0.0, 0.0)
    end
    actuators.thruster_position[0] = Eigen::Vector3.new(0.0, -0.157, 0.0)
    actuators.thruster_position[1] = Eigen::Vector3.new(0.0, 0.157, 0.0)
    actuators.thruster_position[2] = Eigen::Vector3.new(0.0, 0.0, 0.0)
    actuators.thruster_position[3] = Eigen::Vector3.new(-0.688, 0.0820, 0.0)
    actuators.thruster_position[4] = Eigen::Vector3.new(0.0, 0.0, 0.0)
    actuators.thruster_position[5] = Eigen::Vector3.new(-0.5955, 0.0, 0.0)
    actuators.thruster_direction[0] = Eigen::Vector3.new(1.0, 0.0, 0.0)
    actuators.thruster_direction[1] = Eigen::Vector3.new(1.0, 0.0, 0.0)
    actuators.thruster_direction[2] = Eigen::Vector3.new(0.0, 1.0, 0.0)
    actuators.thruster_direction[3] = Eigen::Vector3.new(0.0, -1.0, 0.0)
    actuators.thruster_direction[4] = Eigen::Vector3.new(0.0, 0.0, 1.0)
    actuators.thruster_direction[5] = Eigen::Vector3.new(0.0, 0.0, 1.0)
    actuators.configure
    actuators.start
    writer = actuators.command.writer
    
	# ASV
    asv_actuators = TaskContext.get 'actuators'
    asv_actuators.node_name = "asv"
    asv_actuators.amount_of_actuators = 4
    0.upto(3) do
        asv_actuators.maximum_thruster_force << 10.0
        asv_actuators.thruster_position << Eigen::Vector3.new(0.0, 0.0, 0.0)
        asv_actuators.thruster_direction << Eigen::Vector3.new(0.0, 0.0, 0.0)
    end
    asv_actuators.maximum_thruster_force[2] = 2.0
    asv_actuators.maximum_thruster_force[3] = 2.0
    asv_actuators.thruster_position[0] = Eigen::Vector3.new(-0.5, -0.5, 0.0)
    asv_actuators.thruster_position[1] = Eigen::Vector3.new(-0.5, 0.5, 0.0)
    asv_actuators.thruster_position[2] = Eigen::Vector3.new(0.5, 0.0, 0.0)
    asv_actuators.thruster_position[3] = Eigen::Vector3.new(-0.5, 0.0, 0.0)
    asv_actuators.thruster_direction[0] = Eigen::Vector3.new(1.0, 0.0, 0.0)
    asv_actuators.thruster_direction[1] = Eigen::Vector3.new(1.0, 0.0, 0.0)
    asv_actuators.thruster_direction[2] = Eigen::Vector3.new(0.0, 1.0, 0.0)
    asv_actuators.thruster_direction[3] = Eigen::Vector3.new(0.0, 1.0, 0.0)
    asv_actuators.configure
    asv_actuators.start
    asv_writer = asv_actuators.command.writer
    
	#Testfahrzeug
    test_vehicle_actuators = TaskContext.get 'actuators'
    test_vehicle_actuators.node_name = "test_vehicle"
    test_vehicle_actuators.amount_of_actuators = 6
    0.upto(5) do
        test_vehicle_actuators.maximum_thruster_force << 10.0
        test_vehicle_actuators.thruster_position << Eigen::Vector3.new(0.0, 0.0, 0.0)
        test_vehicle_actuators.thruster_direction << Eigen::Vector3.new(0.0, 0.0, 0.0)
    end
    test_vehicle_actuators.thruster_position[0] = Eigen::Vector3.new(0.5, 0.0, 0.0)
    test_vehicle_actuators.thruster_position[1] = Eigen::Vector3.new(0.0, 0.5, 0.0)
    test_vehicle_actuators.thruster_position[2] = Eigen::Vector3.new(-0.5, 0.0, 0.0)
    test_vehicle_actuators.thruster_position[3] = Eigen::Vector3.new(0.0, -0.5, 0.0)
    test_vehicle_actuators.thruster_position[4] = Eigen::Vector3.new(0.0, 0.0, 0.5)
    test_vehicle_actuators.thruster_position[5] = Eigen::Vector3.new(0.0, 0.0, -0.5)
    test_vehicle_actuators.thruster_direction[0] = Eigen::Vector3.new(0.0, 0.0, 1.0)
    test_vehicle_actuators.thruster_direction[1] = Eigen::Vector3.new(1.0, 0.0, 0.0)
    test_vehicle_actuators.thruster_direction[2] = Eigen::Vector3.new(0.0, 0.0, 1.0)
    test_vehicle_actuators.thruster_direction[3] = Eigen::Vector3.new(1.0, 0.0, 0.0)
    test_vehicle_actuators.thruster_direction[4] = Eigen::Vector3.new(0.0, 1.0, 0.0)
    test_vehicle_actuators.thruster_direction[5] = Eigen::Vector3.new(0.0, 1.0, 0.0)
    test_vehicle_actuators.configure
    test_vehicle_actuators.start
    test_vehicle_writer = test_vehicle_actuators.command.writer




    sonar = TaskContext.get 'sonar'
    sonar.configure
    sonar.start
    sonar_rear = TaskContext.get 'sonar_rear'
    sonar_rear.configure
    sonar_rear.start
    state_estimator = TaskContext.get 'state_estimator'
    state_estimator.configure
    state_estimator.start
#    Vizkit.display sonar.sonar_beam, :widget => widget.sonar_top
#    Vizkit.display sonar_rear.sonar_beam, :widget => widget.sonar_rear
#    Vizkit.display state_estimator.pose_samples, :widget => widget.orientation

    widget.joystick1.connect(SIGNAL('axisChanged(double,double)'))do |x,y|
        sample = writer.new_sample
        sample.time = Time.now 
        0.upto(5) do
            sample.mode << :DM_PWM
            sample.target << 0;
        end
        sample.target[2] = -x
        sample.target[3] = -x
        sample.target[4] = y
        writer.write sample
    end

    widget.joystick2.connect(SIGNAL('axisChanged(double,double)'))do |x,y|
        sample = writer.new_sample
        sample.time = Time.now 
        0.upto(5) do
            sample.mode << :DM_PWM
            sample.target << 0;
        end
        sample.target[1] = -x
        sample.target[5] = y
        writer.write sample
    end

    asv_navigation.addWaypoint(5.0,2.0)
    asv_navigation.addWaypoint(3.0,5.0)
    asv_navigation.addWaypoint(-3.0,3.0)

    asv_navigation.clearWaypoints

#    Vizkit.display simulation
#    Vizkit.display bottom_cam
#    Vizkit.display front_cam
#    Vizkit.display top_cam
#    Vizkit.display sonar
#    Vizkit.display sonar_rear
#    Vizkit.display pinger_search
#    Vizkit.display actuators
#    Vizkit.display asv_actuators
#    Vizkit.display test_vehicle_actuators
    widget.show 
    Vizkit.exec


end

