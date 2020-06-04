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
#include "SparkFun_BNO080_Arduino_Library.h"
#include "Captain.h"
#include "HMI.h"
#include "Helmsman.h"

BNO080        compassIMU;            // the compass object
Captain       captain(&compassIMU);  // the "brain" of the autopilot
Helmsman      helmsman(&captain);    // controlling the Motor
HMI           hmi(&captain);         // the Human Machine Interface

void setup()
{
  Serial.begin(9600);
  Serial.println("Captain autopilot");

  Wire.begin();
  Wire.setClock(400000); //Increase I2C data rate to 400kHz

  compassIMU.begin();
  compassIMU.enableRotationVector(10); //Send data update every 50ms

  captain.loadSettings();
}

void loop()
{
  hmi.update();
  captain.updateHeading();
  helmsman.updateRudder();
}
