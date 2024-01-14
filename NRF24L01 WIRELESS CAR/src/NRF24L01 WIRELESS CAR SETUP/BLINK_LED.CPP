//blink the led 

#include<Arduino.h>

//define the led pinout 
const int led = PC13;

void setup(){
    pinMode(led , OUTPUT);

}
void loop(){
    digitalWrite(led , HIGH); //turn on led 
    delay(1000); //delay for 1 second 
    digitalWrite(led , LOW); //turn off the led 
    delay(1000);   //delay for 1 second 

}