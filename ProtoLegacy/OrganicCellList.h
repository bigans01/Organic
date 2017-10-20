/*------------------------------------------------------------------------------------------

--OrganicCellList.h	(Last Update 10/19/2017)

Description: Header file for OrganicCellList.cpp

Summary: An OrganicCellList is a vector that contains one or more OrganicCell instances and an integer that keeps track of how many active threads there are

------------------------------------------------------------------------------------------*/

#pragma once

#include <map>
#include "OrganicCell.h"

#ifndef ORGANICCELLIST_H
#define ORGANICCELLIST_H

class OrganicCellList
{
public:
	int numberOfCells = 0;
	std::map<int, OrganicCell> cellList;
};

#endif
