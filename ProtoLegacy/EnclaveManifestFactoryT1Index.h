/*------------------------------------------------------------------------------------------

--EnclaveManifestFactoryT1Index.h	(Last Update 8/11/2017)

Description: Header file for EnclaveManifestFactoryT1.cpp

Summary: A EnclaveManifestFactoryIndex is a container that contains one or more instances of EnclaveManifestFactoryT1. Only one thread should be "leasing" a factory at a given point in time.

Dependents: a valid instance of ManifestCollectionMatrix

------------------------------------------------------------------------------------------*/

#pragma once

#ifndef ENCLAVEMANIFESTFACTORYT1INDEX_H
#define ENCLAVEMANIFESTFACTORYT1INDEX_H

#include "stdafx.h"
#include <stdio.h>
#include <iostream>
#include <unordered_map>
#include <EnclaveManifestFactoryT1.h>

class OrganicSystem;
class EnclaveManifestFactoryT1Index
{
public:
	friend class OrganicSystem;
	std::unordered_map<int, EnclaveManifestFactoryT1> FactoryMap;		// unordered_map that contains instances of EnclaveManifestFactoryT1
};

#endif
