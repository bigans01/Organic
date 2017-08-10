#pragma once

// activate guards
#ifndef MDLISTJOBMATERIALIZECOLLECTION_H
#define MDLISTJOBMATERIALIZECOLLECTION_H

#include <unordered_map>
#include "MDJobMaterializeCollection.h"

class MDListJobMaterializeCollection
{
	public:
		std::unordered_map<EnclaveKeyDef::EnclaveKey, MDJobMaterializeCollection, EnclaveKeyDef::KeyHasher> ListMatrix;
};

#endif