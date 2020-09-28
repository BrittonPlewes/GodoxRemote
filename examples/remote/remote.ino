/*

Simple flash and fade through the brightness values on the light

*/



#include "GodoxRemote.h"

// initiates remote with transmitter DIN attached to arduino pin 4
GodoxRemote remote(4);


void setup(){
    Serial.begin(9600);
    Serial.println("ready");

    // change these values to affect different light channels/groups
    remote.setChannel(0); //1
    remote.setGroup(0);   //A


    //number of times to repeat signal when sent
    //remote repeats 4-5 times per button press
    remote.setRepeatNum(2);    

    // delay between repetitions in ms
    // remote uses 35-45ms, but 10-25 can be used reliably for faster response
    remote.setRepeatDelay(25); 
}


void loop(){
    Serial.println("fade up!");

    // fade light values up
    for( int i=25; i<=100; i+=5 ){
        remote.setBrightness(i);
        delay(50);
    }

    // off/on twice
    delay(500);    
    remote.toggle(0);
    Serial.println("off");

    delay(500);
    remote.toggle(1);
    Serial.println("on");

    delay(500);
    remote.toggle(0);
    Serial.println("off");

    delay(500);
    remote.toggle(1); 
    Serial.println("on");   


    Serial.println("fade down...");
    // fade light values down
    for( int i=100; i>25; i-=5 ){
        remote.setBrightness(i);
        delay(50);
    }
}


