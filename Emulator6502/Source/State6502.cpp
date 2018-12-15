#pragma once

#include "State6502.h"

#include "Instruction.h"
#include "Flags.h"

#include <assert.h>
#include <cstdio>

#define NEGATIVE_BIT_SET(value) ((value & (1 << 7)) != 0)

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

    // This bit is unused, but is expected to always be set
    mFlags = StatusFlag::Unused;
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

void State6502::Adc(u8 operand)
{
    u16 result = static_cast<u16>(mRegA + operand);
    if (FLAGS_CHECK_ALL(mFlags, StatusFlag::Carry))
    {
        ++result;
    }
    mRegA = static_cast<u8>(result);

    FLAGS_SET_TO(mFlags, StatusFlag::Carry, (result > 0xFF));
    FLAGS_SET_TO(mFlags, StatusFlag::Negative, NEGATIVE_BIT_SET(mRegA));
    FLAGS_SET_TO(mFlags, StatusFlag::Zero, (mRegA == 0));
}

void State6502::Sbc(u8 operand)
{
    Adc(static_cast<u8>(operand ^ 0xff));
}

// Returns whether execution should continue
bool State6502::ExecuteNext()
{
    u8 opCode = Fetch();

    switch (opCode)
    {
        case Instruction::ADC_IM:
        {
            u8 operand = Fetch();
            Adc(operand);
        }
        return true;

        case Instruction::SBC_IM:
        {
            u8 operand = Fetch();
            Sbc(operand);
        }
        return true;

        case Instruction::LDA_IM:
        {
            u8 operand = Fetch();
            mRegA = operand;
            FLAGS_SET_TO(mFlags, StatusFlag::Negative, NEGATIVE_BIT_SET(mRegA));
            FLAGS_SET_TO(mFlags, StatusFlag::Zero, (mRegA == 0));
        }
        return true;

        case Instruction::SEC:
        {
            FLAGS_SET(mFlags, StatusFlag::Carry);
        }
        return true;

        case Instruction::SED:
        {
            // The NES doesn't support arithmetic mode, so I'm not going to
            assert(!FLAGS_CHECK_ALL(mFlags, StatusFlag::DecimalMode));
            FLAGS_SET(mFlags, StatusFlag::DecimalMode);
        }
        return true;

        case Instruction::SEI:
        {
            FLAGS_SET(mFlags, StatusFlag::InterruptDisable);
        }
        return true;

        case Instruction::CLC:
        {
            FLAGS_UNSET(mFlags, StatusFlag::Carry);
        }
        return true;

        case Instruction::CLD:
        {
            FLAGS_UNSET(mFlags, StatusFlag::DecimalMode);
        }
        return true;

        case Instruction::CLI:
        {
            FLAGS_UNSET(mFlags, StatusFlag::InterruptDisable);
        }
        return true;

        case Instruction::CLV:
        {
            FLAGS_UNSET(mFlags, StatusFlag::Overflow);
        }
        return true;

        case Instruction::CMP:
        {
            u8 operand = Fetch();
            FLAGS_SET_TO(mFlags, StatusFlag::Zero, (operand == mRegA));
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