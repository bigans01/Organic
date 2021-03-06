/*------------------------------------------------------------------------------------------

--EnclavePainter.h		(Last update 7/21/2017)

Description: Header file for EnclavePainter

Summary: An EnclavePainter is an object that updates the block types in a single enclave.


------------------------------------------------------------------------------------------*/

#pragma once

#ifndef ENCLAVEPAINTER_H
#define ENCLAVEPAINTER_H

class EnclaveCollectionMatrix;
class EnclavePainter {
	private:
		friend class EnclaveCollectionMatrix;
		unsigned char PaintData[8];			// determines which of the 64 blocks in an Enclave will be painted.
};
#endif
