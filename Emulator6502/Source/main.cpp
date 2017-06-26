#include "stdafx.h"

#include "Roms\AccumulatorTest.h"
#include "State6502.h"


int main()
{
    State6502 state;
    state.Load(Roms::kAccumulatorTestRom, sizeof(Roms::kAccumulatorTestRom));
    state.Run();

    return 0;
}

