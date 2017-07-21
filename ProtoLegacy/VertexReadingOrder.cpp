#pragma once

#include "stdafx.h"
#include <stdio.h>
#include <iostream>
#include <map>
#include <array>
#include "VertexReadingOrder.h"

void VertexReadingOrder::SetOrder()
{
	std::array<char, 6> rowone = {  0,0,0,0,0,0  };
	ReadOrder.insert(std::make_pair(3, rowone));
}

