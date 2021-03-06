#include "stdafx.h"
#include <stdio.h>
#include <chrono>
#include "EnclaveCollectionBlueprintMatrix.h"
#include "EnclaveCollectionNeighborList.h"

EnclaveCollectionNeighborList EnclaveCollectionBlueprintMatrix::DetermineBlueprintBordersToRender(EnclaveKeyDef::EnclaveKey blueprintKey, EnclaveCollectionBlueprint* originBlueprint, EnclaveCollectionBorderFlags* borderFlagsPtr, int mode)
{
	std::unordered_map<EnclaveKeyDef::EnclaveKey, EnclaveCollectionBlueprint, EnclaveKeyDef::KeyHasher>::iterator blueprintMapIterator;
	EnclaveKeyDef::EnclaveKey tempKey;
	EnclaveCollectionNeighborList returnList;
	returnList.originPtr = originBlueprint;

	tempKey.x = blueprintKey.x;		
	tempKey.y = blueprintKey.y;
	tempKey.z = blueprintKey.z;

	if (mode == 0) // default mode
	{
		//std::cout << "neighboring east border being rendered...ignoring west border..." << std::endl;

		// Check West (negative x)
		tempKey.x -= 1;										// subtract 1 to get the blueprint at -1 x 
		//std::cout << "Looking for blueprint with key: " << tempKey.x << ", " << tempKey.y << ", " << tempKey.z << std::endl;
		//auto targetsbegin = std::chrono::high_resolution_clock::now();
		blueprintMapIterator = BlueprintMap.find(tempKey);  // find the blueprint, check if it exists
															//Organic.DetermineMouseCursorTargets2(originVecPtr, directionVecPtr, 10);
		//auto targetsend = std::chrono::high_resolution_clock::now();
		//std::chrono::duration<double> targetselapsed = targetsend - targetsbegin;
		//std::cout << "Blueprint find time: " << targetselapsed.count() << std::endl;

		if (blueprintMapIterator != BlueprintMap.end())		// if this is true, a neighboring blueprint was found
		{
			//std::cout << "neighboring collection discovered." << std::endl;
			if ((blueprintMapIterator->second.RenderEastBorder == 1) && (originBlueprint->RenderWestBorder == 1)) // compare the east border of the blueprint at -1 x to the west border of the origin blueprint. If both are 1, don't render this side
			{
				//std::cout << "neighboring east border being rendered...ignoring west border... (2)" << std::endl;
				borderFlagsPtr->West = 0;				// both matched as 1, set the origin blueprint's west side to be non-renderable
				returnList.WestNeighborExists = 1;		// indicate that a neighbor exists to the west of the origin collection
				returnList.westPtr = &blueprintMapIterator->second;
			}
		}

		if (blueprintMapIterator == BlueprintMap.end())
		{
			//std::cout << "No blueprint found." << std::endl;
		}

		tempKey.x += 1;										// add +1 x to the key.



		// Check North (negative z)
		tempKey.z -= 1;										// subtract 1 to get the blueprint at -1 z
		blueprintMapIterator = BlueprintMap.find(tempKey);  // find the blueprint, check if it exists
		if (blueprintMapIterator != BlueprintMap.end())		// if this is true, a neighboring blueprint was found
		{
			if ((blueprintMapIterator->second.RenderSouthBorder == 1) && (originBlueprint->RenderNorthBorder == 1)) // compare the south border of the blueprint at -1 z to the north border of the origin blueprint. If both are 1, don't render this side
			{
				borderFlagsPtr->North = 0;				// both matched as 1, set the origin blueprint's north side to be non-renderable
				returnList.NorthNeighborExists = 1;		// indicate that a neighbor exists to the north of the origin collection
				returnList.northPtr = &blueprintMapIterator->second;
			}
		}
		tempKey.z += 1;



		// Check East (positive x)
		tempKey.x += 1;
		blueprintMapIterator = BlueprintMap.find(tempKey);  // find the blueprint, check if it exists
		if (blueprintMapIterator != BlueprintMap.end())		// if this is true, a neighboring blueprint was found
		{
			if ((blueprintMapIterator->second.RenderWestBorder == 1) && (originBlueprint->RenderEastBorder == 1)) // compare the west border of the blueprint at +1 x to the east border of the origin blueprint. If both are 1, don't render this side
			{
				borderFlagsPtr->East = 0;			// both matched as 1, set the origin blueprint's east side to be non-renderable
				returnList.EastNeighborExists = 1;	// indicate that a neighbor exists to the east of the origin collection
				returnList.eastPtr = &blueprintMapIterator->second;
			}
		}
		tempKey.x -= 1;



		// Check South (positive z)
		tempKey.z += 1;
		blueprintMapIterator = BlueprintMap.find(tempKey);  // find the blueprint, check if it exists
		if (blueprintMapIterator != BlueprintMap.end())		// if this is true, a neighboring blueprint was found
		{
			if ((blueprintMapIterator->second.RenderNorthBorder == 1) && (originBlueprint->RenderSouthBorder == 1)) // compare the north border of the blueprint at +1 z to the south border of the origin blueprint. If both are 1, don't render this side
			{
				borderFlagsPtr->South = 0;			// both matched as 1, set the origin blueprint's south side to be non-renderable
				returnList.SouthNeighborExists = 1; // indicate that a neighbor exists to the south of the origin collection
				returnList.southPtr = &blueprintMapIterator->second;
			}
		}
		tempKey.z -= 1;


		// Check Top (positive y)
		tempKey.y += 1;
		blueprintMapIterator = BlueprintMap.find(tempKey);
		if (blueprintMapIterator != BlueprintMap.end())		// if this is true, a neighboring blueprint was found
		{
			if ((blueprintMapIterator->second.RenderBottomBorder == 1) && (originBlueprint->RenderTopBorder == 1)) // compare the bottom border of the blueprint at +1 y to the top border of the origin blueprint. If both are 1, don't render this side
			{
				borderFlagsPtr->Top = 0;			// both matched as 1, set the origin blueprint's top side to be non-renderable
				returnList.TopNeighborExists = 1;	// indicate that a neighbor exists to the top of the origin collection
				returnList.topPtr = &blueprintMapIterator->second;
			}
		}
		tempKey.y -= 1;


		// Check Bottom (negative y)
		tempKey.y -= 1;
		blueprintMapIterator = BlueprintMap.find(tempKey);
		if (blueprintMapIterator != BlueprintMap.end())		// if this is true, a neighboring blueprint was found
		{
			if ((blueprintMapIterator->second.RenderTopBorder == 1) && (originBlueprint->RenderBottomBorder == 1)) // compare the top border of the blueprint at +1 y to the bottom border of the origin blueprint. If both are 1, don't render this side
			{
				borderFlagsPtr->Bottom = 0;				// both matched as 1, set the origin blueprint's top side to be non-renderable
				returnList.BottomNeighborExists = 1;	// indicate that a neighbor exists to the bottom of the origin collection
				returnList.bottomPtr = &blueprintMapIterator->second;

			}
		}
		tempKey.y += 1;
	}

	else if (mode == 1)
	{
		returnList.WestNeighborExists = 0;
		returnList.NorthNeighborExists = 0;
		returnList.EastNeighborExists = 0;
		returnList.SouthNeighborExists = 0;
		returnList.TopNeighborExists = 0;
		returnList.BottomNeighborExists = 0;

	}

	return returnList;
}