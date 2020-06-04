/*
   Helmsman autopilot a yacht autopilot for holding a set course.

   Author: Arne
   Copyright (c) 2018 Arne

   This library is free software; you can redistribute it
   and/or modify it under the terms of the GNU Lesser
   General Public License as published by the Free Software
   Foundation; either version 2.1 of the License, or (at
   your option) any later version.

   This library is distributed in the hope that it will
   be useful, but WITHOUT ANY WARRANTY; without even the
   implied warranty of MERCHANTABILITY or FITNESS FOR A
   PARTICULAR PURPOSE.  See the GNU Lesser General Public
   License for more details.

   You should have received a copy of the GNU Lesser
   General Public License along with this library; if not,
   write to the Free Software Foundation, Inc.,
   51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA

*/
#ifndef HMI_h
#define HMI_h
#include "Captain.h"
#include "Arduino.h"
#include <LiquidCrystal.h>

#define btnRIGHT  0
#define btnUP     1
#define btnDOWN   2
#define btnLEFT   3
#define btnSELECT 4
#define btnNONE   5

class HMI
{
  public:
    HMI(Captain* captain);
    void          update();

  private:
    // Declare push button pins & objects
    bool                powerState = false;
    int                 menuState = 1;

    Captain*           _captain;
    LiquidCrystal*     _lcd;
    long                _lastDebounceTime = 0; 
    int                 readButtons();
    void                mainSettings(),
                        deadbandSettings(),
                        sampleTimeSettings(),
                        kDSettings(),
                        kISettings(),
                        kPSettings(),
                        autoTuneSettings(),
                        motorDirectionSettings(),
                        maxMotorRunTime();
};
#endif
