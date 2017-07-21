#pragma once

// adding guards
#ifndef BASICENTITY_H
#define BASICENTITY_H

#include <vector>
#include "Movegoal.h"

using namespace std; // set the namespace

class BasicEntity {


	int health;						// current entity's health
	int mood;						// current entity's mood
	int personality;				// personality
	int hunger;						// hunger level
	int posx, posy, posz;			// x,y,z coords
	vector<Movegoal*> moveGoals;		// will store various move goals

public:
	int getMood() // returns entity's mood
	{
		return mood;
	}

	int getHealth() // returns entity's health
	{
		return health;
	}

	int getPersonality()	// return entity's personality
	{
		return personality;
	}

	vector<Movegoal*> getMoveGoals() // get the entity's current list of goals
	{
		return moveGoals;
	}

	/*void setMoveGoal(GoalMoveToLocation goalName)
	{
		moveGoals.push_back(goalName); // add a new goal to the entity (look for food, etc)
	}*/

	template <class T, class U> void setMoveGoal2(T param, U param2) // give ability to set *any* type of move goal
	{
		U.push_back(param);
		//return 1;
	}
};

#endif