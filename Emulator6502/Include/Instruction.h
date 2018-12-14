#pragma once

// Note: The following suffixes refer to the address mode:
// 
// - IM = Immediate
// - ZP = Zero Page
// - ZPX = Zero Page,X
// - AB = Absolute
// - ABX = Absolute,X
// - ABY = Absolute,Y
// - INX = (Indirect,X)
// - INY = (Indirect),Y

enum class Instruction : u8
{
    // Add with carry
    ADC_IM = 0x69,
    ADC_ZP = 0x65,
    ADC_ZPX = 0x75,
    ADC_AB = 0x6d,
    ADC_ABX = 0x7d,
    ADC_ABY = 0x6d,
    ADC_INX = 0x61,
    ADC_INY = 0x71,

    SBC = 0xE9, // Subtract with carry
    CMP = 0xC9, // Compare
    STA = 0x85, // Store Accumulator
    LDA = 0xA5, // Store Accumulator
};
