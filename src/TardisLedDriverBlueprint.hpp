//
// Created by Robert Jacobson on 10/22/19.
//

#pragma once

#ifdef ARDUINO_ARM_TARDIS

#include <Arduino.h>

#include "kaleidoscope/driver/BaseLedDriverBlueprint.h"

namespace kaleidoscope::hardware::empros {

struct TardisLedDriverBlueprint: public kaleidoscope::driver::BaseLedDriverBlueprint {
    static constexpr LedCountType led_count = 148;
};
}

#endif
