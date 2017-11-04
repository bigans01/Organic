#include "stdafx.h"
#include <stdio.h>
#include <iostream>
#include "OrganicCellLimits.h"
#include "OrganicSystem.h"

void OrganicCellLimits::setInitialOrganicCellLimits(OrganicSystem* in_organicPtr)
{
	/* Summary: initialize the cell limits when engine starts, based on number of cores in CPU*/
	maxNumberOfCells = std::thread::hardware_concurrency() - 1;		// get the maximum number of cells
	organicSystemPtr = in_organicPtr;

	std::cout << "Max number of cells allowed: " << maxNumberOfCells << std::endl;
	std::cout << "Current priority mode: " << organicSystemPtr->workPriority << std::endl;
	if (maxNumberOfCells == 3)
	{
		limitArray[0] = 1;	// set the T1 limit
		limitArray[1] = 2;	// set the T2 limit
	}
	else
	{
		std::cout << "WARNING...Organic is not supported on your operating system!! (not enough cores)" << std::endl;
	}
}