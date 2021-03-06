// Yuri Zamyatin, 2020-2021. This file is part of LibTests

#pragma once

#include "Vector.h"
#include "Optional.h"
#include "List.h"


inline void TestContainers()
{
	RunVectorTests();
	RunOptionalTests();
	RunListTests();
}
