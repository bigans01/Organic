/*------------------------------------------------------------------------------------------

--EnclaveKeyT1Meta.h: created (12/3/2017)

Description: Header file for EnclaveKeyT1Meta.h

Summary: EnclaveKeyT1Meta is meant to be used in a vector in an instance of OrganicSystem. It is meant to track when a particular collection should be sent to the T1 processing queue, based on 
certain actions.

------------------------------------------------------------------------------------------*/

#pragma once

#ifndef ENCLAVEKEYT1META_H
#define ENCLAVEKEYT1META_H

#include "EnclaveKeyDef.h"

class EnclaveKeyT1Meta
{
public:
	int depth;
	EnclaveKeyDef::EnclaveKey key;
};

#endif

