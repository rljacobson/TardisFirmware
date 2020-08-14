/* -*- mode: c++ -*-
 * Kaleidoscope-Hardware-OLKB-Planck -- Planck hardware support for Kaleidoscope
 * Copyright (C) 2018  Keyboard.io, Inc
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

#ifdef ARDUINO_AVR_PLANCK

#include <Kaleidoscope.h>
#include <avr/wdt.h>

ATMEGA_KEYSCANNER_IMPLEMENTATION(olkb::Planck);
HARDWARE_IMPLEMENTATION KeyboardHardware;

#endif
