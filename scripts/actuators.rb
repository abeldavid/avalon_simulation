class ActuatorsConfig

# Konfiguration der Aktuatoren für alle Fahrzeuge:    

attr_reader :avalon_amount_of_actuators, :avalon_maximum_thruster_force, :avalon_thruster_position, :avalon_thruster_direction, :asv_amount_of_actuators, :asv_maximum_thruster_force, :asv_thruster_position, :asv_thruster_direction, :test_vehicle_amount_of_actuators, :test_vehicle_maximum_thruster_force, :test_vehicle_thruster_position, :test_vehicle_thruster_direction
	    
def initialize()
	# Avalon
	    @avalon_amount_of_actuators = 6
	
	    avalon_maximum_thruster_force = []
	    0.upto(5) do
		avalon_maximum_thruster_force.push 10.0
	    end
	    @avalon_maximum_thruster_force = avalon_maximum_thruster_force
	    
	    avalon_thruster_position = []
	    avalon_thruster_position.push Eigen::Vector3.new(0.0, -0.157, 0.0)
	    avalon_thruster_position.push Eigen::Vector3.new(0.0, 0.157, 0.0)
	    avalon_thruster_position.push Eigen::Vector3.new(0.0, 0.0, 0.0)
	    avalon_thruster_position.push Eigen::Vector3.new(-0.688, 0.0820, 0.0)
	    avalon_thruster_position.push Eigen::Vector3.new(0.0, 0.0, 0.0)
	    avalon_thruster_position.push Eigen::Vector3.new(-0.5955, 0.0, 0.0)
	    @avalon_thruster_position = avalon_thruster_position
	    
	    avalon_thruster_direction = []
	    avalon_thruster_direction.push Eigen::Vector3.new(1.0, 0.0, 0.0)
	    avalon_thruster_direction.push Eigen::Vector3.new(1.0, 0.0, 0.0)
	    avalon_thruster_direction.push Eigen::Vector3.new(0.0, 1.0, 0.0)
	    avalon_thruster_direction.push Eigen::Vector3.new(0.0, -1.0, 0.0)
	    avalon_thruster_direction.push Eigen::Vector3.new(0.0, 0.0, 1.0)
	    avalon_thruster_direction.push Eigen::Vector3.new(0.0, 0.0, 1.0)
	    @avalon_thruster_direction = avalon_thruster_direction
	    
	# ASV
	    @asv_amount_of_actuators = 4
	    
	    asv_maximum_thruster_force  = []
	    asv_maximum_thruster_force.push 10.0
	    asv_maximum_thruster_force.push 10.0
	    asv_maximum_thruster_force.push 3.0
	    asv_maximum_thruster_force.push 3.0
	    @asv_maximum_thruster_force = asv_maximum_thruster_force
	    
	    asv_thruster_position = []
	    asv_thruster_position.push Eigen::Vector3.new(-0.5, -0.5, 0.0)
	    asv_thruster_position.push Eigen::Vector3.new(-0.5, 0.5, 0.0)
	    asv_thruster_position.push Eigen::Vector3.new(0.5, 0.0, 0.0)
	    asv_thruster_position.push Eigen::Vector3.new(-0.5, 0.0, 0.0)
	    @asv_thruster_position = asv_thruster_position

	    asv_thruster_direction = []
	    asv_thruster_direction.push Eigen::Vector3.new(1.0, 0.0, 0.0)
	    asv_thruster_direction.push Eigen::Vector3.new(1.0, 0.0, 0.0)
	    asv_thruster_direction.push Eigen::Vector3.new(0.0, 1.0, 0.0)
	    asv_thruster_direction.push Eigen::Vector3.new(0.0, 1.0, 0.0)
	    @asv_thruster_direction = asv_thruster_direction
	    
	# Testfahrzeug
	    @test_vehicle_amount_of_actuators = 6
	    
	    test_vehicle_maximum_thruster_force  = []
	    0.upto(5) do
		test_vehicle_maximum_thruster_force.push 10.0
	    end
	    @test_vehicle_maximum_thruster_force = test_vehicle_maximum_thruster_force
	    
	    test_vehicle_thruster_position = []
	    test_vehicle_thruster_position.push Eigen::Vector3.new(0.5, 0.0, 0.0)
	    test_vehicle_thruster_position.push Eigen::Vector3.new(0.0, 0.5, 0.0)
	    test_vehicle_thruster_position.push Eigen::Vector3.new(-0.5, 0.0, 0.0)
	    test_vehicle_thruster_position.push Eigen::Vector3.new(0.0, -0.5, 0.0)
	    test_vehicle_thruster_position.push Eigen::Vector3.new(0.0, 0.0, 0.5)
	    test_vehicle_thruster_position.push Eigen::Vector3.new(0.0, 0.0, -0.5)
	    @test_vehicle_thruster_position = test_vehicle_thruster_position
	    
	    test_vehicle_thruster_direction = []
	    test_vehicle_thruster_direction.push Eigen::Vector3.new(0.0, 0.0, 1.0)
	    test_vehicle_thruster_direction.push Eigen::Vector3.new(1.0, 0.0, 0.0)
	    test_vehicle_thruster_direction.push Eigen::Vector3.new(0.0, 0.0, 1.0)
	    test_vehicle_thruster_direction.push Eigen::Vector3.new(1.0, 0.0, 0.0)
	    test_vehicle_thruster_direction.push Eigen::Vector3.new(0.0, 1.0, 0.0)
	    test_vehicle_thruster_direction.push Eigen::Vector3.new(0.0, 1.0, 0.0)
	    @test_vehicle_thruster_direction = test_vehicle_thruster_direction
    end
end