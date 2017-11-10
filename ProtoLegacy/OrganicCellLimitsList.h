/*------------------------------------------------------------------------------------------

--OrganicCellLimitsList.h	(Last Update 11/4/2017)

Description: header file for OrganicCellLimitsList.h

Summary: An OrganicCellLimitsList stores one or more OrganicCellLimits, one per mode.

------------------------------------------------------------------------------------------*/

#pragma once

#ifndef ORGANICCELLLIMITSLIST_H
#define ORGANICCELLLIMITSLIST_H

#include "OrganicCellLimits.h"

class OrganicSystem;
class OrganicCellManager;
class OrganicCellLimitsList
{
public:
	OrganicCellManager* organicCellManagerPtr;	// pointer to the OrganicCellManager in OrganicSystem
	OrganicSystem* organicSystemPtr;		// a pointer to the OrganicSystem that this instance of OrganicCellLimits belongs to (used to reference workPriority in the OrganicSystem)
	int maxCellLimit;						// the max number of cells that will be allowed
	OrganicCellLimits cellLimitsArray[4];	// contains cell limits for 4 different modes: 0, 1, 2, and 3.
	void setInitialOrganicCellLimits(OrganicSystem* in_organicPtr, OrganicCellManager* in_organicCellManagerPtr); // sets the cells based on the number of cores (called once on engine startup)
	void changeToPriority(int in_newPriority, int in_oldPriority);	// change to a new priority
};

#endif
