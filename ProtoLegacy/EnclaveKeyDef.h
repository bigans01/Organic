/*------------------------------------------------------------------------------------------

--EnclaveKeyDef.h		(Last update 7/7/2017)

Description: Header file for EnclaveKeyDef.cpp.

Summary: EnclaveKeyDef.h supplies the definition that will be used for the key values in any type of collection. The Key is made up of a x/y/z coordinate, and is meant to be put 
into an unordered map. A hash for this is then constructed, in accordance with the rules for an unordered map. Examples of classes that use this are:

-EnclaveCollectionMatrix (member variable: EnclaveCollectionMap)
-ManifestCollectionMatrix (member variable: ManiCollectionMap)
-RenderCollectionMatrix (member variable: RenderMatrix)

Dependents: none


------------------------------------------------------------------------------------------*/

#pragma once

#ifndef ENCLAVEKEY_H
#define ENCLAVEKEY_H

#include <iostream>

class EnclaveKeyDef {

public:
	// forms the structure of the Key itself.
	struct EnclaveKey
	{
		int x = 0;
		int y = 0;
		int z = 0;

		// operator overloading (required)
		bool operator==(const EnclaveKey &other) const
		{
			return (x == other.x
				&&
				y == other.y
				&&
				z == other.z);
		}
	};

	struct Enclave2DKey			// for use with EnclaveUnveilMeta only (subject to change later)
	{
		int a = 0;
		int b = 0;
	};


	struct KeyHasher
	{
		// hash generation function, required to be specified when put into an unordered map. 
		std::size_t operator()(const EnclaveKey& k) const
		{
			using std::size_t;
			using std::hash;
			using std::string;
			return (
					(hash<int>()(k.x)
				^ (hash<int>()(k.y) << 1)
						) >> 1)
				^ (hash<int>()(k.z) << 1);
		}
	};
};

#endif