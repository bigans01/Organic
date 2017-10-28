#include "stdafx.h"
#include <stdio.h>
#include <iostream>
#include <chrono>
#include "OrganicCellManager.h"

void OrganicCellManager::AssignT1TerrainCells()
{
	std::map<int, OrganicCell*>::iterator availableCellMapIter = availableCellMap.begin();	// set up an iterator for looping
	std::vector<int> availablesToErase;

	// first, copy the values into the terrain cell map
	for (availableCellMapIter; availableCellMapIter != availableCellMap.end(); ++availableCellMapIter)
	{
		int cellID = availableCellMapIter->first;					// get the OrganicCell's ID
		//OrganicCell* cellReference = availableCellMapIter->second;	// get the direct reference to the cell
		terrainCellMap[cellID] = availableCellMap[cellID];			// copy the value into terrainCellMap
		availablesToErase.push_back(cellID);						// add the element to the eraseable list
	}

	// now, erase from the available cell map
	for (std::vector<int>::iterator availablesToEraseIter = availablesToErase.begin(); availablesToEraseIter != availablesToErase.end(); ++availablesToEraseIter)
	{
		availableCellMap.erase(*availablesToEraseIter);	// erase the value
	}
	cout << ">>>> Cell Assignment complete..." << endl;
}

void OrganicCellManager::ReleaseT1TerrainCells()
{
	std::map<int, OrganicCell*>::iterator terrainCellMapIter = terrainCellMap.begin();
	std::vector<int> terrainCellsToErase;

	// first, copy the values into the available cell map
	for (terrainCellMapIter; terrainCellMapIter != terrainCellMap.end(); ++terrainCellMapIter)
	{
		int cellID = terrainCellMapIter->first;
		availableCellMap[cellID] = terrainCellMap[cellID];
		terrainCellsToErase.push_back(cellID);
	}

	for (std::vector<int>::iterator terrainCellsToEraseIter = terrainCellsToErase.begin(); terrainCellsToEraseIter != terrainCellsToErase.end(); ++terrainCellsToEraseIter)
	{
		terrainCellMap.erase(*terrainCellsToEraseIter);	// erase the value
	}
	cout << ">>>> Terrain cells successfully released..." << endl;
}