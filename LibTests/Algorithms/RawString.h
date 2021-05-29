// Yuri Zamyatin, 2020-2021. This file is part of LibTests

#pragma once

// RUN IN DEBUG MODE!!

#include "CommonUtils/RawString.h"
#include "CommonUtils/Assert.h"


void RawStringTestLength();
void RawStringTestCopy();
void RawStringTestEqual();


inline void RunRawStringTests()
{
	RawStringTestLength();
	RawStringTestCopy();
	RawStringTestEqual();
}