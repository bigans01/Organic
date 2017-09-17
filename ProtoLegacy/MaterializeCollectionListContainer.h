/*------------------------------------------------------------------------------------------

--MaterializeCollectionListContainer.h (Last update 9/15/2017)

Description: Header file for MaterializeCollectionListContainer data structure

Summary: A MaterializeCollectionListContainer contains a vector of MDListJobMaterializeCollections. It is designed to be used with multithreading; one thread gets an instance of this list
so that it knows all collections it needs to materialize.


------------------------------------------------------------------------------------------*/

#pragma once

#ifndef MATERIALIZECOLLECTIONLISTCONTAINER_H
#define MATERIALIZECOLLECTIONLISTCONTAINER_H

#include <vector>
#include "MDListJobMaterializeCollection.h"

class  MaterializeCollectionListContainer
{
public:
	std::vector<MDListJobMaterializeCollection> MaterializeCollectionList;	// the vector which contains the lists
};

#endif

