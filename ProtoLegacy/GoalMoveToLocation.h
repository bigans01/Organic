#pragma once

// adding guards 
#ifndef GOALMOVETOLOCATION_H
#define GOALMOVETOLOCATION_H

#include "Movegoal.h"
//#include "BasicEntity.h"

class BasicEntity; // forward declarations from BasicEntity.h file

class GoalMoveToLocation : public Movegoal {
	void Movegoal::calculate(BasicEntity* entity)
	{
		// define custom code for this function here...
	}

};

#endif