#pragma once

enum ProcessorFlag
{
    kFlagCarry = 1 << 0,
    kFlagZero = 1 << 1,
    kFlagInterruptDisable = 1 << 2,
    kFlagDecimalMode = 1 << 3,
    kFlagBreakCommand = 1 << 4,
    kFlagOverflow = 1 << 5,
    kFlagNegative = 1 << 6,
};

