/* -*- mode: c++ -*-
 * Empros Systems International's Operation Keyboard support for Kaleidoscope.
 * Copyright (C) 2019 Robert Jacobson
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

//
// Created by Robert Jacobson on 10/18/19.
//

#pragma once
#pragma clang diagnostic push
#pragma ide diagnostic   ignored "HidingNonVirtualFunction"

#ifdef ARDUINO_ARM_TARDIS

/*
    IMPORT ALL THE THINGS!
    ─────────────────────────────▄██▄
    ─────────────────────────────▀███
    ────────────────────────────────█
    ───────────────▄▄▄▄▄────────────█
    ──────────────▀▄────▀▄──────────█
    ──────────▄▀▀▀▄─█▄▄▄▄█▄▄─▄▀▀▀▄──█
    ─────────█──▄──█────────█───▄─█─█
    ─────────▀▄───▄▀────────▀▄───▄▀─█
    ──────────█▀▀▀────────────▀▀▀─█─█
    ──────────█───────────────────█─█
    ▄▀▄▄▀▄────█──▄█▀█▀█▀█▀█▀█▄────█─█
    █▒▒▒▒█────█──█████████████▄───█─█
    █▒▒▒▒█────█──██████████████▄──█─█
    █▒▒▒▒█────█───██████████████▄─█─█
    █▒▒▒▒█────█────██████████████─█─█
    █▒▒▒▒█────█───██████████████▀─█─█
    █▒▒▒▒█───██───██████████████──█─█
    ▀████▀──██▀█──█████████████▀──█▄█
    ──██───██──▀█──█▄█▄█▄█▄█▄█▀──▄█▀
    ──██──██────▀█─────────────▄▀▓█
    ──██─██──────▀█▀▄▄▄▄▄▄▄▄▄▀▀▓▓▓█
    ──████────────█▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓█
    ──███─────────█▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓█
    ──██──────────█▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓█
    ──██──────────█▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓█
    ──██─────────▐█▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓█
    ──██────────▐█▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓█
    ──██───────▐█▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓█▌
    ──██──────▐█▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓█▌
    ──██─────▐█▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓█▌
    ──██────▐█▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓█▌
*/

#include "Arduino.h"

#define KALEIDOSCOPE_HIDADAPTOR_DISABLE_MOUSE 1
#define HARDWARE_IMPLEMENTATION kaleidoscope::hardware::empros::Tardis
#include "ARMDevice.hpp"
#include "ARMDeviceBlueprint.hpp"
#include "Kaleidoscope-HIDAdaptor-KeyboardioHID.h"
#include "Kaleidoscope.h"
#include "TardisKeyScanner.hpp"
#include "TardisKeyScannerBlueprint.hpp"
#include "TardisLedDriver.hpp"
#include "TardisLedDriverBlueprint.hpp"
#include "kaleidoscope/driver/bootloader/None.h"

// These are all re-assignable. Changing the GPIO port (GPIOA vs. GPIOB) requires editing the defines in
// `Tardis.ccp` and almost every line in `Tardis::setup()`.
#define NSS 12  /* PB12 */
#define SCK 6   /* PB6, the EVENT pin. */
#define MISO 14 /* PB14 */
#define MOSI 15 /* PB15 */

namespace kaleidoscope::hardware::empros {

struct TardisDeviceBlueprint: kaleidoscope::hardware::arm::ARMDeviceBlueprint {
    typedef TardisKeyScannerBlueprint              KeyScannerBlueprint;
    typedef TardisKeyScanner                       KeyScanner;
    typedef TardisLedDriverBlueprint               LEDsBlueprint;
    typedef TardisLedDriver                        LEDs;
    typedef kaleidoscope::driver::bootloader::None BootLoader;
};

typedef kaleidoscope::hardware::arm::ARMDevice<TardisDeviceBlueprint> TardisSuper;

class Tardis: public TardisSuper {
public:
    void setup();
    // static void enableHardwareTestMode();

    static void setKeyscanInterval(uint8_t interval) {
        TardisKeyScanner::setKeyscanInterval(interval);
    }
    static int getKeyscanInterval() {
        TardisKeyScanner::getKeyscanInterval();
    }

    static void setLEDFrequency(int frequency) {
        TardisLedDriver::LEDFrequency_ = frequency;
    }
    static int getLEDFrequency() {
        return TardisLedDriver::LEDFrequency_;
    }

    // TardisKeyScanner defers to this.
    static void readMatrix();

private:
    // Data management
    keydata_t &in_buffer;  // input_buffer
};

}  // namespace kaleidoscope::hardware::empros

#endif

#pragma clang diagnostic pop
