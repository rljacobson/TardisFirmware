/* -*- mode: c++ -*-
 * DeviceBlueprint -- Device blueprint base class
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

#include "kaleidoscope/driver/keyscanner/None.h"
#include "kaleidoscope/driver/led/None.h"
#include "kaleidoscope/driver/mcu/None.h"
#include "kaleidoscope/driver/bootloader/None.h"
#include "kaleidoscope/driver/storage/None.h"

namespace kaleidoscope {

struct DeviceBlueprint {
  typedef kaleidoscope::driver::keyscanner::NoKeyScannerBlueprint KeyScannerBlueprint;
  typedef kaleidoscope::driver::keyscanner::None KeyScanner;
  typedef kaleidoscope::driver::led::NoLedDriverBlueprint LEDDriverBlueprint;
  typedef kaleidoscope::driver::led::None LEDDriver;
  typedef kaleidoscope::driver::mcu::None MCU;
  typedef kaleidoscope::driver::bootloader::None BootLoader;
  typedef kaleidoscope::driver::storage::NoStorageBlueprint StorageBlueprint;
  typedef kaleidoscope::driver::storage::None Storage;
};

}
