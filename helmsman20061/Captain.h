/*
   Captain.h

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
#ifndef Captain_h
#define Captain_h
#include "Arduino.h"
#include "SparkFun_BNO080_Arduino_Library.h"
#include <PID_SGD.h>
#include <EEPROM.h>

class Captain
{
  public:
    Captain(BNO080* compassIMU);
    int                 updateHeading(),                     
                        getHeading(),                        
                        getDeadband(),
                        getSampleTime(),                    
                        getMaxMotorRunTime(),
                        getCourse(),
                        output();
                        
    double              getkP(),
                        getkI(),
                        getkD();
    
    void                setkP( double kP ),
                        setkI(double kI),
                        setkD(double kD),
                        setDeadband (int deadband),
                        setSampleTime(int sampleTime),
                        setAutoTune(bool state),
                        setMotorDirection(bool state),
                        setMaxMotorRunTime(int change),
                        loadSettings();
                        
                        

    bool                changeCourse( int change ),
                        getAutoTune(),
                        getMotorDirection(),
                        togglePowerState(),
                        getPowerState();

    

  private:
    BNO080*              _compassIMU;
    PID_SGD*             _apPID;
    
    double              _courseError,_output, _prevOutput;

    int                 _heading,
                        _course,
                        _previousHeading,
                        _ramp;
    
    float               _courseErrorFloat;
    
    int                 courseError(),skipT;
    

    void                setCourse(),
                        setNextSampleTime(),
                        saveSettings(),
                        logging();

    unsigned long       _nextSampleTime,
                        _rateOfTurnTimer;

    struct settings  {
      int               sampleTime,                                       
                        deadband,
                        maxMotorRunTime;

      float             kP,
                        kI,
                        kD;
      
      const int         maxCourseError = 100;


      bool              powerState = false,
                        motorDirection = true;
    }_settings;

    bool                autoTuning = true;
        

};
#endif
