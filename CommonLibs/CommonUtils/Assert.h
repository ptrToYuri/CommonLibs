// Yuri Zamyatin, 2020. This file is part of CommonLibs

#pragma once

#ifndef NDEBUG

    #include <iostream>

    #define ASSERT(Condition, Message) \
    do { \
        if (!(Condition)) { \
            std::cerr << "ASSERTION (" #Condition ") FAILED in " << __FILE__ \
                << ", line " << __LINE__ << ": " << (Message) << std::endl; \
            std::abort(); \
         } \
    } while (false)

#else

    # define ASSERT(Condition, Message) ((void)0)

#endif
