require 'vizkit'
include Orocos

Orocos.initialize

widget = Vizkit.load "simulator.ui"

#Orocos.run "AvalonSimulation" ,:wait => 10000, :valgrind => false, :valgrind_options => ['--undef-value-errors=no'] do 
Orocos.run "AvalonSimulation" ,:wait => 10000, :valgrind => false, :valgrind_options => ['--undef-value-errors=no'] do 
    simulation = TaskContext.get 'avalon_simulation'
    
    pinger_search = TaskContext.get 'pingersearch_simulation'
    asv_navigation = TaskContext.get 'asv_navigation'


      white_light = TaskContext.get 'white_light'
#     white_light.interval_mode = 1
#     white_light.constantInterval = 3000
      white_light.interval_mode = 2
      white_light.randomInterval_min = 1;
      white_light.randomInterval_max = 5000;
      white_light.start
      
simulation.scenefile = "#{ENV['AUTOPROJ_PROJECT_BASE']}/simulation/orogen/avalon_simulation/configuration/avalon.scn"

    simulation.debug_sonar = false 
    simulation.use_osg_ocean = false 
    simulation.enable_gui = true
    simulation.configure
    simulation.start
    pinger_search.start
    asv_navigation.start

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


# Camera configuration

    front_cam = TaskContext.get 'front_camera'
    front_cam.name = 'front_cam'
    front_cam.configure
    front_cam.start
    
    bottom_cam = TaskContext.get 'bottom_camera'
    bottom_cam.name = 'bottom_cam'
    bottom_cam.configure
    bottom_cam.start
    
    top_cam = TaskContext.get 'top_camera'
    top_cam.name = 'top_cam'
    top_cam.configure
    top_cam.start


    sonar = TaskContext.get 'sonar'
    sonar.node_name = "sonar_top_sensor"
    sonar.left_limit = Math::PI
    sonar.right_limit = -Math::PI
    sonar.resolution = 0.1
    sonar.maximum_distance = 100.0
    sonar.ping_pong_mode = false
    sonar.configure
    sonar.start
    
    sonar_rear = TaskContext.get 'sonar_rear'
    sonar_rear.node_name = "sonar_rear_sensor"
    sonar_rear.left_limit = 0.7*Math::PI
    sonar_rear.right_limit = 0.3*Math::PI
    sonar_rear.resolution = 0.1
    sonar_rear.maximum_distance = 50.0
    sonar_rear.ping_pong_mode = true
    sonar_rear.configure
    sonar_rear.start
        
    ground_distance = TaskContext.get 'ground_distance'
    ground_distance.configure
    ground_distance.start
    
    state_estimator = TaskContext.get 'state_estimator'
    state_estimator.node_name = 'avalon'
    state_estimator.configure
    state_estimator.start
    Vizkit.display sonar.sonar_beam, :widget => widget.sonar_top
    Vizkit.display sonar_rear.sonar_beam, :widget => widget.sonar_rear
    Vizkit.display state_estimator.pose_samples, :widget => widget.orientation

    widget.joystick1.connect(SIGNAL('axisChanged(double,double)'))do |x,y|
        sample = writer.new_sample
        sample.time = Time.now 
        0.upto(5) do
            sample.mode << :DM_PWM
            sample.target << 0;
        end
        sample.target[0] = x
        sample.target[1] = x
        sample.target[2] = -y
        sample.target[3] = -y
        writer.write sample
    end

    widget.joystick2.connect(SIGNAL('axisChanged(double,double)'))do |x,y|
        sample = writer.new_sample
        sample.time = Time.now 
        0.upto(5) do
            sample.mode << :DM_PWM
            sample.target << 0;
        end
        sample.target[4] = x
        sample.target[2] = -y
        writer.write sample
    end

    asv_navigation.addWaypoint(5.0,2.0)
    asv_navigation.addWaypoint(3.0,5.0)
    asv_navigation.addWaypoint(-3.0,3.0)

    asv_navigation.clearWaypoints

#    Vizkit.display simulation
    Vizkit.display bottom_cam
    Vizkit.display front_cam
    Vizkit.display top_cam
#    Vizkit.display sonar
#    Vizkit.display sonar_rear
#    Vizkit.display ground_distance
#    Vizkit.display pinger_search
#    Vizkit.display actuators
#    Vizkit.display asv_actuators
#    Vizkit.display test_vehicle_actuators
    widget.show 
    Vizkit.exec


end

