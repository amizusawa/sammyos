#ifndef FUNCTION_H
#define FUNCTION_H

#include <panic.h>

#define UNUSED(x) (void) (x);
#define NOT_REACHED() kpanic("Reached an unreachable statement");

//TODO Make assertion failure message actually useful
#define ASSERT(condition) \
    if (!(condition)) {\
        kpanic("Assertion failed"); \
    }

#endif
