//define the main transmitter code in this code 
//i am using bluepill development board 

#include<Arduino.h>
#include<SPI.h>
#include<nRF24L01.h>
#include<RF24.h>

//define the CE and CNS pin 
const int CE_PIN = PB0 ;
const int CNS_PIN = PA4;

//define the radio
RF24 radio (CE_PIN , CNS_PIN);

//define the address 
const byte address[][7] = {"12345" , "123456"};

//define the BTS7960 motor driver 

//define the fist motor driver 
const int L_EN_ONE = PB15;
const int R_EN_ONE = PB14;
const int LPWM_ONE = PB13;
const int RPWM_ONE = PA8;

//define the second motor driver 

const int L_EN_TWO = PB1;
const int R_EN_TWO = PA3;
const int LPWM_TWO = PA2;
const int RPWM_TWO = PA1;

//define the buzzer 
const int BUZZER = PC15;

//define the ultrosonic sensor 
//define the first ultrosonic sensor 
const int TRIG_PIN = PB10 ;
const int ECHO_PIN = PB11 ;


//define the second ultrosonic sensor 
const int TRIG_PIN_ONE = PA11 ;
const int ECHO_PIN_ONE = PA15 ;


//define the inbuild led 
const int INBUILD_LED =  PC13;

//max size of this structure is 32 bytes
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


//define the some value Buzzer_Status
int Buzzer_Status = 0;
int LEFT_RIGHT_VALUE , FORWARD_BACKWARD_VALUE ;

int MotorSpeedOne = 0;
int MotorSpeedTwo = 0;


unsigned long lastReceiveTime = 0;
unsigned long currentTime = 0;


//start tone 
void start_tone(void){
    for (int a = 0 ; a<= 10; a++){
        digitalWrite(BUZZER , HIGH);
        delay(100);

    }

    digitalWrite(BUZZER , LOW);
    delay(100);
}


//check first ultrosonic sensor 
void First_UltraSonic(void){
    digitalWrite(TRIG_PIN , LOW);
    delayMicroseconds(2);
    digitalWrite(TRIG_PIN , HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG_PIN , LOW);

    float Duration = pulseIn(ECHO_PIN , HIGH);
    float Distance = Duration * 0.0343 /2 ;

    //print on the serial monitor 
    for (int b  =0 ; b <= 10 ; b++){
        Serial.print("Distance_one = ");
        Serial.println(Distance);
    }

}

//check the second ultrasonic sensor
void Second_Ultrasonic(void){
    digitalWrite(TRIG_PIN_ONE , LOW);
    delayMicroseconds(2);
    digitalWrite(TRIG_PIN_ONE , HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG_PIN_ONE , LOW);

    float Duration_One = pulseIn(ECHO_PIN_ONE , HIGH);
    float Distance_One = Duration_One * 0.0343 /2 ;

    //print on the serial monitor 
    for (int c = 0 ; c<= 10 ; c++)
    {
        Serial.print("Distance_two = ");
        Serial.println(Distance_One);
    }

}



void setup(){

    //define the bud rate
    //in this case i am using 9600 bits per second 
    Serial.begin(9600);

    radio.begin();
    radio.openWritingPipe(address[1]);  // the address is = 123456 
    radio.openReadingPipe(1 , address[0]); //the address is 12345
    radio.setPALevel(RF24_PA_HIGH);

    resetdata();

    //define the pinMode function for motor driver 
    pinMode(L_EN_ONE , OUTPUT);
    pinMode(R_EN_ONE , OUTPUT);
    pinMode(LPWM_ONE , OUTPUT);
    pinMode(RPWM_ONE , OUTPUT);


    pinMode(L_EN_TWO , OUTPUT);
    pinMode(R_EN_TWO , OUTPUT);
    pinMode(LPWM_TWO , OUTPUT);
    pinMode(RPWM_TWO , OUTPUT);


    //define the buzzer pinMode
    pinMode(BUZZER , OUTPUT);

    //define the first ultrosonic value 
    pinMode(TRIG_PIN , OUTPUT);
    pinMode(ECHO_PIN , INPUT);

    //define the second ultrosonic value 
    pinMode(TRIG_PIN_ONE , OUTPUT);
    pinMode(ECHO_PIN_ONE , INPUT);

    //define the inbuild led 
    pinMode(INBUILD_LED , OUTPUT);


    //check the syatem at once a time
    Serial.println("CHECK ALL THE SYSTEM ");
    Serial.println("CHECK THE BUZZER ");
    start_tone();
    Serial.println("DONE");

    //check ultrosonic sensor 
    //define the first ultrasonic sensor 
    First_UltraSonic();
    //define the second ultrasonic sensor 
    Second_Ultrasonic();

    // enable pin _high 
    digitalWrite(L_EN_ONE , HIGH);
    digitalWrite(R_EN_ONE , HIGH);

    digitalWrite(L_EN_TWO , HIGH);
    digitalWrite(R_EN_TWO , HIGH);





}


void loop(){

    radio.startListening();

    //high the left and right motor enable pin 
    // enable pin _high 
    digitalWrite(L_EN_ONE , HIGH);
    digitalWrite(R_EN_ONE , HIGH);

    digitalWrite(L_EN_TWO , HIGH);
    digitalWrite(R_EN_TWO , HIGH);

    currentTime = millis();
    if ( currentTime - lastReceiveTime > 1000 ) { // If current time is more then 1 second since we have recived the last data, that means we have lost connection
    resetdata(); // If connection is lost, reset the data. It prevents unwanted behavior, for example if a drone jas a throttle up, if we lose connection it can keep flying away if we dont reset the function
  }


    if(radio.available()){
        radio.read(&data ,sizeof(Data_Package));
        lastReceiveTime = millis();
    }

    Buzzer_Status = data.button3;

    if(Buzzer_Status == LOW){
        digitalWrite(BUZZER , LOW);
        
    }
    else if (Buzzer_Status == HIGH)
    {
        digitalWrite(BUZZER , HIGH);
    }
     

    //receive the data from the joystic in the left_right_value and FORWARD_BACKWARD_VALUE to store the data 
    FORWARD_BACKWARD_VALUE = data.PotX;
    LEFT_RIGHT_VALUE= data.PotY;

    if(FORWARD_BACKWARD_VALUE < 110){
        digitalWrite(LPWM_ONE , HIGH);
        digitalWrite(RPWM_ONE , LOW);

        digitalWrite(LPWM_TWO , HIGH);
        digitalWrite(RPWM_TWO , LOW);

        // MotorSpeedOne = map(FORWARD_BACKWARD_VALUE , 110 , 0 , 0 , 255);
        // MotorSpeedTwo = map(FORWARD_BACKWARD_VALUE , 110 , 0 , 0 , 255);
    }

    else if (FORWARD_BACKWARD_VALUE > 140){
        digitalWrite(LPWM_ONE , LOW);
        digitalWrite(RPWM_ONE , HIGH);

        digitalWrite(LPWM_TWO , LOW);
        digitalWrite(RPWM_TWO , HIGH);

        // MotorSpeedOne = map(FORWARD_BACKWARD_VALUE , 140 , 255  , 0 , 255);
        // MotorSpeedTwo = map(FORWARD_BACKWARD_VALUE , 140 , 255 , 0 , 255);


    }

    else if(LEFT_RIGHT_VALUE < 110 ){
        digitalWrite(LPWM_ONE , LOW);
        digitalWrite(RPWM_ONE , HIGH);

        digitalWrite(LPWM_TWO , HIGH);
        digitalWrite(RPWM_TWO , LOW);

    }
    else if(LEFT_RIGHT_VALUE > 140){
        digitalWrite(LPWM_ONE , HIGH);
        digitalWrite(RPWM_ONE , LOW);

        digitalWrite(LPWM_TWO , LOW);
        digitalWrite(RPWM_TWO , HIGH);

    }

    else{
        // MotorSpeedOne = 0;
        // MotorSpeedTwo = 0;

        digitalWrite(LPWM_ONE , LOW);
        digitalWrite(RPWM_ONE , LOW);

        digitalWrite(LPWM_TWO , LOW);
        digitalWrite(RPWM_TWO , LOW);


    }

    // analogWrite(L_EN_ONE , MotorSpeedOne);
    // analogWrite(R_EN_ONE , MotorSpeedOne);

    // analogWrite(L_EN_TWO , MotorSpeedTwo);
    // analogWrite(R_EN_TWO , MotorSpeedTwo);


    
}

void resetdata(){
    data. PotX  = 127;
    data. PotY  = 127;
    data. button1 = 0 ;
    data. button2 = 0 ;
    data. button3 = 0 ;
    data. button4 = 0 ;
    data. button5 = 0 ;
}
