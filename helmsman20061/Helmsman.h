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

pin   _R_EN 12
pin   _RPWM 11
pin   _LPWM 3
pin   _L_EN 2

*/



#ifndef Helmsman_h
#define Helmsman_h
#include "Arduino.h"
#include "Captain.h"
#include <IBT2.h>

class Helmsman
{
  public:
    Helmsman(Captain* captain);
    void                updateRudder();

  private:
    Captain*            _captain;
    IBT2                _motor;
    unsigned long       _motorRunTime;
    int                 _motorDirection, _output;
};
#endif
