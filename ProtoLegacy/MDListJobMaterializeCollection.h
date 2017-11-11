/*------------------------------------------------------------------------------------------

--MDListJobMaterializeCollection.h (Last update 8/16/2017)

Description: Header file for MDJobMaterializeCollection.h

Summary: A MDListJobMaterializeCollection is a class object that contains all the MDJobMaterializeCollection instances (one for each EnclaveCollection) to be iterated over in a job; a MDListJobMaterializeCollection contains all the
meta data for one particular EnclaveCollection that is needed for one job. An instance of MD

Dependents: a valid instance of a EnclaveCollectionMatrix

------------------------------------------------------------------------------------------*/

#pragma once

// activate guards
#ifndef MDLISTJOBMATERIALIZECOLLECTION_H
#define MDLISTJOBMATERIALIZECOLLECTION_H

#include <unordered_map>
#include "MDJobMaterializeCollection.h"

class OrganicSystem;
class MDListJobMaterializeCollection
{
	private:
		friend class OrganicSystem;
		std::unordered_map<EnclaveKeyDef::EnclaveKey, MDJobMaterializeCollection, EnclaveKeyDef::KeyHasher> ListMatrix; // the unordered_map containing all the MDJobMaterializeCollection instances needed for this job. 
};

#endif