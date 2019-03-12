#include "StatusFlag.h"

const char* GetStatusFlagName(StatusFlag flag)
{
    switch (flag)
    {
        case StatusFlag::Carry:
            return "Carry";
        case StatusFlag::Zero:
            return "Zero";
        case StatusFlag::InterruptDisable:
            return "InterruptDisable";
        case StatusFlag::DecimalMode:
            return "DecimalMode";
        case StatusFlag::BreakCommand:
            return "BreakCommand";
        case StatusFlag::Unused:
            return "Unused";
        case StatusFlag::Overflow:
            return "Overflow";
        case StatusFlag::Negative:
            return "Negative";
        default:
            return "Unknown";
    }
}
