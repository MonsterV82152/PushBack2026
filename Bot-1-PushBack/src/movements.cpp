#ifndef MOVEMENTS_CPP
#define MOVEMENTS_CPP

#include "globals.hpp"
#include "AutonSelector.hpp"

namespace rollers
{
	/**
	 * @brief A namespace for controlling the roller motors
	 * A set of speeds for the roller motors to achieve different functions
	 */
	struct rollerState
	{
		std::string name;
		double bottomSpeed;
		double middleSpeed;
		double topSpeed;
		double bucketSpeed;
	};
	/**
	 * @brief A temporary roller state that can override the main roller state
	 * with a set importance level
	 */
	struct temporaryRollerState
	{
		std::string name;
		double bottomSpeed;
		double middleSpeed;
		double topSpeed;
		double bucketSpeed;
		int importance; /// Importance level of the temporary state
	};
	inline rollerState state = {"none", 0, 0, 0, 0};		///< Current main roller state
	inline rollerState currentState = {"none", 0, 0, 0, 0}; ///< Currently active roller state (could be temporary)

	/// A list of predefined roller states
	inline std::vector<rollerState> rollerStates = {
		//
		{"intake", 127, 127, 127, 0},
		{"intakeC", 127, 127, 127, 0},
		{"outtake", -127, -127, -127, 0},
		{"scoreBottom", -60, -60, -127, 127},
		{"scorePark", -70, 0, 0, 70},
		{"scorePark2", -45, 0, 0, 45},
		{"scoreTopAuton", 90, 90, -90, 127},
		{"scoreMiddle", 50, -50, 127, 127},
		{"scoreMiddleAuton", 90, -90, 127, 127},

		{"scoreMiddleC", 90, -90, 90, 0},
		{"scoreTop", 127, 127, -127, 127},
		{"scoreTopC", 90, 90, -90, 0},
		{"scoreTopCT", 90, 90, -90, 0},
		{"directIntake", 127, 0, 0, -127},
		{"cycle", 127, 127, 127, 127},
		{"cycleC", 127, 127, 127, 127},
		{"cycleCT", 0, 127, 127, 127},
		{"none", 0, 0, 0, 0},
		{"reverseTop", 0, 0, -127, 0},

		{"clearIntake", -127, -127, -127, 0},
		{"clearIntakeC", -127, -127, -127, 0},

		{"ejectMiddle", 127, -127, 0, 127}};
	inline std::vector<temporaryRollerState> temporaryRollerStates;
	inline temporaryRollerState currentTemporaryState = {"none", 0, 0, 0, 0, 10};
	inline temporaryRollerState _stateToTemp(rollerState state, int importance = 0)
	{
		return temporaryRollerState{
			state.name,
			state.bottomSpeed,
			state.middleSpeed,
			state.topSpeed,
			state.bucketSpeed,
			importance};
	}
	/**
	 * @brief Sets the roller state based on the provided state name
	 * @param newState The name of the roller state to set
	 */
	inline void setState(std::string newState)
	{
		// Searches for the roller state by name
		for (const auto &rollerState : rollerStates)
		{
			// Checks if the current roller state matches the new state
			if (rollerState.name == newState)
			{
				// If no temporary states are active, it sets the motor speeds
				if (temporaryRollerStates.empty())
				{
					bottom.move(rollerState.bottomSpeed);
					middle.move(rollerState.middleSpeed);
					top.move(rollerState.topSpeed);
					bucket.move(rollerState.bucketSpeed);
				}
				// Updates the current and main roller states
				currentState = rollerState;
				state = rollerState;
				return;
			}
		}
		// If the state is not found, it logs an error
		std::cerr << "Invalid roller state: " << newState << std::endl;
	}
	inline void setState(rollerState newState)
	{
		for (const auto &rollerState : rollerStates)
		{
			if (rollerState.name == newState.name)
			{
				bottom.move(newState.bottomSpeed);
				middle.move(newState.middleSpeed);
				top.move(newState.topSpeed);
				bucket.move(newState.bucketSpeed);
				state = newState;
				currentState = newState;
				return;
			}
		}
		rollerStates.push_back(newState);
		bottom.move(newState.bottomSpeed);
		middle.move(newState.middleSpeed);
		top.move(newState.topSpeed);
		bucket.move(newState.bucketSpeed);
		state = newState;
		currentState = newState;
	}
	inline void _runLowestTemporaryState()
	{
		if (temporaryRollerStates.empty())
		{
			bottom.move(state.bottomSpeed);
			middle.move(state.middleSpeed);
			top.move(state.topSpeed);
			bucket.move(state.bucketSpeed);
			currentState = state;
			return;
		}
		temporaryRollerState lowest = temporaryRollerStates[0];
		for (const auto &tempState : temporaryRollerStates)
		{
			if (tempState.importance <= lowest.importance)
			{
				lowest = tempState;
			}
		}
		currentTemporaryState = lowest;
		currentState.name = lowest.name;
		currentState.bottomSpeed = lowest.bottomSpeed;
		currentState.middleSpeed = lowest.middleSpeed;
		currentState.topSpeed = lowest.topSpeed;
		currentState.bucketSpeed = lowest.bucketSpeed;
		bottom.move(lowest.bottomSpeed);
		middle.move(lowest.middleSpeed);
		top.move(lowest.topSpeed);
		bucket.move(lowest.bucketSpeed);
	}
	/**
	 * @brief Adds a temporary roller state that overrides the main state
	 * @param newState The name of the temporary roller state to add
	 * @param importance The importance level of the temporary state
	 */
	inline void addTemporaryState(std::string newState, int importance)
	{
		// Searches for the roller state by name
		for (const auto &rollerState : rollerStates)
		{
			// Checks if the current roller state matches the new state
			if (rollerState.name == newState)
			{
				// Adds the temporary state to the list
				temporaryRollerStates.push_back(_stateToTemp(rollerState, importance));
				break;
			}
		}
		// Runs the temporary state with the highest importance
		_runLowestTemporaryState();
	}

	/**
	 * @brief Adds a temporary roller state that overrides the main state
	 * @param newState The temporary roller state to add
	 * @param importance The importance level of the temporary state
	 */
	inline void addTemporaryState(rollerState newState, int importance)
	{
		// Adds the temporary state to the list
		temporaryRollerStates.push_back(_stateToTemp(newState, importance));
		// Runs the temporary state with the highest importance
		_runLowestTemporaryState();
		// Ensures the roller state exists in the main roller states list
		for (const auto &rollerState : rollerStates)
		{
			if (rollerState.name == newState.name)
			{
				return;
			}
		}
		// If not found, it adds the new state to the main list
		rollerStates.push_back(newState);
	}
	inline void removeTemporaryState(std::string stateName)
	{
		for (auto it = temporaryRollerStates.begin(); it != temporaryRollerStates.end(); ++it)
		{
			if (it->name == stateName)
			{
				temporaryRollerStates.erase(it);
				break;
			}
		}
		_runLowestTemporaryState();
	}
	inline bool findState(std::string a)
	{
		if (state.name == a)
		{
			return true;
		}
		for (const auto &tempState : temporaryRollerStates)
		{
			if (tempState.name == a)
			{
				return true;
			}
		}
		return false;
	}
	inline temporaryRollerState findLowestState(int importance = 0)
	{
		for (temporaryRollerState i : temporaryRollerStates)
		{
			if (i.importance >= importance)
			{
				return i;
			}
		}
		return temporaryRollerState{state.name, state.bottomSpeed, state.middleSpeed, state.topSpeed, state.bucketSpeed, 0};
	}

	inline void removeAllTemporaryState()
	{
		temporaryRollerStates.clear();
		_runLowestTemporaryState();
	}

	inline void intake()
	{
		setState("intake");
	}
	inline void outtake()
	{
		setState("outtake");
	}
	inline void stop()
	{
		setState("none");
	}
	inline void scoreMiddle()
	{
		setState("scoreMiddle");
	}
	inline void scoreTop()
	{
		setState("scoreTop");
	}
	inline void scoreBottom()
	{
		setState("scoreBottom");
	}
}

namespace colourSort
{
	struct timeout
	{
		int cycleCount;
		std::string name;
	};
	inline std::atomic<bool> redTeam(true);
	inline std::atomic<bool> on(true);
	inline std::atomic<bool> sortML(false);
	inline void start(void *params)
	{
		double redMax = 40;
		double redMin = 350;
		double blueMax = 240;
		double blueMin = 170;
		short currentState = 0;

		std::vector<timeout> timeouts = {};
		std::vector<timeout> starts = {};
		int cycleCount = 0;

		topColour.set_led_pwm(100);
		// bottomColor.set_led_pwm(100);
		while (true)
		{
			if (master.get_digital_new_press(buttons::RIGHT))
			{
				rollers::setState("scorePark");
				pros::delay(500);
				while (bottomDS.get_distance() >= 60)
				{
					pros::delay(10);
				}
				rollers::setState("scorePark2");
				while (bottomDS.get_distance() <= 60)
				{
					pros::delay(10);
				}
				// pros::delay(100);
				// rollers::setState("intakeC");
				// pros::delay(50);
				rollers::setState("none");
				park.setState(true);
			}
			std::string currentRollerState = rollers::findLowestState(2).name;
			cycleCount++;
			for (int i = 0; i < timeouts.size(); i++)
			{
				if (timeouts[i].cycleCount == cycleCount)
				{
					rollers::removeTemporaryState(timeouts[i].name);
					timeouts.erase(timeouts.begin() + i);
				}
			}
			for (int i = 0; i < starts.size(); i++)
			{
				if (starts[i].cycleCount == cycleCount)
				{
					rollers::addTemporaryState(starts[i].name, 1);
					starts.erase(starts.begin() + i);
				}
			}
			// Top sensor triggers colour sensor to read next block - fixes the ghost block issue
			if (rollers::currentState.name != "none" && on.load() && (!autonSelect.isSkills() && !matchLoader.getState() || autonSelect.isSkills()))
			{
				double middleColor = bottomColor.get_hue();
				double topColor = topColour.get_hue();
				if (((topColor > redMin || topColor < redMax) && !redTeam.load()) || ((topColor > blueMin && topColor < blueMax) && redTeam.load()))
				{
					if (currentState == 0)
					{
						if (autonSelect.isSkills())
						{
							if (currentRollerState == "scoreTop")
							{
								rollers::addTemporaryState("cycleC", 1);
								timeouts.push_back(timeout{cycleCount + 10, "cycleC"});
							}
							else if ((currentRollerState == "intake" || currentRollerState == "intake") && !matchLoader.getState())
							{
								rollers::addTemporaryState("scoreTopC", 1);
								timeouts.push_back(timeout{cycleCount + 10, "scoreTopC"});
							}
							else if (currentRollerState == "intake" && matchLoader.getState() && sortML.load())
							{
								currentState = 1;
								rollers::addTemporaryState("reverseTop", 1);
								timeouts.push_back(timeout{cycleCount + 5, "reverseTop"});
								starts.push_back(timeout{cycleCount + 5, "none"});
								timeouts.push_back(timeout{cycleCount + 40, "none"});
								starts.push_back(timeout{cycleCount + 40, "scoreTopC"});
								timeouts.push_back(timeout{cycleCount + 140, "scoreTopC"});
							}
						}
						else
						{
							if (currentRollerState == "intake")
							{
								rollers::addTemporaryState("scoreTopC", 1);
								timeouts.push_back(timeout{cycleCount + 10, "scoreTopC"});
							}
							else if (currentRollerState == "scoreTop")
							{
								rollers::addTemporaryState("cycleC", 1);
								timeouts.push_back(timeout{cycleCount + 10, "cycleC"});
							}
						}
					}
				}
				else
				{
					currentState = 0;
				}

				//     if (((middleColor > redMin || middleColor < redMax) && !redTeam.load()) || ((middleColor > blueMin && middleColor < blueMax) && redTeam.load()))
				//     {
				//         if (autonSelect.isSkills())
				//         {
				//             if (currentRollerState == "scoreMiddle")
				//             {
				//                 rollers::addTemporaryState("cycleCT", 1);
				//                 timeouts.push_back(timeout{cycleCount + 15, "cycleCT"});
				//             }
				//             else if ((currentRollerState == "intake" || currentRollerState == "intake") && !matchLoader.getState())
				//             {
				//                 rollers::addTemporaryState("scoreMiddleC", 1);
				//                 timeouts.push_back(timeout{cycleCount + 15, "scoreMiddleC"});
				//             }
				//         }
				//         else
				//         {
				//             if (currentRollerState == "intake")
				//             {
				//                 rollers::addTemporaryState("scoreMiddleC", 1);
				//                 timeouts.push_back(timeout{cycleCount + 15, "scoreMiddleC"});
				//             }
				//             else if (currentRollerState == "scoreMiddle")
				//             {
				//                 rollers::addTemporaryState("cycleCT", 1);
				//                 timeouts.push_back(timeout{cycleCount + 15, "cycleCT"});
				//             }
				//         }
				//     }
			}
			pros::delay(10);
		}
	}

}

#endif