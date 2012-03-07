require 'vizkit'
include Orocos

Orocos.initialize

widget = Vizkit.load "simulator.ui"

Orocos.run "AvalonSimulation" ,:wait => 60, :valgrind => false, :valgrind_options => ['--undef-value-errors=no'] do 
    simulation = TaskContext.get 'avalon_simulation'
    front_cam = TaskContext.get 'front_camera_simulation'
    bottom_cam = TaskContext.get 'bottom_camera_simulation'

    simulation.debug_sonar = true 
    simulation.use_osg_ocean = true 
    simulation.enable_gui = true
    simulation.configure
    simulation.start
    front_cam.start
    bottom_cam.start

    actuactors = TaskContext.get 'actuators_simulation'
    actuactors.configure
    actuactors.start
    writer = actuactors.command.writer

    sonar = TaskContext.get 'sonar_top_simulation'
    sonar.configure
    sonar.start
    sonar_rear = TaskContext.get 'sonar_rear_simulation'
    sonar_rear.configure
    sonar_rear.start
    state_estimator = TaskContext.get 'state_estimator_simulation'
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
    Vizkit.display simulation
    Vizkit.display bottom_cam
    Vizkit.display front_cam
    Vizkit.display sonar
    Vizkit.display sonar_rear
    widget.show 
    Vizkit.exec
end

