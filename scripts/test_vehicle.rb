require 'vizkit'
include Orocos

Orocos.initialize

widget = Vizkit.load "simulator.ui"

Orocos.run "AvalonSimulation" ,:wait => 10000, :valgrind => false, :valgrind_options => ['--undef-value-errors=no'] do 
    simulation = TaskContext.get 'avalon_simulation'
    simulation.scenefile = "#{ENV['AUTOPROJ_PROJECT_BASE']}/simulation/orogen/avalon_simulation/configuration/test_vehicle.scn"

    simulation.debug_sonar = false 
    simulation.use_osg_ocean = false 
    simulation.enable_gui = true
    simulation.configure
    simulation.start
    test_vehicle_actuactors = TaskContext.get 'test_vehicle_actuators'
    test_vehicle_actuactors.configure
    test_vehicle_actuactors.start
    test_vehicle_writer = test_vehicle_actuactors.command.writer

    widget.joystick1.connect(SIGNAL('axisChanged(double,double)'))do |x,y|
        sample = test_vehicle_writer.new_sample
        sample.time = Time.now 
        0.upto(3) do
            sample.mode << :DM_PWM
            sample.target << 0;
        end
        sample.target[0] = x
        sample.target[1] = x
        sample.target[2] = -y
        sample.target[3] = y
        test_vehicle_writer.write sample
    end

    Vizkit.display simulation
    Vizkit.display test_vehicle_actuactors
    widget.show 
    Vizkit.exec


end

