#pragma once

#include "State6502.h"

#include "Instruction.h"

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
}

void State6502::Run()
{
    while (mpProgramCounter < mpRomEnd)
    {
        bool executed = ExecuteNext();

        if (executed)
        {
            printf("a = %#X, z = %s\n", mRegA, (GetZero()) ? "True" : "False");
        }
    }
}

// Returns whether execution should continue
bool State6502::ExecuteNext()
{
    u8 opCode = Fetch();

    switch (opCode)
    {
        case Instruction::ADC:
        {
            u8 operand = Fetch();
            mRegA += operand;
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
            SetZero(operand == mRegA);
        }
        return true;

        case Instruction::LDA:
        {
            u8 operand = Fetch();
            u8 value = mpMemory[operand];
            mRegA = value;
            SetZero(mRegA == 0);
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