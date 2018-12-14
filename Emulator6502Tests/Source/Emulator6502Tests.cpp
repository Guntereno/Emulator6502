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

    printf("ADC Test\n");
    state.Load(kRom, sizeof(kRom));
    
    bool shouldContinue = true;
    u8 kExpectedStartFlags = StatusFlag::Unused;
    CheckFlags(state.GetFlags(), kExpectedStartFlags);

    // ADC #$2
    shouldContinue = state.Advance();
    EXPECT_EQ(state.GetA(), 0x2);
    CheckFlags(state.GetFlags(), kExpectedStartFlags);
    ASSERT_EQ(shouldContinue, true);

    // ADC #$4
    shouldContinue = state.Advance();
    EXPECT_EQ(state.GetA(), 0x6);
    CheckFlags(state.GetFlags(), kExpectedStartFlags);
    ASSERT_EQ(shouldContinue, true);

    // ADC #$8
    shouldContinue = state.Advance();
    EXPECT_EQ(state.GetA(), 0xe);
    CheckFlags(state.GetFlags(), kExpectedStartFlags);
    ASSERT_EQ(shouldContinue, true);

    // ADC #$2
    shouldContinue = state.Advance();
    EXPECT_EQ(state.GetA(), 0x10);
    CheckFlags(state.GetFlags(), kExpectedStartFlags);
    ASSERT_EQ(shouldContinue, true);

    // ADC #$ef
    shouldContinue = state.Advance();
    EXPECT_EQ(state.GetA(), 0xff);
    CheckFlags(state.GetFlags(),
        kExpectedStartFlags |
        StatusFlag::Negative); // As the result was 0xff
    ASSERT_EQ(shouldContinue, true);

    // ADC #$1
    shouldContinue = state.Advance();
    EXPECT_EQ(state.GetA(), 0x0);
    CheckFlags(state.GetFlags(),
        kExpectedStartFlags | // Initial flags should still be set
        StatusFlag::Carry |   // We overflowed
        StatusFlag::Zero );   // The result was 0
    ASSERT_EQ(shouldContinue, false);
}

TEST(InstructionTests, LoadAccumulatorImmediateTest)
{
    State6502 state;

    constexpr u8 kRom[] =
    {
        0xa9, 0x00, // LDA #$0
        0xa9, 0x01, // LDA #$1
        0xa9, 0xff, // LDA #$FF
        0xa9, 0x00  // LDA #$0
    };

    printf("LDA Test\n");
    state.Load(kRom, sizeof(kRom));

    bool shouldContinue = true;
    u8 kExpectedStartFlags = StatusFlag::Unused;
    CheckFlags(state.GetFlags(), kExpectedStartFlags);

    // LDA #$0
    shouldContinue = state.Advance();
    EXPECT_EQ(state.GetA(), 0x00);
    CheckFlags(state.GetFlags(),
        kExpectedStartFlags |
        StatusFlag::Zero); // A is now 0
    ASSERT_EQ(shouldContinue, true);

    // LDA #$1
    shouldContinue = state.Advance();
    EXPECT_EQ(state.GetA(), 0x01);
    CheckFlags(state.GetFlags(), kExpectedStartFlags);
    ASSERT_EQ(shouldContinue, true);

    // LDA #$1
    shouldContinue = state.Advance();
    EXPECT_EQ(state.GetA(), 0xff);
    CheckFlags(state.GetFlags(),
        kExpectedStartFlags |
        StatusFlag::Negative); // Sign bit was set
    ASSERT_EQ(shouldContinue, true);

    // LDA #$0
    shouldContinue = state.Advance();
    EXPECT_EQ(state.GetA(), 0x00);
    CheckFlags(state.GetFlags(),
        kExpectedStartFlags |
        StatusFlag::Zero); // A is now 0
    ASSERT_EQ(shouldContinue, false);
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
