#pragma once

enum class Instruction : u8
{
    ADC = 0x69, // Add with carry
    SBC = 0xE9, // Subtract with carry
    CMP = 0xC9, // Compare
    STA = 0x85, // Store Accumulator
    LDA = 0xA5, // Store Accumulator
};
