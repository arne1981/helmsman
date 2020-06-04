#include <IBT2.h>

IBT2 motor;

void setup()
{
}

void loop() {
    motor.setMotor('R',255); //turn right
    delay(1000);
    motor.closeMotor('R');
    delay(1000);
    motor.setMotor('L',255); //turn left
    delay(1000);
    motor.closeMotor('L');
    delay(1000);
}
