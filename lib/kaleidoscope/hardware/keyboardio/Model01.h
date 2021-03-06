/* -*- mode: c++ -*-
 * Kaleidoscope-Hardware-Model01 -- Keyboard.io Model01 hardware support for Kaleidoscope
 * Copyright (C) 2017-2019  Keyboard.io, Inc
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

#ifdef ARDUINO_AVR_MODEL01

#include <Arduino.h>

#define HARDWARE_IMPLEMENTATION kaleidoscope::hardware::keyboardio::Model01
#include "Kaleidoscope-HIDAdaptor-KeyboardioHID.h"
#include "KeyboardioScanner.h"

#define CRGB(r,g,b) (cRGB){b, g, r}

#include "kaleidoscope/driver/BaseKeyScanner.h"
#include "kaleidoscope/driver/BaseLEDDriver.h"
#include "kaleidoscope/driver/bootloader/avr/Caterina.h"
#include "kaleidoscope/hardware/avr/AVRDevice.h"

namespace kaleidoscope {
namespace hardware {
namespace keyboardio {

class Model01;

struct Model01LEDDriverBlueprint : public kaleidoscope::driver::BaseLEDDriverBlueprint {
  static constexpr LEDCountType led_count = 64;
};

class Model01LEDDriver : public kaleidoscope::driver::BaseLEDDriver<Model01LEDDriverBlueprint> {
 public:
  static void setup();
  static void syncLeds();
  static void setCrgbAt(int8_t i, cRGB crgb);
  static cRGB getCrgbAt(int8_t i);

  static int8_t getLedIndex(uint8_t key_offset);

  static void enableHighPowerLeds();
  static boolean ledPowerFault();

 private:
  static bool isLEDChanged;
};

struct Model01KeyScannerBlueprint : public kaleidoscope::driver::BaseKeyScannerBlueprint {
  KEYSCANNER_BLUEPRINT(4, 16);
};

class Model01KeyScanner : public kaleidoscope::driver::BaseKeyScanner<Model01KeyScannerBlueprint> {
  friend class Model01LEDDriver;
  friend class Model01;
 private:
  typedef Model01KeyScanner ThisType;
 public:
  static void setup();
  static void scanMatrix();
  static void readMatrix();
  static void actOnMatrixScan();

  static void maskKey(KeyAddr key_addr);
  static void unMaskKey(KeyAddr key_addr);
  static bool isKeyMasked(KeyAddr key_addr);
  static void maskHeldKeys();

  static bool isKeyswitchPressed(KeyAddr key_addr);
  static uint8_t pressedKeyswitchCount();

  static bool wasKeyswitchPressed(KeyAddr key_addr);
  static uint8_t previousPressedKeyswitchCount();

  static void setKeyscanInterval(uint8_t interval);

 protected:

  static keydata_t leftHandState;
  static keydata_t rightHandState;
  static keydata_t previousLeftHandState;
  static keydata_t previousRightHandState;

  static KeyboardioScanner leftHand;
  static KeyboardioScanner rightHand;

  static keydata_t leftHandMask;
  static keydata_t rightHandMask;

  static void actOnHalfRow(byte row, byte colState, byte colPrevState, byte startPos);
  static void enableScannerPower();
};

struct Model01DeviceBlueprint : kaleidoscope::hardware::avr::AVRDeviceBlueprint {
  typedef Model01LEDDriverBlueprint  LEDDriverBlueprint;
  typedef Model01LEDDriver LEDDriver;
  typedef Model01KeyScannerBlueprint KeyScannerBlueprint;
  typedef Model01KeyScanner KeyScanner;
  typedef kaleidoscope::driver::bootloader::avr::Caterina BootLoader;
};

class Model01 : public kaleidoscope::hardware::avr::AVRDevice<Model01DeviceBlueprint> {
 public:
  static void setup();

  static void enableHardwareTestMode();
};

}
}
}

#include "kaleidoscope/hardware/key_indexes.h"

#define PER_KEY_DATA_STACKED(dflt,                                    \
               r0c0, r0c1, r0c2, r0c3, r0c4, r0c5, r0c6,                \
               r1c0, r1c1, r1c2, r1c3, r1c4, r1c5, r1c6,                \
               r2c0, r2c1, r2c2, r2c3, r2c4, r2c5,                      \
               r3c0, r3c1, r3c2, r3c3, r3c4, r3c5, r2c6,                \
               r0c7, r1c7, r2c7, r3c7,                                  \
               r3c6,                                                    \
                                                                        \
               r0c9,  r0c10, r0c11, r0c12, r0c13, r0c14, r0c15,         \
               r1c9,  r1c10, r1c11, r1c12, r1c13, r1c14, r1c15,         \
                      r2c10, r2c11, r2c12, r2c13, r2c14, r2c15,         \
               r2c9,  r3c10, r3c11, r3c12, r3c13, r3c14, r3c15,         \
               r3c8,  r2c8,  r1c8, r0c8,                                \
               r3c9, ...)                                               \
                                                                        \
    r0c0, r0c1, r0c2, r0c3, r0c4, r0c5, r0c6, r0c7, r0c8, r0c9, r0c10, r0c11, r0c12, r0c13, r0c14, r0c15, \
    r1c0, r1c1, r1c2, r1c3, r1c4, r1c5, r1c6, r1c7, r1c8, r1c9, r1c10, r1c11, r1c12, r1c13, r1c14, r1c15, \
    r2c0, r2c1, r2c2, r2c3, r2c4, r2c5, r2c6, r2c7, r2c8, r2c9, r2c10, r2c11, r2c12, r2c13, r2c14, r2c15, \
    r3c0, r3c1, r3c2, r3c3, r3c4, r3c5, r3c6, r3c7, r3c8, r3c9, r3c10, r3c11, r3c12, r3c13, r3c14, RESTRICT_ARGS_COUNT((r3c15), 64, KEYMAP_STACKED, ##__VA_ARGS__)

#define PER_KEY_DATA(dflt,                                                                                 \
  r0c0, r0c1, r0c2, r0c3, r0c4, r0c5, r0c6,        r0c9,  r0c10, r0c11, r0c12, r0c13, r0c14, r0c15, \
  r1c0, r1c1, r1c2, r1c3, r1c4, r1c5, r1c6,        r1c9,  r1c10, r1c11, r1c12, r1c13, r1c14, r1c15, \
  r2c0, r2c1, r2c2, r2c3, r2c4, r2c5,                     r2c10, r2c11, r2c12, r2c13, r2c14, r2c15, \
  r3c0, r3c1, r3c2, r3c3, r3c4, r3c5, r2c6,        r2c9,  r3c10, r3c11, r3c12, r3c13, r3c14, r3c15, \
              r0c7, r1c7, r2c7, r3c7,                             r3c8,  r2c8,  r1c8, r0c8,         \
                          r3c6,                                          r3c9, ...)                      \
    r0c0, r0c1, r0c2, r0c3, r0c4, r0c5, r0c6, r0c7, r0c8, r0c9, r0c10, r0c11, r0c12, r0c13, r0c14, r0c15, \
    r1c0, r1c1, r1c2, r1c3, r1c4, r1c5, r1c6, r1c7, r1c8, r1c9, r1c10, r1c11, r1c12, r1c13, r1c14, r1c15, \
    r2c0, r2c1, r2c2, r2c3, r2c4, r2c5, r2c6, r2c7, r2c8, r2c9, r2c10, r2c11, r2c12, r2c13, r2c14, r2c15, \
    r3c0, r3c1, r3c2, r3c3, r3c4, r3c5, r3c6, r3c7, r3c8, r3c9, r3c10, r3c11, r3c12, r3c13, r3c14, RESTRICT_ARGS_COUNT((r3c15), 64, KEYMAP, ##__VA_ARGS__)

#endif
