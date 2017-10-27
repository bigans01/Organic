/*------------------------------------------------------------------------------------------

--OrganicCell.h	(Last Update 10/19/2017)

Description: Header file for OrganicCell.cpp

Summary: An OrganicCell is a data structure that contains a pointer to a "thread_pool" and an integer that keeps track of the status of this thread

------------------------------------------------------------------------------------------*/

#pragma once

#include "thread_pool.h"
#include "EnclaveManifestFactoryT1.h"

#ifndef ORGANICCELL_H
#define ORGANICCELL_H

class OrganicCell
{
public:
	int cellStatus;
	thread_pool* threadPtr;
	EnclaveManifestFactoryT1* factoryPtr;

};

#endif