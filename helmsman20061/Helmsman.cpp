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
#include "Helmsman.h"

Helmsman::Helmsman (Captain* captain)
  : _captain(captain)
{  
}

void Helmsman::updateRudder()
{
  if (_captain->getPowerState())
  {
    double motorSpeed;
    double runTime;
    
    if(_output ==0 && _motorDirection !=3) 
    {
      _output = _captain->output();
    }
    
    if (_output < 0 && _motorDirection !=1)
    {
      motorSpeed = 105 + int(abs(_output))*2.5; 
      runTime = 100 + (abs(_output) * ((_captain->getSampleTime()-100)/15));
      runTime = runTime * ((double)_captain->getMaxMotorRunTime()/100);
      if (runTime > _captain->getSampleTime()) runTime = _captain->getSampleTime() + 50;
      _motorRunTime = (int)runTime + millis();
      _motor.setMotor('L', (int)motorSpeed); //Turn left
      _motorDirection = -1; 
      _output = 0;
      Serial.print("Turn left, runtime:");
      Serial.print(runTime);
      Serial.print(" Speed: ");
      Serial.println(motorSpeed);
    }
    if (_output > 0 && _motorDirection !=-1) // turn motor right
    {
      motorSpeed = 105 + int(abs(_output))*2.5; 
      
      runTime = 100 + (abs(_output) * ((_captain->getSampleTime()-100)/15));
      runTime = runTime * ((double)_captain->getMaxMotorRunTime()/100);

      if (runTime > _captain->getSampleTime()) runTime = _captain->getSampleTime() + 50;
     
      _motorRunTime = (int)runTime + millis();
      _motor.setMotor('R', (int)motorSpeed);  
      _motorDirection = 1; 
      _output = 0;
      
      Serial.print("Turn right, runtime:");
      Serial.print(runTime);
      Serial.print(" Speed: ");
      Serial.println(motorSpeed);

    }
    if (_motorRunTime < millis() && _motorDirection != 3 && _motorDirection !=0)  // breaking
    {
      _motor.closeMotor('R');
      _motor.closeMotor('L');
      //_motorRunTime = millis() + 300;
      _motorDirection = 3; 
      
      //Serial.println("Breaking");
    }
    if (_motorRunTime < millis() && _motorDirection == 3) // set motor to stopped 
    {
      _motorDirection = 0; 
      //Serial.println("Stopped");
    }
  } else
  {
    _motorDirection = 0;
    _motor.closeMotor('R');
    _motor.closeMotor('L'); // stop motor when autopilot is standby mode
  }
}
