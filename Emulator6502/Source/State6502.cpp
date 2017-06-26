#pragma once

#include "stdafx.h"

#include "State6502.h"

#include "Instruction.h"


State6502::State6502()
{
    Reset();
}

void State6502::Load(const u8* pRom, size_t romLen)
{
    Reset();
    _pRom = pRom;
    _pRomEnd = _pRom + romLen;
    _pProgramCounter = _pRom;
}

void State6502::Reset()
{
    _pRom = NULL;
    _pProgramCounter = 0;
    _regA = 0;
}

void State6502::Run()
{
    while (_pProgramCounter < _pRomEnd)
    {
        bool executed = ExecuteNext();

        if (executed)
        {
            printf("a = %#X, z = %s\n", _regA, (GetZero()) ? "True" : "False");
        }
    }
}

// Returns whether execution should continue
bool State6502::ExecuteNext()
{
    u8 opCode = Fetch();

    switch (opCode)
    {
        case Instruction::kADC:
        {
            u8 operand = Fetch();
            _regA += operand;
        }
        return true;

        case Instruction::kSBC:
        {
            u8 operand = Fetch();
            _regA -= operand;
        }
        return true;

        case Instruction::kCMP:
        {
            u8 operand = Fetch();
            SetZero(operand == _regA);
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
   u8 opCode = *(_pProgramCounter);
    ++_pProgramCounter;
    return opCode;
}