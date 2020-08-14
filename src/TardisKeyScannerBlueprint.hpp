//
// Created by Robert Jacobson on 10/22/19.
//

#pragma once

#ifdef ARDUINO_ARM_TARDIS

#include "Arduino.h"

#include "kaleidoscope/driver/BaseKeyScannerBlueprint.h"

namespace kaleidoscope::hardware::empros {

struct TardisKeyScannerBlueprint : kaleidoscope::driver::BaseKeyScannerBlueprint {
  KEYSCANNER_BLUEPRINT(8, 32)
};

}

typedef kaleidoscope::hardware::empros::TardisKeyScannerBlueprint::KeyAddr KeyAddr;

#endif
