/*------------------------------------------------------------------------------------------

--OrganicCellManager.h	(Last Update 10/27/2017)

Description: header file for OrganicCellManager.cpp

Summary: An OrganicCellManager is responsible for determining what type of work each OrganicCell can be working on during this game loop tick.

------------------------------------------------------------------------------------------*/

#pragma once

#ifndef ORGANICCELLMANAGER_H
#define ORGANICCELLMANAGER_H

#include "OrganicCell.h"
#include <map>

class OrganicCellLimitsList;
class OrganicSystem;
class OrganicCellManager
{
	//void AssignT1TerrainCells();			// attempts to assign cells as T1 terrain processing cells
	//void ReleaseT1TerrainCells();			// releases t1 terrain cells and puts them back into the available pool

private:
	friend class OrganicSystem;
	friend class OrganicCellLimitsList;
	std::map<int, OrganicCell*> availableCellMap;		// a map containing available cells
	std::map<int, OrganicCell*> t2CellMap;				// a map containing all worker threads that can work as T2 terrain cells
	std::map<int, OrganicCell*> t1CellMap;				// a map containing all worker threads that can work as T1 terrain cells
	OrganicCellLimitsList* organicCellLimitsListPtr = NULL;	// a pointer to the OrganicSystem's instance of OrganicCellLimitsList
	OrganicSystem* organicSystemPtr = NULL;					// a pointer ot the OrganicSystem that this instance belongs to
	void setOrganicCellLimitsListPtr(OrganicCellLimitsList* in_OrganicCellLimitsListPtr);
	void setOrganicSystemPtr(OrganicSystem* in_organicSystemPtr);
	void populateCellMapsOnEngineStart();
	void reAssignCell_T1toT2();
	void reAssignCell_T2toT1();
};


#endif
