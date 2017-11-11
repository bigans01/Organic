#include "stdafx.h"
#include <stdio.h>
#include <iostream>
#include <chrono>
#include "OrganicCellManager.h"
#include "OrganicSystem.h"
#include "OrganicCellLimitsList.h"

/*
void OrganicCellManager::AssignT1TerrainCells()
{
	std::map<int, OrganicCell*>::iterator availableCellMapIter = availableCellMap.begin();	// set up an iterator for looping
	std::vector<int> availablesToErase;

	// first, copy the values into the terrain cell map
	for (availableCellMapIter; availableCellMapIter != availableCellMap.end(); ++availableCellMapIter)
	{
		int cellID = availableCellMapIter->first;					// get the OrganicCell's ID
		//OrganicCell* cellReference = availableCellMapIter->second;	// get the direct reference to the cell
		t2CellMap[cellID] = availableCellMap[cellID];			// copy the value into terrainCellMap
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
	std::map<int, OrganicCell*>::iterator terrainCellMapIter = t2CellMap.begin();
	std::vector<int> terrainCellsToErase;

	// first, copy the values into the available cell map
	for (terrainCellMapIter; terrainCellMapIter != t2CellMap.end(); ++terrainCellMapIter)
	{
		int cellID = terrainCellMapIter->first;
		availableCellMap[cellID] = t2CellMap[cellID];
		terrainCellsToErase.push_back(cellID);
	}

	for (std::vector<int>::iterator terrainCellsToEraseIter = terrainCellsToErase.begin(); terrainCellsToEraseIter != terrainCellsToErase.end(); ++terrainCellsToEraseIter)
	{
		t2CellMap.erase(*terrainCellsToEraseIter);	// erase the value
	}
	cout << ">>>> Terrain cells successfully released..." << endl;
}
*/

void OrganicCellManager::populateCellMapsOnEngineStart()
{
	// if the cell limit is 3, do the following
	if (organicCellLimitsListPtr->maxCellLimit == 3)
	{
		// if the priority on start up is 0, do the following
		if (organicSystemPtr->workPriority == 0)
		{
			std::map<int, OrganicCell*>::iterator availableCellMapIter = availableCellMap.begin();		// start at beginning of available Cells
			std::vector<int> availablesToErase;															// used to remove from available cells

			// assign 1 T1 cell
			int numberOfT1Cells = organicCellLimitsListPtr->cellLimitsArray[0].limitArray[0];		// get the number of T1 cells to allocate
			for (int x = 0; x < numberOfT1Cells; x++)
			{
				int cellID = availableCellMapIter->first;					// get the OrganicCell's ID
				t1CellMap[cellID] = availableCellMap[cellID];			// copy the value into terrainCellMap
				availablesToErase.push_back(cellID);						// add the element to the eraseable list
				++availableCellMapIter;
			}

			// assign 2 T2 cells
			int numberOFT2Cells = organicCellLimitsListPtr->cellLimitsArray[0].limitArray[1];		// get the number of T2 cells to allocate
			for (int x = 0; x < numberOFT2Cells; x++)
			{
				int cellID = availableCellMapIter->first;					// get the OrganicCell's ID
				t2CellMap[cellID] = availableCellMap[cellID];			// copy the value into terrainCellMap
				availablesToErase.push_back(cellID);						// add the element to the eraseable list
				++availableCellMapIter;
			}

			for (std::vector<int>::iterator availablesToEraseIter = availablesToErase.begin(); availablesToEraseIter != availablesToErase.end(); ++availablesToEraseIter)
			{
				availableCellMap.erase(*availablesToEraseIter);	// erase the value
			}
			cout << ">>>> Cell Assignment on engine start complete, (assigned with priority " << organicSystemPtr->workPriority << ")" << endl;

		}
	}
}

void OrganicCellManager::reAssignCell_T1toT2()
{
	/* Summary: transforms a single cell to be of type T2, from the T1 list */
	std::map<int, OrganicCell*>::iterator t1CellMapIter = t1CellMap.begin();	// get an iterator for the element at the beginning of the t1 cell map 
	int cellID = t1CellMapIter->first;		// get the ID (key) of the first element in the t1 map
	t2CellMap[cellID] = t1CellMap[cellID];	// make the t2 cell map element equal to the t1 element
	t1CellMap.erase(cellID);				// erase the element from the T1 map
}

void OrganicCellManager::reAssignCell_T2toT1()
{
	/* Summary: transforms a single cell to be of type T1, from the T2 list */
	std::map<int, OrganicCell*>::iterator t2CellMapIter = t2CellMap.begin();	// get an iterator for the element at the beginning of the t2 cell map 
	int cellID = t2CellMapIter->first;		// get the ID (key) of the first element in the t2 map
	t1CellMap[cellID] = t2CellMap[cellID];	// make the t1 cell map element equal to the t2 element
	t2CellMap.erase(cellID);				// erase the element from the T2 map
}

void OrganicCellManager::setOrganicCellLimitsListPtr(OrganicCellLimitsList* in_OrganicCellLimitsListPtr)
{
	organicCellLimitsListPtr = in_OrganicCellLimitsListPtr;
}

void OrganicCellManager::setOrganicSystemPtr(OrganicSystem* in_organicSystemPtr)
{
	organicSystemPtr = in_organicSystemPtr;
}