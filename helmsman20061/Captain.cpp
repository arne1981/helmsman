/*
   Captain.cpp

   Helmsman autopilot a yacht autopilot for holding a set course.

   This class is the brain of the Helmsman autopilot.
   Based on the set course & current heading the desired rate of turn is calculated( rateOfTurnSetpoint ).


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
#include "Captain.h"

Captain::Captain(BNO080* compassIMU)
  :_compassIMU(compassIMU),
  _apPID( new PID_SGD(&_courseError, &_output, 0, 1, DIRECT, 0.1, 0.0005))
{
  _apPID->SetMode(AUTOMATIC);
  _apPID->SetOutputLimits(-30,30);
  _apPID->SetLearningFlag(autoTuning);
}
  
int Captain::updateHeading()
{
  if (_compassIMU->dataAvailable() == true)
  { 
    float x, y, yaw;
    float i = _compassIMU->getQuatI();
    float j = _compassIMU->getQuatJ();
    float k = _compassIMU->getQuatK();
    float real = _compassIMU->getQuatReal();
    
    // YAW 
    x = 2 * ((i * j) + (real * k)); 
    y = sq(real) - sq(k) - sq(j) + sq(i); 
    yaw = degrees(atan2(y, x));
    
    if( yaw < 0 ) yaw += 360.0;
    _heading = (int)yaw;

  }
  return _heading; 
}

int Captain::courseError()
{
  int courseError = (_heading - _course + 540) % 360 - 180;

  _courseError = courseError;
  return courseError;
}

int Captain::output()
{
  courseError();
  if (_settings.deadband >= abs( _courseError)) _courseError = 0;
  
  if (_apPID->Compute()) {  
    _settings.kP = _apPID->GetKp();
    _settings.kD = _apPID->GetKd();
    _settings.kI = _apPID->GetKi();
   
    _ramp = (int)_output - (int)_prevOutput;  //ramping output
        
    _prevOutput = _output;
    if(_ramp!=0) logging();

   
    return _ramp;
  }
  return 0;
  
}

void Captain::setCourse()
{
  _course = _heading;
  _previousHeading = _heading;

}

bool Captain::changeCourse(int change)
{
  _course = _course + change;
  if ( _course > 360 ) _course = _course - 360;
  if ( _course < 1 )   _course = _course + 360;

  Serial.print("course:");
  Serial.println(_course);
}

void Captain::setkP(double kP)
{
  _settings.kP = kP;
  saveSettings();
}

double Captain::getkP() {
  return _settings.kP;
}

void Captain::setkD(double kD)
{
  _settings.kD = kD;
  saveSettings();
}

double  Captain::getkD() {
  return _settings.kD;
}

void Captain::setkI(double kI)
{
  _settings.kI = kI;
  saveSettings();
}

double  Captain::getkI() {
  return _settings.kI;
}

void Captain::setDeadband(int deadband)
{
  _settings.deadband = deadband;
  saveSettings();
}

int Captain::getDeadband() {
  return _settings.deadband;
}

void Captain::setSampleTime(int sampleTime)
{
  _settings.sampleTime = sampleTime;
  saveSettings();
}

void Captain::setMaxMotorRunTime(int change)
{
  _settings.maxMotorRunTime = _settings.maxMotorRunTime + change;
  if(_settings.maxMotorRunTime > 100) _settings.maxMotorRunTime = 100;
  if(_settings.maxMotorRunTime < 10) _settings.maxMotorRunTime = 10;
  saveSettings();
}

int  Captain::getMaxMotorRunTime() {
  return _settings.maxMotorRunTime;
}


bool Captain::togglePowerState()
{
  setCourse();
  _settings.powerState = !_settings.powerState;
    delete _apPID;
    _apPID = new PID_SGD(&_courseError, &_output, 0, 1, DIRECT, 0.1, 0.001);
    _apPID->SetMode(AUTOMATIC);
    _apPID->SetOutputLimits(-30,30);
    _apPID->SetLearningFlag(autoTuning);  
    _apPID->SetTunings(_settings.kP, _settings.kI, _settings.kD);
    _apPID->SetSampleTime(_settings.sampleTime);
    if(!_settings.motorDirection)_apPID->SetControllerDirection(REVERSE);
    if(_settings.motorDirection)_apPID->SetControllerDirection(DIRECT);
  
  if (!_settings.powerState) saveSettings();
  Serial.print("Switched; Power state = ");
  Serial.println(_settings.powerState);
}

bool Captain::getPowerState() {
  return  _settings.powerState;
}
int Captain::getHeading() {
  return _heading;
}
int Captain::getCourse() {
  return _course;
}

int Captain::getSampleTime() {
  return _settings.sampleTime;
}

void Captain::setAutoTune(bool state)
{
  autoTuning = state;
  _apPID->SetLearningFlag(!state);
}

bool Captain::getAutoTune() {
  return autoTuning;
}


void Captain::setMotorDirection(bool state)
{
  _settings.motorDirection = state;
  saveSettings();
}

bool Captain::getMotorDirection() {
  return _settings.motorDirection;
}
void Captain::loadSettings()
{
  
  EEPROM.get(0, _settings.kP); if(isnan(_settings.kP)) _settings.kP= 1.0;
  EEPROM.get(10, _settings.kD); if(isnan(_settings.kD)) _settings.kD= 0.5;
  EEPROM.get(20, _settings.kI); if(isnan(_settings.kI)) _settings.kI= 0.2;
  EEPROM.get(30, _settings.sampleTime); if(isnan(_settings.sampleTime)) _settings.sampleTime= 4000;
  EEPROM.get(40, _settings.deadband); if(isnan(_settings.deadband)) _settings.deadband= 2;
  EEPROM.get(50, _settings.motorDirection); if(isnan(_settings.motorDirection)) _settings.motorDirection= true;
  EEPROM.get(60, _settings.maxMotorRunTime); if(isnan(_settings.maxMotorRunTime)) _settings.maxMotorRunTime= 100;
  
  if(!_settings.motorDirection)_apPID->SetControllerDirection(REVERSE);
  if(_settings.motorDirection)_apPID->SetControllerDirection(DIRECT);
  _apPID->SetTunings(_settings.kP, _settings.kI, _settings.kD);
  _apPID->SetSampleTime(_settings.sampleTime);
}

void Captain::saveSettings()
{ 
  
  // Set the values 
  EEPROM.put(0, _settings.kP);
  EEPROM.put(10, _settings.kD);
  EEPROM.put(20, _settings.kI);
  EEPROM.put(30, _settings.sampleTime);
  EEPROM.put(40, _settings.deadband); 
  EEPROM.put(50, _settings.motorDirection); 
  EEPROM.put(60, _settings.maxMotorRunTime); 
  EEPROM.update(100, 100);  
  
  if(!_settings.motorDirection)_apPID->SetControllerDirection(REVERSE);
  if(_settings.motorDirection)_apPID->SetControllerDirection(DIRECT);
  _apPID->SetTunings(_settings.kP, _settings.kI, _settings.kD);
  _apPID->SetSampleTime(_settings.sampleTime);

  
  Serial.println("settings saved");
}

void Captain::logging()
{ 
  Serial.print(" Course: ");
  Serial.print(_course);
  Serial.print(" Heading: ");
  Serial.print(_heading);
  Serial.print(" Error: ");
  Serial.print(_courseError);
  Serial.print(" Output: ");
  Serial.print(_output);
  Serial.print(" Ramp: ");
  Serial.print(_ramp);
  Serial.println(" ");
  
  String dataString = "";
  dataString += String(_course);
  dataString += ",";
  dataString += String(_heading);
  dataString += ",";
  dataString += String(_courseError);
  dataString += ",";
  dataString += String(_output);
  dataString += ",";
  dataString += String(_ramp);
  
 // Serial.println(dataString);   
}

/*
void Captain::loadSettings()
{

  // Open file for reading
    File file = SD.open("/config.txt");
  
  // Allocate a temporary JsonDocument
  // Don't forget to change the capacity to match your requirements.
  // Use arduinojson.org/v6/assistant to compute the capacity.
  StaticJsonDocument<1024> doc;
  
  // Deserialize the JSON document
   DeserializationError error = deserializeJson(doc, file);
  if (error) Serial.println(F("Failed to read file, using default configuration"));
  
  // Copy values from the JsonDocument to the Config
  _settings.kP = doc["kP"] | 1.0;
  _settings.kD = doc["kD"] | 0.5;
  _settings.kI = doc["kI"] | 0.2;
  _settings.sampleTime = doc["sampleTime"] | 4000;
  _settings.deadband = doc["deadband"] | 2;
  _settings.motorDirection = doc["motorDirection"] | true;
    
  if(!_settings.motorDirection)_apPID->SetControllerDirection(REVERSE);
  if(_settings.motorDirection)_apPID->SetControllerDirection(DIRECT);
  _apPID->SetTunings(_settings.kP, _settings.kI, _settings.kD);
  _apPID->SetSampleTime(_settings.sampleTime);
 
  file.close();
    
}

void Captain::saveSettings()
{ 
  SD.remove("/config.txt");
  
  // Open file for writing
  File file = SD.open("/config.txt", FILE_WRITE);
  if (!file) {
    Serial.println(F("Failed to create file"));
    return;
  }
  
  // Allocate a temporary JsonDocument
  // Don't forget to change the capacity to match your requirements.
  // Use arduinojson.org/assistant to compute the capacity.
  StaticJsonDocument<1024> doc;
  
  // Set the values in the document
  doc["kP"] = _settings.kP;
  doc["kD"] = _settings.kD;
  doc["kI"] = _settings.kI;
  doc["sampleTime"] =_settings.sampleTime;
  doc["deadband"] = _settings.deadband;
  doc["motorDirection"] = _settings.motorDirection;
    
  if(!_settings.motorDirection)_apPID->SetControllerDirection(REVERSE);
  if(_settings.motorDirection)_apPID->SetControllerDirection(DIRECT);
  _apPID->SetTunings(_settings.kP, _settings.kI, _settings.kD);
  _apPID->SetSampleTime(_settings.sampleTime);
    
  // Serialize JSON to file
  if (serializeJson(doc, file) == 0) {
    Serial.println(F("Failed to write to file"));
  }
  Serial.println("settings saved");
  file.close();
}

void Captain::logging()
{
  String dataString = "";
  dataString += String(_course);
  dataString += ",";
  dataString += String(_heading);
  dataString += ",";
  dataString += String(_courseError);
  dataString += ",";
  dataString += String(_output);
  dataString += ",";
  dataString += String(_ramp);
  
  File dataFile = SD.open("datalog.txt", FILE_WRITE); 
  if (dataFile) {
    dataFile.println(dataString);
    dataFile.close();
    // print to the serial port too:
    Serial.println(dataString);
  }
  // if the file isn't open, pop up an error:
  else {
    Serial.println("error opening datalog.txt");
  } 
}
*/
