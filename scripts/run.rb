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



    actuactors = TaskContext.get 'actuators'
    asv_actuactors = TaskContext.get 'asv_actuators'
    asv_actuactors.configure
    actuactors.configure
    actuactors.start
    asv_actuactors.start
    writer = actuactors.command.writer
    asv_writer = asv_actuactors.command.writer

    sonar = TaskContext.get 'sonar'
    sonar.configure
    sonar.start
    sonar_rear = TaskContext.get 'sonar_rear'
    sonar_rear.configure
    sonar_rear.start
    state_estimator = TaskContext.get 'state_estimator'
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

    Vizkit.display simulation
    Vizkit.display bottom_cam
    Vizkit.display front_cam
    Vizkit.display top_cam
    Vizkit.display sonar
    Vizkit.display sonar_rear
    Vizkit.display pinger_search
    Vizkit.display actuactors
    Vizkit.display asv_actuactors
    widget.show 
    Vizkit.exec


end

