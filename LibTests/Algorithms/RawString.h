// Yuri Zamyatin, 2020. This file is part of LibTests

#pragma once

// RUN IN DEBUG MODE!!

#include "../../CommonLibs/CommonUtils/RawString.h"
#include "../../CommonLibs/CommonUtils/Assert.h"


void RawStringTestLength();
void RawStringTestCopy();
void RawStringTestEqual();


inline void RunRawStringTests()
{
	RawStringTestLength();
	RawStringTestCopy();
	RawStringTestEqual();
}