#pragma once

#ifndef DUMMYMANIFEST_H
#define DUMMYMANIFEST_H


#include <memory>
#include <stdio.h>
#include <iostream>

class dummyManifest
{
public:
	//dummyManifest(dummyManifest& a) : testint(std::move(a.testint)) {}
	dummyManifest(dummyManifest& a) : somevalue(5) {}
	std::unique_ptr<int> testint;
	int somevalue = 0;
	//dummyManifest() {};
	//dummyManifest(dummyManifest &&) {};
	
	dummyManifest& operator=(dummyManifest& a)
	{
		//testint = std::move(a.testint);
		return *this;
	}
	
};

#endif