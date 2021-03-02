#include <iostream>
#include "lfsr.h"

void lfsr_calculate(uint16_t *reg)
{
    *reg = ((((*reg >> 5) ^ (*reg >> 3) ^ (*reg >> 2) ^ (*reg) ) & 0x00000001) << 15) | (*reg >> 1);
}
