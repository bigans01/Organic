/*------------------------------------------------------------------------------------------

--ECBCollectionPainter.h		(Last update 9/1/2017)

Description: Header file for ECBCollectionPainter

Summary: An ECbCollectionPainter will paint all blocks in all marked Enclaves exactly one block ID.

------------------------------------------------------------------------------------------*/

#pragma once

#ifndef ECBCOLLECTIONPAINTER_H
#define ECBCOLLECTIONPAINTER_H

class ECBCollectionPainter
{
public:
	int blockID = 0;
	unsigned char chunksToPaint[8][8];
};

#endif