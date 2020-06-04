/*
 * IBT2.cpp - Library for easy use of the IBT-2 Motor driver.
 *
 * https://github.com/
 *
 * Copyright (C) 2015 Arne Hoffman
 *
 * This code is released under the terms of the LGPLv3 licence.
 */

#ifndef IBT2_h
#define IBT2_h

#include "Arduino.h"

#define   _R_EN 12
#define   _RPWM 11
#define   _LPWM 3
#define   _L_EN 2


class IBT2
{
  public:
    IBT2();    
    void setMotor(char side, byte pwm);
    void closeMotor(char side); 
    private:
    void setPWMfrequency(int freq);
    void MotorActiveStatus(char Side, boolean s);
};
#endif
