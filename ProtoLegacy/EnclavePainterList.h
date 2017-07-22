/*------------------------------------------------------------------------------------------

--EnclavePainterList.h		(Last update 7/21/2017)

Description: Header file for EnclavePainterList

Summary: An EnclavePainter is an object that contains a list of EnclavePainters.


------------------------------------------------------------------------------------------*/
#pragma once

#ifndef ENCLAVEPAINTERLIST_H
#define ENCLAVEPAINTERLIST_H

#include "EnclavePainter.h"
#include <unordered_map>

class EnclavePainterList {
	public:
		std::unordered_map<int, EnclavePainter> PaintList;
};

#endif