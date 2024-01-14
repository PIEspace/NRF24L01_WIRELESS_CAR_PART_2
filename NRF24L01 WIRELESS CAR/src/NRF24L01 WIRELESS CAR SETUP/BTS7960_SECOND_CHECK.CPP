//CONNECT THE BTS7960 MOTOR DRIVER TO STM32F013C8 

/*
connection :

stm32                bts7960 

VCC                    5V
GND                    GND
PA15                   L_EN
PB3                    R_EN
PB4                    LPWM
PB5                    RPWM

*/

#include<Arduino.h>
//define the bts7960 pin 

const int L_EN = PB12;
const int R_EN = PB13;
const int LPWM = PB14;
const int RPWM = PB15;


void setup(){

    pinMode(L_EN , OUTPUT);
    pinMode(R_EN , OUTPUT);
    pinMode(LPWM , OUTPUT);
    pinMode(RPWM , OUTPUT);

    digitalWrite(L_EN , HIGH);
    digitalWrite(R_EN ,HIGH);
    

}
void loop(){
    digitalWrite(L_EN , HIGH);
    digitalWrite(R_EN ,HIGH);

    digitalWrite(LPWM , HIGH);
    delay(1000);
    digitalWrite(LPWM , LOW);
    delay(1000);

    digitalWrite(RPWM , HIGH);
    delay(1000);
    digitalWrite(RPWM , LOW);
    delay(1000);

}