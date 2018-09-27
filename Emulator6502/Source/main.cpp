#include "Roms\AccumulatorTest.h"
#include "Roms\RamTest.h"

#include "State6502.h"

#include <cstdio>

int main()
{
    State6502 state;

    printf("Accumulator Test\n");
    state.Load(Roms::kAccumulatorTestRom, sizeof(Roms::kAccumulatorTestRom));
    state.Run();

    printf("\nRAM Test\n");
    state.Load(Roms::kRamTestRom, sizeof(Roms::kRamTestRom));
    state.Run();

    return 0;
}

