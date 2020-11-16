// RUN IN DEBUG MODE!!

#pragma once

#include "../../CommonLibs/CommonTypes/Vector.h"
#include "../../CommonLibs/CommonUtils/Assert.h"
#include "../../CommonLibs/CommonTypes/Exception.h"


void VectorTestConstructors();
void VectorTestOperators();
void VectorTestAssignment();
void VectorTestPushPopShift();
void VectorTestInsert();
void VectorTestReserve();
void VectorTestResize();
void VectorTestErase();
void VectorTestIterators();
void VectorTestConst();


inline void RunVectorTests()
{
	VectorTestConstructors();
	VectorTestAssignment();
	VectorTestPushPopShift();
	VectorTestInsert();
	VectorTestReserve();
	VectorTestResize();
	VectorTestErase();
	VectorTestIterators();
	VectorTestConst();
}