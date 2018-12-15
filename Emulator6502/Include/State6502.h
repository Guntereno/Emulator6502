#pragma once

#include "Types.h"

#include "StatusFlag.h"

#define SET_FLAG(flags, flag, value) (flags = value ? (flags | flag) : (flags & flag))
#define GET_FLAG(flags, flag) ((flags & flag) != 0)

class State6502
{
public:
    State6502();

    void Load(const u8* pRom, size_t romLen);

    // Retuns false when loaded program ends
    bool Advance();

    u8 GetFlags() { return mFlags; }
    u8 GetX() { return mRegX; }
    u8 GetY() { return mRegY; }
    u8 GetA() { return mRegA; }

private:
    void Reset();
    bool ExecuteNext();
    u8 Fetch();

    void Adc(u8 operand);
    void Sbc(u8 operand);

    const u8* mpRom = 0;
    const u8* mpRomEnd = 0;
    const u8* mpProgramCounter = 0;
    u8 mFlags = StatusFlag::Unused;
    u8 mRegA = 0;
    u8 mRegX = 0;
    u8 mRegY = 0;
    u8 mStackPointer = 0;
    u8 mpMemory[0xFFFF];
};

