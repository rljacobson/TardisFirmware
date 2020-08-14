/* -*- mode: c++ -*-
 * Kaleidoscope-Hardware-Model01 -- Keyboard.io Model01 hardware support for Kaleidoscope
 * Copyright (C) 2017-2018  Keyboard.io, Inc
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

#ifdef ARDUINO_AVR_MODEL01
#ifndef KALEIDOSCOPE_VIRTUAL_BUILD

#include <Kaleidoscope.h>
#include <KeyboardioHID.h>
#include <avr/wdt.h>

namespace kaleidoscope {
namespace hardware {
namespace keyboardio {

/********* LED Driver *********/
bool Model01LEDDriver::isLEDChanged = true;

static constexpr int8_t key_led_map[Model01::led_count] PROGMEM = {
  3, 4, 11, 12, 19, 20, 26, 27,     36, 37, 43, 44, 51, 52, 59, 60,
  2, 5, 10, 13, 18, 21, 25, 28,     35, 38, 42, 45, 50, 53, 58, 61,
  1, 6, 9, 14, 17, 22, 24, 29,     34, 39, 41, 46, 49, 54, 57, 62,
  0, 7, 8, 15, 16, 23, 31, 30,     33, 32, 40, 47, 48, 55, 56, 63,
};

void Model01LEDDriver::enableHighPowerLeds(void) {
  // This lets the keyboard pull up to 1.6 amps from the host.
  // That violates the USB spec. But it sure is pretty looking
  DDRE |= _BV(6);
  PORTE &= ~_BV(6);

  // Set B4, the overcurrent check to an input with an internal pull-up
  DDRB &= ~_BV(4);	// set bit, input
  PORTB &= ~_BV(4);	// set bit, enable pull-up resistor
}

void Model01LEDDriver::setup() {
  // TODO: Consider not doing this until 30s after keyboard
  // boot up, to make it easier to rescue things in case of power draw issues.
  enableHighPowerLeds();
}

void Model01LEDDriver::setCrgbAt(int8_t i, cRGB crgb) {
  if (i < 0) {
    return;
  }
  if (i < 32) {
    cRGB oldColor = getCrgbAt(i);
    isLEDChanged |= !(oldColor.r == crgb.r && oldColor.g == crgb.g && oldColor.b == crgb.b);

    Model01KeyScanner::leftHand.ledData.leds[i] = crgb;
  } else if (i < 64) {
    cRGB oldColor = getCrgbAt(i);
    isLEDChanged |= !(oldColor.r == crgb.r && oldColor.g == crgb.g && oldColor.b == crgb.b);

    Model01KeyScanner::rightHand.ledData.leds[i - 32] = crgb;
  } else {
    // TODO(anyone):
    // how do we want to handle debugging assertions about crazy user
    // code that would overwrite other memory?
  }
}

int8_t Model01LEDDriver::getLedIndex(uint8_t key_offset) {
  return pgm_read_byte(&(key_led_map[key_offset]));
}

cRGB Model01LEDDriver::getCrgbAt(int8_t i) {
  if (i < 0 || i >= 64)
    return {0, 0, 0};

  if (i < 32) {
    return Model01KeyScanner::leftHand.ledData.leds[i];
  } else {
    return Model01KeyScanner::rightHand.ledData.leds[i - 32];
  }
}

void Model01LEDDriver::syncLeds() {
  if (!isLEDChanged)
    return;

  // LED Data is stored in four "banks" for each side
  // We send it all at once to make it look nicer.
  // We alternate left and right hands because otherwise
  // we run into a race condition with updating the next bank
  // on an ATTiny before it's done writing the previous one to memory

  Model01KeyScanner::leftHand.sendLEDData();
  Model01KeyScanner::rightHand.sendLEDData();

  Model01KeyScanner::leftHand.sendLEDData();
  Model01KeyScanner::rightHand.sendLEDData();

  Model01KeyScanner::leftHand.sendLEDData();
  Model01KeyScanner::rightHand.sendLEDData();

  Model01KeyScanner::leftHand.sendLEDData();
  Model01KeyScanner::rightHand.sendLEDData();

  isLEDChanged = false;
}

boolean Model01LEDDriver::ledPowerFault() {
  if (PINB & _BV(4)) {
    return true;
  } else {
    return false;
  }
}

/********* Key scanner *********/

KeyboardioScanner Model01KeyScanner::leftHand(0);
KeyboardioScanner Model01KeyScanner::rightHand(3);

keydata_t Model01KeyScanner::leftHandState;
keydata_t Model01KeyScanner::rightHandState;
keydata_t Model01KeyScanner::previousLeftHandState;
keydata_t Model01KeyScanner::previousRightHandState;
keydata_t Model01KeyScanner::leftHandMask;
keydata_t Model01KeyScanner::rightHandMask;

void Model01KeyScanner::enableScannerPower(void) {
  // Turn on power to the LED net
  DDRC |= _BV(7);
  PORTC |= _BV(7);
}

void Model01KeyScanner::setup() {
  wdt_disable();
  delay(100);
  enableScannerPower();
}

void Model01KeyScanner::readMatrix() {
  //scan the Keyboard matrix looking for connections
  previousLeftHandState = leftHandState;
  previousRightHandState = rightHandState;

  if (leftHand.readKeys()) {
    leftHandState = leftHand.getKeyData();
  }

  if (rightHand.readKeys()) {
    rightHandState = rightHand.getKeyData();
  }
}

void Model01KeyScanner::actOnHalfRow(byte row, byte colState, byte colPrevState, byte startPos) {
  if ((colState != colPrevState) || (colState != 0)) {
    for (byte col = 0; col < 8; col++) {
      // Build up the key state for row, col
      uint8_t keyState = ((bitRead(colPrevState, 0) << 0) |
                          (bitRead(colState,     0) << 1));
      if (keyState)
        ThisType::handleKeyswitchEvent(Key_NoKey, KeyAddr(row, startPos - col), keyState);

      // Throw away the data we've just used, so we can read the next column
      colState = colState >> 1;
      colPrevState = colPrevState >> 1;
    }
  }
}

void Model01KeyScanner::actOnMatrixScan() {
  for (byte row = 0; row < 4; row++) {
    actOnHalfRow(row, leftHandState.rows[row], previousLeftHandState.rows[row], 7);
    actOnHalfRow(row, rightHandState.rows[row], previousRightHandState.rows[row], 15);
  }
}


void Model01KeyScanner::scanMatrix() {
  readMatrix();
  actOnMatrixScan();
}

// In the maskKey(), unMaskKey(), and isKeyMasked() functions, we read and write bits in
// two bitfields -- one for each half of the keyboard. The fourth bit of the column number
// tells us which bitfield (right or left) to access, thus the "8" (B00001000). The row
// number tells us which element of the array to access. The last three bits of the column
// number tell us which of the eight bits to access, thus the "7" (B00000111), and we
// shift a bit starting from the left (B10000000, or 128) by that many places to get
// there. This is all nice and convenient because the keyboard has 64 keys, in symmetric
// halves, with eight keys per logical row.

constexpr byte HIGH_BIT = B10000000;
constexpr byte HAND_BIT = B00001000;
constexpr byte ROW_BITS = B00110000;
constexpr byte COL_BITS = B00000111;

void Model01KeyScanner::maskKey(KeyAddr key_addr) {
  if (!key_addr.isValid())
    return;

  auto row = key_addr.row();
  auto col = key_addr.col();
  if (col & HAND_BIT) {
    rightHandMask.rows[row] |= (HIGH_BIT >> (col & COL_BITS));
  } else {
    leftHandMask.rows[row] |= (HIGH_BIT >> (col & COL_BITS));
  }
}

void Model01KeyScanner::unMaskKey(KeyAddr key_addr) {
  if (!key_addr.isValid())
    return;

  auto row = key_addr.row();
  auto col = key_addr.col();
  if (col & HAND_BIT) {
    rightHandMask.rows[row] &= ~(HIGH_BIT >> (col & COL_BITS));
  } else {
    leftHandMask.rows[row] &= ~(HIGH_BIT >> (col & COL_BITS));
  }
}

bool Model01KeyScanner::isKeyMasked(KeyAddr key_addr) {
  if (!key_addr.isValid())
    return false;

  auto row = key_addr.row();
  auto col = key_addr.col();
  if (col & HAND_BIT) {
    return rightHandMask.rows[row] & (HIGH_BIT >> (col & COL_BITS));
  } else {
    return leftHandMask.rows[row] & (HIGH_BIT >> (col & COL_BITS));
  }
}

void Model01KeyScanner::maskHeldKeys() {
  memcpy(leftHandMask.rows, leftHandState.rows, sizeof(leftHandMask));
  memcpy(rightHandMask.rows, rightHandState.rows, sizeof(rightHandMask));
}


void Model01KeyScanner::setKeyscanInterval(uint8_t interval) {
  leftHand.setKeyscanInterval(interval);
  rightHand.setKeyscanInterval(interval);
}

bool Model01KeyScanner::isKeyswitchPressed(KeyAddr key_addr) {
  auto row = key_addr.row();
  auto col = key_addr.col();
  if (col <= 7) {
    return (bitRead(leftHandState.rows[row], 7 - col) != 0);
  } else {
    return (bitRead(rightHandState.rows[row], 7 - (col - 8)) != 0);
  }
}

bool Model01KeyScanner::wasKeyswitchPressed(KeyAddr key_addr) {
  auto row = key_addr.row();
  auto col = key_addr.col();
  if (col <= 7) {
    return (bitRead(previousLeftHandState.rows[row], 7 - col) != 0);
  } else {
    return (bitRead(previousRightHandState.rows[row], 7 - (col - 8)) != 0);
  }
}

uint8_t Model01KeyScanner::pressedKeyswitchCount() {
  return __builtin_popcountl(leftHandState.all) + __builtin_popcountl(rightHandState.all);
}

uint8_t Model01KeyScanner::previousPressedKeyswitchCount() {
  return __builtin_popcountl(previousLeftHandState.all) + __builtin_popcountl(previousRightHandState.all);
}

/********* Hardware plugin *********/

void Model01::setup() {
  KeyScanner::setup();
  LEDDriver::setup();

  TWBR = 12; // This is 400mhz, which is the fastest we can drive the ATTiny
}

void Model01::enableHardwareTestMode() {
  // Toggle the programming LEDS on
  PORTD |= (1 << 5);
  PORTB |= (1 << 0);

  // Disable the debouncer on the ATTinys
  KeyScanner::setKeyscanInterval(2);
}

}
}
}

HARDWARE_IMPLEMENTATION KeyboardHardware;

#endif
#endif
