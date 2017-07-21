#pragma once

#ifndef VERTEX_READING_ORDER_H
#define VERTEX_READING_ORDER_H

#include "stdafx.h"
#include <stdio.h>
#include <iostream>
#include <map>


class VertexReadingOrder {

public:
	std::map<char, std::array<char,6>> ReadOrder;			/* first value of map (key) will be the bit value for the face...32, 16, 8...etc. 
												   second value of map (value) will be an array of 6 chars that determine which vertex to go to. (value is >= 0 and <= 15) */
	void SetOrder();
											
};

#endif