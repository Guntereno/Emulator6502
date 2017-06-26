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


    inline void SetCarry(bool value) { SET_FLAG(_flags, kFlagCarry, value); }
    inline bool GetCarry() { return GET_FLAG(_flags, kFlagCarry); }

    inline void SetZero(bool value) { SET_FLAG(_flags, kFlagZero, value); }
    inline bool GetZero() { return GET_FLAG(_flags, kFlagZero); }

    inline void SetInterruptDisable(bool value) { SET_FLAG(_flags, kFlagInterruptDisable, value); }
    inline bool GetInterruptDisable() { return GET_FLAG(_flags, kFlagInterruptDisable); }

    inline void SetDecimalMode(bool value) { SET_FLAG(_flags, kFlagDecimalMode, value); }
    inline bool GetDecimalMode() { return GET_FLAG(_flags, kFlagDecimalMode); }

    inline void SetBreakCommand(bool value) { SET_FLAG(_flags, kFlagBreakCommand, value); }
    inline bool GetBreakCommand() { return GET_FLAG(_flags, kFlagBreakCommand); }

    inline void SetOverflow(bool value) { SET_FLAG(_flags, kFlagOverflow, value); }
    inline bool GetOverflow() { return GET_FLAG(_flags, kFlagOverflow); }

    inline void SetNegative(bool value) { SET_FLAG(_flags, kFlagNegative, value); }
    inline bool GetNegative() { return GET_FLAG(_flags, kFlagNegative); }


    const u8* _pRom;
    const u8* _pRomEnd;
    const u8* _pProgramCounter;
    u8 _flags;
    u8 _regA;
    u8 _regX;
    u8 _regY;
};

