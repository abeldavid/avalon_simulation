require 'vizkit'
include Orocos

Orocos.initialize

joystick = Vizkit.default_loader.VirtualJoystick
joystick2 = Vizkit.default_loader.VirtualJoystick
joystick.show
joystick2.show

Orocos.run "AvalonSimulation" ,:wait => 9999999 do 
    simulation = TaskContext.get 'avalon_simulation'
    simulation.enable_gui = true
    simulation.configure
    simulation.start

    actuactors = TaskContext.get 'actuators_simulation'
    actuactors.configure
    actuactors.start
    writer = actuactors.command.writer

    joystick.connect(SIGNAL('axisChanged(double,double)'))do |x,y|
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

    joystick2.connect(SIGNAL('axisChanged(double,double)'))do |x,y|
        sample = writer.new_sample
        sample.time = Time.now 
        0.upto(5) do
            sample.mode << :DM_PWM
            sample.target << 0;
        end
        sample.target[1] = x
        sample.target[5] = -y
        writer.write sample
    end

    Vizkit.exec
end

