/*------------------------------------------------------------------------------------------

--OrganicCellLists.h	(Last Update 11/4/2017)

Description: header file for OrganicCellLists.h

Summary: An OrganicCellLimits stores the current number of threads (Cells) that may be allocated to any specific type of task, based on the workPriority; it contains a set of values for each available work mode.

------------------------------------------------------------------------------------------*/

#pragma once

#ifndef ORGANICCELLLIMITS_H
#define ORGANICCELLLIMITS_H

#include <thread>

class OrganicSystem;
class OrganicCellManager;
class OrganicCellLimitsList;
class OrganicCellLimits
{
private:
	friend class OrganicCellManager;
	friend class OrganicCellLimitsList;
	OrganicSystem* organicSystemPtr;	// a pointer to the OrganicSystem that this instance of OrganicCellLimits belongs to (used to reference workPriority in the OrganicSystem)
	int maxNumberOfCells;			// the max number of cells that can possibly run during the engine's life (equal to number of possible CPU threads -1)
	unsigned char limitArray[8];	// an array of 4 "limits"
									// index 0 = T1 terrain cells
									// index 1 = T2 terrain cells
									// index 2 = T3 terrain cells
									// index 3 = T4 terrain cells
									// index 4 = logic cells
									// index 5 = maintenance cells
									// index 6 = unused
									// index 7 = unused

};

#endif
