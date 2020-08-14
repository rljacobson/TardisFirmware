/* -*- mode: c++ -*-
 * hardware::avr::AVRDevice -- Generic AVR device base class
 * Copyright (C) 2019  Keyboard.io, Inc
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of version 3 of the GNU General Public License as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#ifdef __ARM__

#include <Arduino.h>
#include "kaleidoscope/Device.h"
#include "ARMDeviceBlueprint.h"

namespace kaleidoscope {
namespace hardware {
namespace arm {

template <typename _DeviceBlueprint>
class AVRDevice : public kaleidoscope::Device<_DeviceBlueprint> {
 public:
  auto serialPort() -> decltype(Serial) & {
    return Serial;
  }
};

}
}
}

#endif
