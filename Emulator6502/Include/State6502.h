#pragma once

#include "Types.h"

#include "Processor.h"


#define SET_FLAG(flags, flag, value) (flags = value ? (flags | flag) : (flags & flag))
#define GET_FLAG(flags, flag) ((flags & flag) != 0)

class State6502
{
public:
    State6502();

    void Load(const u8* pRom, size_t romLen);

    void Run();

private:
    void Reset();
    bool ExecuteNext();
    u8 Fetch();

    void SetFlags(u32 flags, bool value)
    {
        if (value)
        {
            mFlags = (mFlags | flags);
        }
        else
        {
            mFlags = (mFlags & flags);
        }
    }

    bool GetFlags(u8 flags)
    {
        return (mFlags & flags) != 0;
    }

    void SetCarry(bool value) { SetFlags(ProcessorFlag::Carry, value); }
    bool GetCarry() { return GetFlags(ProcessorFlag::Carry); }

    void SetZero(bool value) { SetFlags(ProcessorFlag::Zero, value); }
    bool GetZero() { return GetFlags(ProcessorFlag::Zero); }

    void SetInterruptDisable(bool value) { SetFlags(ProcessorFlag::InterruptDisable, value); }
    bool GetInterruptDisable() { GetFlags(ProcessorFlag::InterruptDisable); }

    void SetDecimalMode(bool value) { SetFlags(ProcessorFlag::DecimalMode, value); }
    bool GetDecimalMode() { return GetFlags(ProcessorFlag::DecimalMode); }

    void SetBreakCommand(bool value) { SetFlags(ProcessorFlag::BreakCommand, value); }
    bool GetBreakCommand() { return GetFlags(ProcessorFlag::BreakCommand); }

    void SetOverflow(bool value) { SetFlags(ProcessorFlag::Overflow, value); }
    bool GetOverflow() { return GetFlags(ProcessorFlag::Overflow); }

    void SetNegative(bool value) { SetFlags(ProcessorFlag::Negative, value); }
    bool GetNegative() { return GetFlags(ProcessorFlag::Negative); }

    const u8* mpRom;
    const u8* mpRomEnd;
    const u8* mpProgramCounter;
    u32 mFlags;
    u8 mRegA;
    u8 mRegX;
    u8 mRegY;

    u8 mpMemory[0xFFFF];
};

