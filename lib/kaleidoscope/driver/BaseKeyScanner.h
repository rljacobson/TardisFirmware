/* -*- mode: c++ -*-
 * BaseKeyScanner -- Keyscanner base component
 * Copyright (C) 2019  Keyboard.io, Inc
 *
 * This program is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free Software
 * Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program. If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#include <Arduino.h>

#include "kaleidoscope/key_defs.h"
#include "kaleidoscope/driver/BaseKeyScannerBlueprint.h"

namespace kaleidoscope {
namespace driver {

template <typename _KeyScannerBlueprint>
class BaseKeyScanner {
 public:
  BaseKeyScanner() {}

  typedef typename _KeyScannerBlueprint::KeyAddr KeyAddr;

  static void handleKeyswitchEvent(Key mappedKey, KeyAddr key_addr, uint8_t keyState);

  void setup() {}
  void readMatrix() {}
  void scanMatrix() {}
  void actOnMatrixScan() {}

  uint8_t pressedKeyswitchCount() {
    return 0;
  }
  bool isKeyswitchPressed(KeyAddr key_addr) {
    return false;
  }

  uint8_t previousPressedKeyswitchCount() {
    return 0;
  }
  bool wasKeyswitchPressed(KeyAddr key_addr) {
    return false;
  }

  void maskKey(KeyAddr key_addr) {}
  void unMaskKey(KeyAddr key_addr) {}
  bool isKeyMasked(KeyAddr key_addr) {
    return false;
  }
};

}
}
