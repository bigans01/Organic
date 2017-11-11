#include "stdafx.h"
#include <stdio.h>
#include <iostream>
#include "OrganicCellLimitsList.h"
#include "OrganicCellManager.h"
#include "OrganicSystem.h"

void OrganicCellLimitsList::setInitialOrganicCellLimits(OrganicSystem* in_organicPtr, OrganicCellManager* in_organicCellManagerPtr)
{
	/* Summary: this functions sets up the cellLimits for all modes of operation (current modes are 0, 1, 2, 3) */

	organicSystemPtr = in_organicPtr;							// set the OrganicSystem pointer equal to the OrganicSystem that called this function
	organicCellManagerPtr = in_organicCellManagerPtr;			// set the OrganicCellManager ptr equal to the OrganicSystem that called this function
	maxCellLimit = organicSystemPtr->OCList.numberOfCells;		// set the cell limit equal to the number of cells that were added into OCList

	// 4 core processor settings
	if (maxCellLimit == 3)
	{
		// priority 0 values =  balanaced			(1 type 1 cell, 2 type 2 cells)
		cellLimitsArray[0].limitArray[0] = 1;	// type 1 terrain cell count
		cellLimitsArray[0].limitArray[1] = 2;	// type 2 terrain cell count
		cellLimitsArray[0].limitArray[2] = 0;	// type 3 terrain cell count
		cellLimitsArray[0].limitArray[3] = 0;	// type 4 terrain cell count
		cellLimitsArray[0].limitArray[4] = 0;	// logic cells
		cellLimitsArray[0].limitArray[5] = 0;	// maintenance cells
		cellLimitsArray[0].limitArray[6] = 0;	// unused
		cellLimitsArray[0].limitArray[7] = 0;	// unused

		// priority 1 values = focus on T2 terrain	(3 type 2 cells)
		cellLimitsArray[1].limitArray[0] = 0;	// type 1 terrain cell count
		cellLimitsArray[1].limitArray[1] = 3;	// type 2 terrain cell count
		cellLimitsArray[1].limitArray[2] = 0;	// type 3 terrain cell count
		cellLimitsArray[1].limitArray[3] = 0;	// type 4 terrain cell count
		cellLimitsArray[1].limitArray[4] = 0;	// logic cells
		cellLimitsArray[1].limitArray[5] = 0;	// maintenance cells
		cellLimitsArray[1].limitArray[6] = 0;	// unused
		cellLimitsArray[1].limitArray[7] = 0;	// unused

		// priority 2 values


		// priority 3 values
	}
}

void OrganicCellLimitsList::changeToPriority(int in_priority, int in_oldPriority)	// change to a new priority
{
	// settings for a limit of 3 cells
	if (maxCellLimit == 3)
	{
		if (in_priority == 1)	// new priority to change to is 1
		{
			if (in_oldPriority == 0)	// old priority to change from is 0
			{
				organicCellManagerPtr->reAssignCell_T1toT2();	// move a single T1 cell to the T2 availability pool
			}
		}
		else if (in_priority == 0)	// new priority to change to is 0
		{
			if (in_oldPriority == 1)	// old priority to change from is 1
			{
				organicCellManagerPtr->reAssignCell_T2toT1();	// move a single T2 cell to the T1 availability pool
			}
		}
	}
}