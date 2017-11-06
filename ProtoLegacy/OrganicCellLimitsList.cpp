#include "stdafx.h"
#include <stdio.h>
#include <iostream>
#include "OrganicCellLimitsList.h"
#include "OrganicSystem.h"

void OrganicCellLimitsList::setInitialOrganicCellLimits(OrganicSystem* in_organicPtr)
{
	/* Summary: this functions sets up the cellLimits for all modes of operation (current modes are 0, 1, 2, 3) */

	organicSystemPtr = in_organicPtr;							// set the OrganicSystem pointer equal to the OrganicSystem the called this function
	maxCellLimit = organicSystemPtr->OCList.numberOfCells;		// set the cell limit equal to the number of cells that were added into OCList

	// 4 core processor settings
	if (maxCellLimit == 3)
	{

	}
}