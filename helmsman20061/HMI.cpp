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
#include "HMI.h"

HMI::HMI( Captain* captain)
  :_captain(captain),
  _lcd(new LiquidCrystal(8, 9, 4, 5, 6, 7))
{
  _lcd->begin(40,4);
  _lcd->setCursor(0, 0);
  _lcd->print("AP v 01");  
  
}

void HMI::update()
{
  if (menuState == 1) mainSettings();
  if (menuState == 2) kPSettings();
  if (menuState == 3) kDSettings();
  if (menuState == 4) kISettings();
  if (menuState == 5) deadbandSettings();
  if (menuState == 6) sampleTimeSettings();
  if (menuState == 7) autoTuneSettings();
  if (menuState == 8) motorDirectionSettings();
  if (menuState == 9) maxMotorRunTime();
}

void HMI::mainSettings() //1
{
  if ((millis() - _lastDebounceTime) > 250) {
  _lcd->setCursor(0, 0);
  _lcd->print("HDG ");
  _lcd->print("       ");
  _lcd->setCursor(5, 0);
  _lcd->print(_captain->getHeading());
  _lcd->setCursor(0, 1);
  
  if(!_captain->getPowerState())
  {
    _lcd->print("AP OFF ");
  }else{
    _lcd->print("CRS ");
    _lcd->setCursor(5, 1);
    _lcd->print(_captain->getCourse());
  }

  
    int lcd_key = readButtons();
    if ( lcd_key == btnRIGHT)
    {
      menuState = 2;
      _lcd->clear();  
    }
    else if ( lcd_key == btnLEFT ) 
    {
      menuState = 9;
      _lcd->clear();
    }
    else if ( lcd_key == btnUP)
    {
      _captain->changeCourse(3);
      _lcd->clear();
    }
    else if ( lcd_key == btnDOWN)
    {
      _captain->changeCourse(-3);  
      _lcd->clear();
    }
    else if ( lcd_key == btnSELECT) 
    {
      _captain->togglePowerState();
      _lcd->clear();
    }
    else if ( lcd_key == btnNONE)
    {
    
    }
    _lastDebounceTime = millis();
  }
}

void HMI::kPSettings() //2
{
  _lcd->setCursor(0, 0);
  _lcd->print("HDG ");
  _lcd->print(_captain->getHeading());
  if(!_captain->getPowerState())
  {
    _lcd->print(" AP OFF ");
  }
  else{
     _lcd->print(" CRS ");
     _lcd->print(_captain->getCourse());
  }
  _lcd->setCursor(0, 1);
  _lcd->print("kP ");
  _lcd->print(_captain->getkP());
  

  if ((millis() - _lastDebounceTime) > 250) {
    int lcd_key = readButtons();
    if ( lcd_key == btnRIGHT)
    {
      menuState = 3;
      _lcd->clear();
    }
    else if ( lcd_key == btnLEFT ) 
    {
      menuState = 1;
      _lcd->clear();
    }
    else if ( lcd_key == btnUP)
    {
      _captain->setkP(_captain->getkP() + 0.1 );
      _lcd->clear();
    }
    else if ( lcd_key == btnDOWN)
    {
      _captain->setkP(_captain->getkP() +- 0.1 );
      _lcd->clear();
    }
    else if ( lcd_key == btnSELECT) 
    {
      _captain->togglePowerState();
      _lcd->clear();
    }
    else if ( lcd_key == btnNONE)
    {
    
    }
    _lastDebounceTime = millis();
  }
}

void HMI::kDSettings() //3
{
  _lcd->setCursor(0, 0);
  _lcd->print("HDG ");
  _lcd->print(_captain->getHeading());
  if(!_captain->getPowerState())
  {
    _lcd->print(" AP OFF ");
  }
  else
  {
     _lcd->print(" CRS ");
     _lcd->print(_captain->getCourse());
  }
  _lcd->setCursor(0, 1);
  _lcd->print("kD ");
  _lcd->print(_captain->getkD());
  

  if ((millis() - _lastDebounceTime) > 250) {
    int lcd_key = readButtons();
    if ( lcd_key == btnRIGHT)
    {
      menuState = 4;
      _lcd->clear();
    }
    else if ( lcd_key == btnLEFT ) 
    {
      menuState = 2;
      _lcd->clear();
    }
    else if ( lcd_key == btnUP)
    {
      _captain->setkD(_captain->getkD() + .1 );
      _lcd->clear();
    }
    else if ( lcd_key == btnDOWN)
    {
      _captain->setkD(_captain->getkD() - .1 );
      _lcd->clear();
    }
    else if ( lcd_key == btnSELECT) 
    {
      _captain->togglePowerState();
      _lcd->clear();
    }
    else if ( lcd_key == btnNONE)
    {
    
    }
    _lastDebounceTime = millis();
  }
}

void HMI::kISettings() //4
{
  _lcd->setCursor(0, 0);
  _lcd->print("HDG ");
  _lcd->print(_captain->getHeading());
  if(!_captain->getPowerState())
  {
    _lcd->print(" AP OFF ");
  }
  else
  {
     _lcd->print(" CRS ");
     _lcd->print(_captain->getCourse());
  }
  _lcd->setCursor(0, 1);
  _lcd->print("kI ");
  _lcd->print(_captain->getkI());
  

  if ((millis() - _lastDebounceTime) > 250) {
    int lcd_key = readButtons();
    if ( lcd_key == btnRIGHT)
    {
      menuState = 5;
      _lcd->clear();
    }
    else if ( lcd_key == btnLEFT ) 
    {
      menuState = 3;
      _lcd->clear();
    }
    else if ( lcd_key == btnUP)
    {
      _captain->setkI(_captain->getkI() + .1 );
      _lcd->clear();
    }
    else if ( lcd_key == btnDOWN)
    {
      _captain->setkI(_captain->getkI() - .1 );
      _lcd->clear();
    }
    else if ( lcd_key == btnSELECT) 
    {
      _captain->togglePowerState();
      _lcd->clear();
    }
    else if ( lcd_key == btnNONE)
    {
    
    }
    _lastDebounceTime = millis();
  }
}


void HMI::deadbandSettings() //5
{
  _lcd->setCursor(0, 0);
  _lcd->print("HDG ");
  _lcd->print(_captain->getHeading());
  if(!_captain->getPowerState())
  {
    _lcd->print(" AP OFF ");
  }
  else
  {
     _lcd->print(" CRS ");
     _lcd->print(_captain->getCourse());
  }
  _lcd->setCursor(0, 1);
  _lcd->print("Deadband ");
  _lcd->print((int)_captain->getDeadband());
  

  if ((millis() - _lastDebounceTime) > 250) {
    int lcd_key = readButtons();
    if ( lcd_key == btnRIGHT)
    {
      menuState = 6;
      _lcd->clear();
    }
    else if ( lcd_key == btnLEFT ) 
    {
      menuState = 4;
      _lcd->clear();
    }
    else if ( lcd_key == btnUP)
    {
      _captain->setDeadband(_captain->getDeadband() + 1 );
      _lcd->clear();
    }
    else if ( lcd_key == btnDOWN)
    {
     if (_captain->getDeadband() > 0) _captain->setDeadband(_captain->getDeadband() - 1 );
      _lcd->clear();
    }
    else if ( lcd_key == btnSELECT) 
    {
      _captain->togglePowerState();
      _lcd->clear();
    }
    else if ( lcd_key == btnNONE)
    {
    
    }
    _lastDebounceTime = millis();
  }
}

void HMI::sampleTimeSettings() //6
{
  _lcd->setCursor(0, 0);
  _lcd->print("HDG ");
  _lcd->print(_captain->getHeading());
  if(!_captain->getPowerState())
  {
    _lcd->print(" AP OFF ");
  }
  else
  {
     _lcd->print(" CRS ");
     _lcd->print(_captain->getCourse());
  }
  _lcd->setCursor(0, 1);
  _lcd->print("Sample T ");
  _lcd->print((int)_captain->getSampleTime());
  

  if ((millis() - _lastDebounceTime) > 250) {
    int lcd_key = readButtons();
    if ( lcd_key == btnRIGHT)
    {
      menuState = 7;
      _lcd->clear();
    }
    else if ( lcd_key == btnLEFT ) 
    {
      menuState = 5;
      _lcd->clear();
    }
    else if ( lcd_key == btnUP)
    {
      _captain->setSampleTime(_captain->getSampleTime() + 100 );
      _lcd->clear();
    }
    else if ( lcd_key == btnDOWN)
    {
     if (_captain->getSampleTime() > 0) _captain->setSampleTime(_captain->getSampleTime() - 100 );
      _lcd->clear();
    }
    else if ( lcd_key == btnSELECT) 
    {
      _captain->togglePowerState();
      _lcd->clear();
    }
    else if ( lcd_key == btnNONE)
    {
    
    }
    _lastDebounceTime = millis();
  }
}


void HMI::autoTuneSettings() //7
{
  if(_captain->getPowerState())
  {
    _lcd->setCursor(0, 0);
    _lcd->print(_captain->getHeading());
    _lcd->print(" ");
    _lcd->print(_captain->getCourse());
    if(!_captain->getAutoTune()){
      _lcd->print(" P ");
      _lcd->print(_captain->getkP(),2);
      _lcd->setCursor(0, 1);
      _lcd->print("I ");
      _lcd->print(_captain->getkI(),2);
      _lcd->print(" D ");
      _lcd->print(_captain->getkD(),2);
     } else {
      _lcd->setCursor(0, 1);
      _lcd->print("AutoTune "); 
      _captain->getAutoTune() ? _lcd->print("Off") : _lcd->print("On");
    }
  }
  else
  {
    _lcd->setCursor(0, 0);
    _lcd->print(_captain->getHeading());
    _lcd->print(" AP OFF ");
    _lcd->setCursor(0, 1);
    _lcd->print("AutoTune "); 
    _captain->getAutoTune() ? _lcd->print("Off") : _lcd->print("On");
  }

  if ((millis() - _lastDebounceTime) > 250) {
    int lcd_key = readButtons();
    if ( lcd_key == btnRIGHT)
    {
      menuState = 8;
      _lcd->clear();
    }
    else if ( lcd_key == btnLEFT ) 
    {
      menuState = 6;
      _lcd->clear();
    }
    else if ( lcd_key == btnUP)
    {
      _captain->setAutoTune(true);
      _lcd->clear();
    }
    else if ( lcd_key == btnDOWN)
    {
     _captain->setAutoTune(false);
      _lcd->clear();
    }
    else if ( lcd_key == btnSELECT) 
    {
      _captain->togglePowerState();
      _lcd->clear();
    }
    else if ( lcd_key == btnNONE)
    {
    
    }
    _lastDebounceTime = millis();
  }
}

void HMI::motorDirectionSettings() //8
{
  _lcd->setCursor(0, 0);
  _lcd->print("HDG ");
  _lcd->print(_captain->getHeading());
  if(!_captain->getPowerState())
  {
    _lcd->print(" AP OFF ");
  }
  else
  {
     _lcd->print(" CRS ");
     _lcd->print(_captain->getCourse());
  }
  _lcd->setCursor(0, 1);
  _lcd->print("M. dir: ");
  _captain->getMotorDirection() ? _lcd->print("Normal") : _lcd->print("Reversed");
  

  if ((millis() - _lastDebounceTime) > 250) {
    int lcd_key = readButtons();
    if ( lcd_key == btnRIGHT)
    {
      menuState = 9;
      _lcd->clear();
    }
    else if ( lcd_key == btnLEFT ) 
    {
      menuState = 7;
      _lcd->clear();
    }
    else if ( lcd_key == btnUP)
    {
      _captain->setMotorDirection(true);
      _lcd->clear();
    }
    else if ( lcd_key == btnDOWN)
    {
     _captain->setMotorDirection(false);
      _lcd->clear();
    }
    else if ( lcd_key == btnSELECT) 
    {
      _captain->togglePowerState();
      _lcd->clear();
    }
    else if ( lcd_key == btnNONE)
    {
    
    }
    _lastDebounceTime = millis();
  }
}

void HMI::maxMotorRunTime() //9
{
  _lcd->setCursor(0, 0);
  _lcd->print("HDG ");
  _lcd->print(_captain->getHeading());
  if(!_captain->getPowerState())
  {
    _lcd->print(" AP OFF ");
  }
  else
  {
     _lcd->print(" CRS ");
     _lcd->print(_captain->getCourse());
  }
  _lcd->setCursor(0, 1);
  _lcd->print("M. time(%): ");
  _lcd->print(_captain->getMaxMotorRunTime() );
  

  if ((millis() - _lastDebounceTime) > 250) {
    int lcd_key = readButtons();
    if ( lcd_key == btnRIGHT)
    {
      menuState = 1;
      _lcd->clear();
    }
    else if ( lcd_key == btnLEFT ) 
    {
      menuState = 8;
      _lcd->clear();
    }
    else if ( lcd_key == btnUP)
    {
      _captain->setMaxMotorRunTime(10);
      _lcd->clear();
    }
    else if ( lcd_key == btnDOWN)
    {
     _captain->setMaxMotorRunTime(-10);
      _lcd->clear();
    }
    else if ( lcd_key == btnSELECT) 
    {
      _captain->togglePowerState();
      _lcd->clear();
    }
    else if ( lcd_key == btnNONE)
    {
    
    }
    _lastDebounceTime = millis();
  }
}

/*
int HMI::readButtons() { 

  int adc_key_in = analogRead(0);
  if (adc_key_in > 1000) return btnNONE;    
  if (adc_key_in < 100)  return btnRIGHT;
  if (adc_key_in < 170)  return btnUP;
  if (adc_key_in < 450)  return btnDOWN;
  if (adc_key_in < 650)  return btnLEFT;
  if (adc_key_in < 998)  return btnSELECT;

  return btnNONE;  
}
*/
int HMI::readButtons() { 
  
  int adc_key_in = analogRead(0);
  if (adc_key_in > 1000) return btnNONE;    
  if (adc_key_in < 50)   return btnRIGHT;
  if (adc_key_in < 150)  return btnUP;
  if (adc_key_in < 300)  return btnDOWN;
  if (adc_key_in < 500)  return btnLEFT;
  if (adc_key_in < 700)  return btnSELECT;

  return btnNONE;  
}
