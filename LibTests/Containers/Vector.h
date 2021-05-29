// Yuri Zamyatin, 2020-2021. This file is part of LibTests

#pragma once

// RUN IN DEBUG MODE!!

#include <iostream>
#include "CommonTypes/Vector.h"
#include "CommonUtils/Assert.h"
#include "CommonTypes/Exception.h"
#include "CommonUtils/RawString.h"


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
void VectorTestPlacementNew();


inline void RunVectorTests()
{
	VectorTestConstructors();
	VectorTestOperators();
	VectorTestAssignment();
	VectorTestPushPopShift();
	VectorTestInsert();
	VectorTestReserve();
	VectorTestResize();
	VectorTestErase();
	VectorTestIterators();
	VectorTestConst();
//	VectorTestPlacementNew();
}