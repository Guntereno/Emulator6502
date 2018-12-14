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

    const u8* mpRom;
    const u8* mpRomEnd;
    const u8* mpProgramCounter;
    u8 mFlags;
    u8 mRegA;
    u8 mRegX;
    u8 mRegY;
    u8 mStackPointer;
    u8 mpMemory[0xFFFF];
};

