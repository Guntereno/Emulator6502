#pragma once

#include "Types.h"

enum StatusFlag : u32
{
    Carry = 1 << 0,
    Zero = 1 << 1,
    InterruptDisable = 1 << 2,
    DecimalMode = 1 << 3,
    BreakCommand = 1 << 4,
    Overflow = 1 << 5,
    Negative = 1 << 6,
};

const char* GetStatusFlagName(StatusFlag flag);