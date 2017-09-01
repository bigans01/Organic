#include "stdafx.h"
#include <stdio.h>
#include <chrono>
#include "EnclaveCollectionBlueprintMatrix.h"

void EnclaveCollectionBlueprintMatrix::DetermineBlueprintBordersToRender(EnclaveKeyDef::EnclaveKey blueprintKey, EnclaveCollectionBlueprint* originBlueprint, EnclaveCollectionBorderFlags* borderFlagsPtr)
{
	std::unordered_map<EnclaveKeyDef::EnclaveKey, EnclaveCollectionBlueprint, EnclaveKeyDef::KeyHasher>::iterator blueprintMapIterator;
	EnclaveKeyDef::EnclaveKey tempKey;

	tempKey.x = blueprintKey.x;		
	tempKey.y = blueprintKey.y;
	tempKey.z = blueprintKey.z;
	std::cout << "neighboring east border being rendered...ignoring west border..." << std::endl;

	// Check West (negative x)
	tempKey.x -= 1;										// subtract 1 to get the blueprint at -1 x 
	std::cout << "Looking for blueprint with key: " << tempKey.x << ", " << tempKey.y << ", " << tempKey.z << std::endl;
	blueprintMapIterator = BlueprintMap.find(tempKey);  // find the blueprint, check if it exists
	if (blueprintMapIterator != BlueprintMap.end())		// if this is true, a neighboring blueprint was found
	{
		//std::cout << "neighboring collection discovered." << std::endl;
		if ((blueprintMapIterator->second.RenderEastBorder == 1) && (originBlueprint->RenderWestBorder==1)) // compare the east border of the blueprint at -1 x to the west border of the origin blueprint. If both are 1, don't render this side
		{
			//std::cout << "neighboring east border being rendered...ignoring west border... (2)" << std::endl;
			borderFlagsPtr->West = 0;			// both matched as 1, set the origin blueprint's west side to be non-renderable
		}
	}

	if (blueprintMapIterator == BlueprintMap.end())
	{
		std::cout << "No blueprint found." << std::endl;
	}		

	tempKey.x += 1;										// add +1 x to the key.



	// Check North (negative z)
	tempKey.z -= 1;										// subtract 1 to get the blueprint at -1 z
	blueprintMapIterator = BlueprintMap.find(tempKey);  // find the blueprint, check if it exists
	if (blueprintMapIterator != BlueprintMap.end())		// if this is true, a neighboring blueprint was found
	{
		if ((blueprintMapIterator->second.RenderSouthBorder == 1) && (originBlueprint->RenderNorthBorder==1)) // compare the south border of the blueprint at -1 z to the north border of the origin blueprint. If both are 1, don't render this side
		{
			borderFlagsPtr->North = 0;			// both matched as 1, set the origin blueprint's north side to be non-renderable
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
			borderFlagsPtr->Bottom = 0;			// both matched as 1, set the origin blueprint's top side to be non-renderable
		}
	}
	tempKey.y += 1;


}