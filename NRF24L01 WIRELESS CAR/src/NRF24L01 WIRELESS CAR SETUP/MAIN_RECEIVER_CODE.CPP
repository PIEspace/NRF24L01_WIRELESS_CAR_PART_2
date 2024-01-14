//define the main receiver code in this code 
// i am using arduino nano development board 

#include<Arduino.h>
#include<SPI.h>
#include<nRF24L01.h>
#include<RF24.h>
#include<LiquidCrystal_I2C.h>


//define the liquid LiquidCrystal_I2C 
LiquidCrystal_I2C lcd (0x27 , 16 , 2 );

//define the CE and CNS pin 
const int CE_PIN = 7 ;
const int CNS_PIN = 8;

//define the radio
RF24 radio (CE_PIN , CNS_PIN);

//define the address 
const byte address[][7] = {"12345" , "123456"};

//define the joystick 
const int AngleX  = A0;
const int AngleY  = A3;

//define the switches 
const int Switch_ONE   = 2;
const int Switch_TWO   = 3;
const int Switch_THREE = 4;
const int Switch_FOUR  = 5;
const int Switch_FIVE  = 6;


//max size of this structure is 32 byte 
struct Data_Package
{
    byte PotX ;
    byte PotY ;
    byte button1;
    byte button2;
    byte button3;
    byte button4;
    byte button5;
};

Data_Package data ; //create a variable with the bove structure 


void first_lcd(void){
    
    lcd.setCursor(4,0);
    lcd.print("PIE.SPACE");
}


void setup(){
    //define the bud rate 
    //define the bits per second 
    Serial.begin(9600);
    radio.begin();
    radio.openWritingPipe(address[0]); //the address is = 12345
    radio.openReadingPipe(1 , address[1]); //the address is = 123456
    radio.setPALevel(RF24_PA_HIGH);

    //define the joystick 
    pinMode(AngleX , INPUT);
    pinMode(AngleY , INPUT);


    //define the switch pinout 
    pinMode(Switch_ONE   ,   INPUT);
    pinMode(Switch_TWO   ,   INPUT);
    pinMode(Switch_THREE ,   INPUT);
    pinMode(Switch_FOUR  ,   INPUT);
    pinMode(Switch_FIVE  ,   INPUT);

    //initilize the lcd display
    lcd.init();
    lcd.backlight();

    //run the functon 
    first_lcd();


//set the initial value 
data.button1 = 0;
data.button2 = 0;
data.button3 = 0;
data.button4 = 0;
data.button5 = 0;
data.PotX = 127 ; //the value from 0 to 255 . when joystick in resting position , the middule value is 127 
data.PotY = 127;

}

void loop(){
    
    radio.stopListening();
    

    //read all digital pin 
    data.button1 = digitalRead(Switch_ONE);
    data.button2 = digitalRead(Switch_TWO);
    data.button3 = digitalRead(Switch_THREE);
    data.button4 = digitalRead(Switch_FOUR);
    data.button5 = digitalRead(Switch_FIVE);


    //read all analog pin 
    data.PotX = map(analogRead(AngleX) , 0 , 1023 , 0 , 255);
    data.PotY = map(analogRead(AngleY) , 0 , 1023 , 0 , 255);



    radio.write(&data , sizeof(Data_Package));




}