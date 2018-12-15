#include "gtest/gtest.h"

#include "State6502.h"
#include "Instruction.h"

void CheckFlags(u8 flags, u8 expected)
{
    for (int i = 0; i < 8; ++i)
    {
        u8 bit = (1 << i);
        EXPECT_EQ(flags & bit, expected & bit) <<
            "Flag '" << GetStatusFlagName(static_cast<StatusFlag>(bit)) << "' differs!";
    }
}

#define STEP_CHECK_A(expectA, expectFlags, expectContinue) \
do { \
    shouldContinue = state.Advance(); \
    EXPECT_EQ(state.GetA(), expectA); \
    CheckFlags(state.GetFlags(), expectFlags); \
    ASSERT_EQ(shouldContinue, expectContinue); \
} while(0)

constexpr u8 kExpectedStartFlags = StatusFlag::Unused;


TEST(InstructionTests, AddWithCarryImmediateTest)
{
    State6502 state;

    constexpr u8 kRom[] =
    {
        0x69, 0x02,  // ADC #$2
        0x69, 0x04,  // ADC #$4
        0x69, 0x08,  // ADC #$8
        0x69, 0x02,  // ADC #$2
        0x69, 0xef,  // ADC #$ef
        0x69, 0x01   // ADC #$1
    };

    state.Load(kRom, sizeof(kRom));
    CheckFlags(state.GetFlags(), kExpectedStartFlags);

    bool shouldContinue = true;

    // ADC #$2
    STEP_CHECK_A(0x2, kExpectedStartFlags, true);

    // ADC #$4
    STEP_CHECK_A(0x6, kExpectedStartFlags, true);

    // ADC #$8
    STEP_CHECK_A(0xe, kExpectedStartFlags, true);

    // ADC #$2
    STEP_CHECK_A(0x10, kExpectedStartFlags, true);
    
    // ADC #$ef : Should set negative flag as bit 7 of 0xff is set
    STEP_CHECK_A(0xff, (kExpectedStartFlags | StatusFlag::Negative), true);

    // ADC #$1 : Carry and Zero should be set, as we've overflowed into 0
    STEP_CHECK_A(0x0, (kExpectedStartFlags | StatusFlag::Carry | StatusFlag::Zero), false);
}

TEST(InstructionTests, AddWithCarryCarryFlagTest)
{
    State6502 state;

    constexpr u8 kRom[] =
    {
        0xa9, 0x10, // LDA #$10
        0x69, 0x10, // ADC #$10
        0xa9, 0x10, // LDA #$10
        0x38,       // SEC
        0x69, 0x10, // ADC #$10
    };

    state.Load(kRom, sizeof(kRom));
    CheckFlags(state.GetFlags(), kExpectedStartFlags);

    bool shouldContinue = true;

    // LDA #$10
    STEP_CHECK_A(0x10, kExpectedStartFlags, true);

    // ADC #$10
    STEP_CHECK_A(0x20, kExpectedStartFlags, true);

    // LDA #$10
    STEP_CHECK_A(0x10, kExpectedStartFlags, true);

    // SEC
    STEP_CHECK_A(0x10, kExpectedStartFlags | StatusFlag::Carry, true);

    // ADC #$10 : Should be 0x21, 0x10 + 0x10 + carry flag
    STEP_CHECK_A(0x21, kExpectedStartFlags, false);
}

TEST(InstructionTests, LoadAccumulatorImmediateTest)
{
    State6502 state;

    constexpr u8 kRom[] =
    {
        0xa9, 0x00, // LDA #$00
        0xa9, 0x01, // LDA #$01
        0xa9, 0xff, // LDA #$FF
        0xa9, 0x00  // LDA #$00
    };

    state.Load(kRom, sizeof(kRom));
    CheckFlags(state.GetFlags(), kExpectedStartFlags);

    bool shouldContinue = true;

    // LDA #$0
    STEP_CHECK_A(0x00, (kExpectedStartFlags | StatusFlag::Zero), true);

    // LDA #$1
    STEP_CHECK_A(0x01, kExpectedStartFlags, true);

    // LDA #$FF
    STEP_CHECK_A(0xff, (kExpectedStartFlags | StatusFlag::Negative), true);

    // LDA #$0
    STEP_CHECK_A(0x00, (kExpectedStartFlags | StatusFlag::Zero), false);
}

TEST(InstructionTests, SubtractWithCarryImmediateTest)
{
    State6502 state;

    constexpr u8 kRom[] =
    {
        0xa9, 0x08, // LDA #8
        0xe9, 0x04, // SBC #4
        0xa9, 0x08, // LDA #$8
        0xe9, 0x04, // SBC #$4
        0xe9, 0x04, // SBC #$4
        0xe9, 0x04, // SBC #$8
    };

    state.Load(kRom, sizeof(kRom));
    CheckFlags(state.GetFlags(), kExpectedStartFlags);

    bool shouldContinue = true;

    // LDA #8
    STEP_CHECK_A(0x8, kExpectedStartFlags, true);

    // SBC #4 : Result should be 0x8 - 0x4 - ~Carry flag
    STEP_CHECK_A(0x3, (kExpectedStartFlags | StatusFlag::Carry), true);

    // LDA #$8
    STEP_CHECK_A(0x8, (kExpectedStartFlags | StatusFlag::Carry), true);

    // SBC #$4 : Carry flag now set, so just 0x8 - 0x4
    STEP_CHECK_A(0x4, (kExpectedStartFlags | StatusFlag::Carry), true);

    // SBC #$4 : Zero
    STEP_CHECK_A(0x0, (kExpectedStartFlags | StatusFlag::Carry | StatusFlag::Zero), true);

    // SBC #$4 : two's compliment -4, negative
    STEP_CHECK_A(0xfc, (kExpectedStartFlags | StatusFlag::Negative), false);
}

TEST(InstructionTests, StatusFlagsTest)
{
    State6502 state;

    constexpr u8 kRom[] =
    {
        0x38, // SEC
        0xF8, // SED
        0x78, // SEI

        0x58, // CLI
        0xD8, // CLD
        0x18, // CLC

    };

    state.Load(kRom, sizeof(kRom));
    CheckFlags(state.GetFlags(), kExpectedStartFlags);

    bool shouldContinue = true;

    u8 expectedFlags = kExpectedStartFlags;

    // SEC
    STEP_CHECK_A(0x00, (expectedFlags |= StatusFlag::Carry), true);

    // SED
    STEP_CHECK_A(0x00, (expectedFlags |= StatusFlag::DecimalMode), true);

    // SEI
    STEP_CHECK_A(0x00, (expectedFlags |= StatusFlag::InterruptDisable), true);

    // CLI
    STEP_CHECK_A(0x00, (expectedFlags &= ~StatusFlag::InterruptDisable), true);

    // CLD
    STEP_CHECK_A(0x00, (expectedFlags &= ~StatusFlag::DecimalMode), true);

    // CLC
    STEP_CHECK_A(0x00, (expectedFlags &= ~StatusFlag::Carry), false);
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
