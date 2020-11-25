// Yuri Zamyatin, 2020. This file is part of LibTests

#pragma once

// RUN IN DEBUG MODE!!

#include "CommonTypes/Optional.h"
#include "CommonUtils/Assert.h"


void OptionalTestAllocation();


inline void RunOptionalTests()
{
	OptionalTestAllocation();
}