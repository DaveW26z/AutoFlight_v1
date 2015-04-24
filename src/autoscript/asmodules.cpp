#include "asmodules.h"

#define CHECK_RATE 25 // Every how many milliseconds it should be checked if a script abort has been requested

#define ASSERT_ARDRONE_B(d) if(d == NULL) { return false; } // Returns false when the ARDrone control object is NULL (for boolean functions)
#define ASSERT_ARDRONE_F(d) if(d == NULL) { return -1.0f; } // Returns -1.0f when the ARDrone control object is NULL (for float functions)

#define SIMULATE_ACTION_B(sim, text, ssui) if(sim) { ssui->printAction(text); return true; } // Returns true if simulation mode is turned on and prints specified text
#define SIMULATE_INPUT_B(sim, question, ssui) if(sim) { return ssui->getSimulatedBoolInput(question); }    // If simulation mode is on, asks for "true" or "false"
#define SIMULATE_INPUT_F(sim, question, unit, ssui) if(sim) { return ssui->getSimulatedFloatInput(question, unit); } // If sim. is on, asks for a float value

//////////// CONTROL FUNCTIONS ////////////

Control::Control(std::shared_ptr<Drone> drone, bool simulationMode, IScriptSimulationUI *simulationUI)
{
	d = drone;
	sim = simulationMode;
	ssui = simulationUI;
}

bool Control::takeOff()
{
	ASSERT_ARDRONE_B(d)
	SIMULATE_ACTION_B(sim, "Taking off", ssui)

	//TODO: return d->drone_takeOff();
	return false;
}

bool Control::land()
{
	ASSERT_ARDRONE_B(d)
	SIMULATE_ACTION_B(sim, "Landing", ssui)

	//TODO: return d->drone_land();
	return false;
}

bool Control::move(float phi, float theta, float gaz, float yaw)
{
	ASSERT_ARDRONE_B(d)

	if(sim)
	{
		std::stringstream description;
		description << "Moving with phi = " << phi << "; theta = " << theta << "; gaz = " << gaz << " and yaw = " << yaw << ".";

		SIMULATE_ACTION_B(sim, description.str(), ssui)
	}

	//TODO: return d->drone_move(phi, theta, gaz, yaw);
	return false;
}

bool Control::move_distance(float phi, float theta, float gaz, float yaw, float centimeters)
{
	ASSERT_ARDRONE_B(d)

	if(sim)
	{
		std::stringstream description;
		description << "Moving for " << centimeters << "cm with phi = " << phi << "; theta = " << theta << "; gaz = " << gaz << " and yaw = " << yaw << ".";

		SIMULATE_ACTION_B(sim, description.str(), ssui)
	}

	//TODO: this
	/*if(d->drone_getLinearVelocity().vx != 0) // Check if linear velocity data is real
	{
		double distance = centimeters / 100.0;

		double time = 0;
		double speedsum = 0;
		int iteration_number = 0;

		double altitude_at_beginning = d->drone_getAltitude();

		move(phi, theta, gaz, yaw);

		bool completed = false;
		while(!completed)
		{
			iteration_number++;

			boost::this_thread::sleep_for(boost::chrono::milliseconds(CHECK_RATE));

			if(phi == 0 && theta == 0 && gaz != 0) // Precision altitude control mode
			{

			}
			else if(gaz == 0) // Ignore altitude readings
			{
				ardrone::linearvelocity v = d->drone_getLinearVelocity();

				time += CHECK_RATE / 1000;
				speedsum += sqrt(v.vx*v.vx + v.vy*v.vy);
				double avaragespeed = speedsum / iteration_number;

				if(avaragespeed * time >= distance)
				{
					completed = true;
				}
			}
			else if(phi == 0 && theta == 0 && gaz == 0)
			{
				break;
			}
			else // Combined altitude and pitch/roll distance measurement
			{
				ardrone::linearvelocity v = d->drone_getLinearVelocity();

				time += CHECK_RATE / 1000;
				speedsum += sqrt(v.vx*v.vx + v.vy*v.vy);
				double avaragespeed = speedsum / iteration_number;

				double deltaAltitude = abs(d->drone_getAltitude()-altitude_at_beginning);

				if(sqrt(avaragespeed * time + deltaAltitude) >= distance)
				{
					completed = true;
				}
			}

			if(abortFlag)
			{
				break;
			}
		}

		d->drone_hover();

		return completed;
	}
	else
	{*/
		return false; // Something is wrong with the vertical camera. Linear velocity reported to be 0, which is not possible
	//}
}

bool Control::move_time(float phi, float theta, float gaz, float yaw, int milliseconds)
{
	ASSERT_ARDRONE_B(d)

	if(sim)
	{
		std::stringstream description;
		description << "Moving for " << milliseconds << "ms with phi = " << phi << "; theta = " << theta << "; gaz = " << gaz << " and yaw = " << yaw << ".";

		SIMULATE_ACTION_B(sim, description.str(), ssui)
	}

	move(phi, theta, gaz, yaw);

	for(int i = 0; i < (milliseconds / CHECK_RATE); i++)
	{
		boost::this_thread::sleep_for(boost::chrono::milliseconds(CHECK_RATE));

		if(abortFlag)
		{
			break;
		}
	}

	hover();

	return true;
}

bool Control::forward(float speed)
{
	return move(0, -speed, 0, 0);
}

bool Control::forward_time(float speed, int milliseconds)
{
	return move_time(0, -speed, 0, 0, milliseconds);
}

bool Control::forward_distance(float speed, float centimeters)
{
	return move_distance(0, -speed, 0, 0, centimeters);
}

bool Control::backward(float speed)
{
	return move(0, speed, 0, 0);
}

bool Control::backward_time(float speed, int milliseconds)
{
	return move_time(0, speed, 0, 0, milliseconds);
}

bool Control::backward_distance(float speed, float centimeters)
{
	return move_distance(0, speed, 0, 0, centimeters);
}

bool Control::left(float speed)
{
	return move(-speed, 0, 0, 0);
}

bool Control::left_time(float speed, int milliseconds)
{
	return move_time(-speed, 0, 0, 0, milliseconds);
}

bool Control::left_distance(float speed, float centimeters)
{
	return move_distance(-speed, 0, 0, 0, centimeters);
}

bool Control::right(float speed)
{
	return move(speed, 0, 0, 0);
}

bool Control::right_time(float speed, int milliseconds)
{
	return move_time(speed, 0, 0, 0, milliseconds);
}

bool Control::right_distance(float speed, float centimeters)
{
	return move_distance(speed, 0, 0, 0, centimeters);
}

bool Control::up(float speed)
{
	return move(0, 0, speed, 0);
}

bool Control::up_time(float speed, int milliseconds)
{
	return move_time(0, 0, speed, 0, milliseconds);
}

bool Control::up_distance(float speed, float centimeters)
{
	return move_distance(0, 0, speed, 0, centimeters);
}

bool Control::down(float speed)
{
	return move(0, 0, -speed, 0);
}

bool Control::down_time(float speed, int milliseconds)
{
	return move_time(0, 0, -speed, 0, milliseconds);
}

bool Control::down_distance(float speed, float centimeters)
{
	return move_distance(0, 0, -speed, 0, centimeters);
}

bool Control::rotate(float speed, float degs, bool clockwise)
{
	ASSERT_ARDRONE_B(d)

	if(sim)
	{
		std::stringstream description;
		description << "Rotating " << degs << " degrees with yaw = ";
		if(!clockwise)
		{
			description << "-";
		}
		description << speed << ".";

		SIMULATE_ACTION_B(sim, description.str(), ssui)
	}

	//TODO: this
	/*
	if(degs < 0)
	{
		clockwise = !clockwise;
		degs *= -1;
	}

	float initialHeading = d->drone_getRelativeHeading(clockwise);
	float previousHeading = initialHeading;
	float degreesRotated = 0;

	if(clockwise)
	{
		move(0, 0, 0, speed);
	}
	else
	{
		move(0, 0, 0, -speed);
	}

	bool complete = false;

	while(!complete)
	{
		float heading = d->drone_getRelativeHeading(clockwise);

		if(heading >= previousHeading)
		{
			degreesRotated += heading - previousHeading;
		}
		else
		{
			// Just got from (for example) 246 to 5 degrees
			degreesRotated += (360 - previousHeading) + heading;
		}

		if(degreesRotated >= degs)
		{
			complete = true;
			break;
		}

		boost::this_thread::sleep_for(boost::chrono::milliseconds(CHECK_RATE));

		previousHeading = heading;

		if(abortFlag)
		{
			break;
		}
	}

	hover();

	return complete;*/
	return false;
}

bool Control::hover()
{
	ASSERT_ARDRONE_B(d)
	SIMULATE_ACTION_B(sim, "Hovering", ssui)

	//TODO: return d->drone_hover();
	return false;
}

bool Control::flip(std::string direction)
{
	ASSERT_ARDRONE_B(d)
	SIMULATE_ACTION_B(sim, "Flipping", ssui)

	//TODO: this
	/*
	if(direction == "AHEAD")
	{
		return d->drone_flip(ardrone::flip::AHEAD);
	}
	else if(direction == "BEHIND")
	{
		return d->drone_flip(ardrone::flip::BEHIND);
	}
	else if(direction == "LEFT")
	{
		return d->drone_flip(ardrone::flip::LEFT);
	}
	else if(direction == "RIGHT")
	{
		return d->drone_flip(ardrone::flip::RIGHT);
	}
	else
	{*/
		return false;
	//}
}

//////////// SENSOR DATA RETREIVAL FUNCTIONS ////////////

Sensors::Sensors(std::shared_ptr<Drone> drone, bool simulationMode, IScriptSimulationUI *simulationUI)
{
	d = drone;
	sim = simulationMode;
	ssui = simulationUI;
}

float Sensors::getAltitude()
{
	ASSERT_ARDRONE_F(d)
	SIMULATE_INPUT_F(sim, "Please enter a simulated value for the altitude of the drone", "m", ssui)

	//TODO: return d->drone_getAltitude();
	return 0;
}

float Sensors::getOrientation(std::string axis)
{
	ASSERT_ARDRONE_F(d)
	SIMULATE_INPUT_F(sim, std::string("Please enter a simulated angle for the drone's ").append(axis.append(" axis (-180 to +180)")), "degree", ssui)

	//TODO: this
	/*
	if(axis == "YAW")
	{
		return d->drone_getOrientation().yaw;
	}
	else if(axis == "PITCH")
	{
		return d->drone_getOrientation().pitch;
	}
	else if(axis == "ROLL")
	{
		return d->drone_getOrientation().roll;
	}
	else
	{*/
		return -1;
	//}
}

float Sensors::getOrientation360(std::string axis, bool clockwise)
{
	ASSERT_ARDRONE_F(d)
	SIMULATE_INPUT_F(sim, std::string("Please enter a simulated angle for the drone's ").append(axis.append(" axis (0 to 360)")), "degree", ssui)

	float orientation = getOrientation(axis);

	if(clockwise)
	{
		if(orientation < 0)
		{
			orientation += 360;
		}
	}
	else
	{
		if(orientation >= 0)
		{
			orientation = 360 - orientation;
		}
		else
		{
			orientation *= -1;
		}
	}

	return orientation;
}

float Sensors::getAcceleration(std::string axis)
{
	ASSERT_ARDRONE_F(d)
	SIMULATE_INPUT_F(sim, std::string("Please enter a simulated value for the acceleration on the drone's ").append(axis.append(" axis")), "g", ssui)

	//TODO: this
	/*
	if(axis == "X")
	{
		return d->drone_getAcceleration().ax;
	}
	else if(axis == "Y")
	{
		return d->drone_getAcceleration().ay;
	}
	else if(axis == "Z")
	{
		return d->drone_getAcceleration().az;
	}
	else
	{*/
		return -1;
	//}
}

float Sensors::getLinearVelocity(std::string axis)
{
	ASSERT_ARDRONE_F(d)
	SIMULATE_INPUT_F(sim, std::string("Please enter a simulated linear velocity on the drone's ").append(axis.append(" axis")), "m/s", ssui)

	//TODO: this
	/*
	if(axis == "X")
	{
		return d->drone_getLinearVelocity().vx;
	}
	else if(axis == "Y")
	{
		return d->drone_getLinearVelocity().vy;
	}
	else if(axis == "Z")
	{
		return d->drone_getLinearVelocity().vz;
	}
	else
	{*/
		return -1;
	//}
}

float Sensors::getBatteryLevel()
{
	ASSERT_ARDRONE_F(d)
	SIMULATE_INPUT_F(sim, "Please enter a simulated value for the battery level of the drone", "%", ssui)

	//TODO: return d->drone_getBatteryStatus();
	return 0;
}

//////////// UTILITY FUNCTIONS ////////////

Util::Util(std::shared_ptr<Drone> drone, bool simulationMode, IScriptSimulationUI *simulationUI)
{
	d = drone;
	sim = simulationMode;
	ssui = simulationUI;
}

bool Util::isConnected()
{
	ASSERT_ARDRONE_B(d)
	SIMULATE_INPUT_B(sim, "Is the drone connected to the computer?", ssui)

	return d->isConnected();
}

bool Util::isFlying()
{
	ASSERT_ARDRONE_B(d)
	SIMULATE_INPUT_B(sim, "Is the drone flying?", ssui)

	//TODO: return d->drone_isFlying();
	return false;
}

bool Util::startRecording()
{
	ASSERT_ARDRONE_B(d)
	SIMULATE_ACTION_B(sim, "Started recording", ssui)

	//TODO: return d->drone_startRecording();
	return false;
}

bool Util::stopRecording()
{
	ASSERT_ARDRONE_B(d)
	SIMULATE_ACTION_B(sim, "Stopped recording", ssui)

	//TODO: return d->drone_stopRecording();
	return false;
}

bool Util::flatTrim()
{
	ASSERT_ARDRONE_B(d)
	SIMULATE_ACTION_B(sim, "Calibrating gyroscope", ssui)

	//TODO: return d->drone_flattrim();
	return false;
}

bool Util::calibrateMagnetometer()
{
	ASSERT_ARDRONE_B(d)
	SIMULATE_ACTION_B(sim, "Calibrating magnetometer", ssui)

	//TODO: return d->drone_calibmagneto();
	return false;
}

bool Util::changeView(std::string view)
{
	ASSERT_ARDRONE_B(d)
	SIMULATE_ACTION_B(sim, std::string("Changing camera to ").append(view), ssui)

	//TODO: this
	/*
	if(view == "FRONT")
	{
		return d->drone_changeView(ardrone::camera::FRONT);
	}
	else if(view == "BOTTOM")
	{
		return d->drone_changeView(ardrone::camera::BOTTOM);
	}
	else
	{*/
		return false;
	//}
}

bool Util::savePicture(std::string path)
{
	ASSERT_ARDRONE_B(d)
	SIMULATE_ACTION_B(sim, "Took picture", ssui)

	//TODO: return d->drone_takePicture(path);
	return false;
}

//////////// HARDWARE EXTENSION RELATED FUNCTIONS ////////////

HWExt::HWExt(std::shared_ptr<Drone> drone, bool simulationMode, IScriptSimulationUI *simulationUI)
{
	d = drone;
	sim = simulationMode;
	ssui = simulationUI;
}
