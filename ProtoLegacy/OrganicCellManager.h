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

class OrganicCellManager
{
public:
	std::map<int, OrganicCell*> availableCellMap;		// a map containing available cells
	std::map<int, OrganicCell*> t2CellMap;				// a map containing all worker threads that can work as T2 terrain cells
	std::map<int, OrganicCell*> t1CellMap;				// a map containing all worker threads that can work as T1 terrain cells
	int terrainT1CellLimit = 2;

	void AssignT1TerrainCells();			// attempts to assign cells as T1 terrain processing cells
	void ReleaseT1TerrainCells();			// releases t1 terrain cells and puts them back into the available pool
};


#endif
