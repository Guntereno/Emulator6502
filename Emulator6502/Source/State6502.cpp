#pragma once

#include "State6502.h"

#include "Instruction.h"
#include "Flags.h"

#include <assert.h>
#include <cstdio>

State6502::State6502()
{
    Reset();
}

void State6502::Load(const u8* pRom, size_t romLen)
{
    Reset();
    mpRom = pRom;
    mpRomEnd = mpRom + romLen;
    mpProgramCounter = mpRom;
}

void State6502::Reset()
{
    mpRom = nullptr;
    mpProgramCounter = 0;
    mRegA = 0;
    FLAGS_SET(mFlags, StatusFlag::DecimalMode);
}

bool State6502::Advance()
{
    bool shouldContinue = true;

    if (mpRom == nullptr)
        return false;

    if (mpProgramCounter < mpRomEnd)
    {
        shouldContinue = ExecuteNext();
    }
    else
    {
        return false;
    }

    return shouldContinue && (mpProgramCounter < mpRomEnd);
}

// Returns whether execution should continue
bool State6502::ExecuteNext()
{
    u8 opCode = Fetch();

    switch (opCode)
    {
        case Instruction::ADC_IM:
        {
            assert(FLAGS_CHECK_ALL(mFlags, StatusFlag::DecimalMode));
            u8 operand = Fetch();
            u16 result = static_cast<u16>(mRegA + operand);
            mRegA = static_cast<u8>(result);
            bool isNegative = ((result & (1 << 7)) != 0);
            FLAGS_SET_TO(mFlags, StatusFlag::Negative, isNegative);
            FLAGS_SET_TO(mFlags, StatusFlag::Zero, (mRegA == 0));
            FLAGS_SET_TO(mFlags, StatusFlag::Carry, (result > 255));
        }
        return true;

        case Instruction::SBC:
        {
            u8 operand = Fetch();
            mRegA -= operand;
        }
        return true;

        case Instruction::CMP:
        {
            u8 operand = Fetch();
            FLAGS_SET_TO(mFlags, StatusFlag::Zero, (operand == mRegA));
        }
        return true;

        case Instruction::LDA:
        {
            u8 operand = Fetch();
            u8 value = mpMemory[operand];
            mRegA = value;
            FLAGS_SET_TO(mFlags, StatusFlag::Zero, (mRegA == 0));
        }
        return true;

        case Instruction::STA:
        {
            u8 operand = Fetch();
            mpMemory[operand] = mRegA;
        }
        return true;

        default:
        {
            printf("Unknown opcode %#X\n", opCode);
        }
        return false;
    }
}

u8 State6502::Fetch()
{
    u8 opCode = *(mpProgramCounter);
    ++mpProgramCounter;
    return opCode;
}