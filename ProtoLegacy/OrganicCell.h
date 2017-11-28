/*------------------------------------------------------------------------------------------

--OrganicCell.h	(Last Update 10/19/2017)

Description: Header file for OrganicCell.cpp

Summary: An OrganicCell is a data structure that contains a pointer to a "thread_pool" and an integer that keeps track of the status of this thread

------------------------------------------------------------------------------------------*/

#pragma once

#ifndef ORGANICCELL_H
#define ORGANICCELL_H

#include "thread_pool.h"
#include "EnclaveManifestFactoryT1.h"



class OrganicCell
{
public:
	int cellStatus = 0;
	thread_pool* threadPtr = NULL;
	EnclaveManifestFactoryT1* factoryPtr = NULL; 

};

#endif