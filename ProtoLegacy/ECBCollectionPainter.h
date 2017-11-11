/*------------------------------------------------------------------------------------------

--ECBCollectionPainter.h		(Last update 9/15/2017)

Description: Header file for ECBCollectionPainter

Summary: An ECbCollectionPainter will paint all blocks in all marked Enclaves exactly one block ID.

------------------------------------------------------------------------------------------*/

#pragma once

#ifndef ECBCOLLECTIONPAINTER_H
#define ECBCOLLECTIONPAINTER_H

class EnclaveCollectionBlueprint;
class EnclaveCollection;
class EnclaveCollectionMatrix;
class ECBCollectionPainter
{
private:
	friend EnclaveCollectionBlueprint;
	friend EnclaveCollection;
	friend EnclaveCollectionMatrix;
	int blockID = 0;								// the ID of the block to be painted
	unsigned char chunksToPaint[8][8] = { 0 };		// the chunks to paint; each bit in the byte array represents a point on the y-axis. first bit = y at 0, second bit is y at 1, etc.
};

#endif