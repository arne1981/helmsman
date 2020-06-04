/*
 * IBT2.cpp - Library for easy use of the IBT-2 Motor driver.
 *
 * https://github.com/
 *
 *
 * This code is released under the terms of the LGPLv3 licence.
 */

#include "Arduino.h"
#include "IBT2.h"


IBT2::IBT2() {
    IBT2::setPWMfrequency(0x02);  // timer 0 , 3.92KHz
    pinMode(_R_EN,OUTPUT);
    digitalWrite(_R_EN,LOW);
    pinMode(_RPWM,OUTPUT);
    digitalWrite(_RPWM,LOW);
    pinMode(_LPWM,OUTPUT);
    digitalWrite(_LPWM,LOW);
    pinMode(_L_EN,OUTPUT);
    digitalWrite(_L_EN,LOW);

    delay(1000);
    IBT2::MotorActiveStatus('R',true);
    IBT2::MotorActiveStatus('L',true);
}


void IBT2::setPWMfrequency(int freq)
{
    TCCR0B = TCCR0B & 0b11111000 | freq ;
}
 
void IBT2::MotorActiveStatus(char Side,boolean s)
{
    boolean state=s;
    if(Side=='R'){
        digitalWrite(_R_EN,s);
    }
    if(Side=='L'){
        digitalWrite(_L_EN,s);
  }    
}

void IBT2::setMotor(char side,byte pwm)
{
    if(side=='R'){
        analogWrite(_RPWM,pwm);
    }
    if(side=='L'){
        analogWrite(_LPWM,pwm);
    }
}

void IBT2::closeMotor(char side)
{
    if(side=='R'){
        digitalWrite(_RPWM,LOW);
    }
    if(side=='L'){
        digitalWrite(_LPWM,LOW);
    }
}
