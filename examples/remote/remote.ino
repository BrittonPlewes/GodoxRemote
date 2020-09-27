/*

Simple flash and run through the brightness values on the light

*/



#include "GodoxRemote.h"

GodoxRemote remote(4);


void setup(){
    Serial.begin(9600);
    Serial.println("ready");

    // change these values to affect different light channels/groups
    remote.setChannel(0); //1
    remote.setGroup(0);   //A

    remote.setRepeatDelay(25); //remote uses 35-45
    remote.setRepeatNum(2);    //remote uses 4-5
}


void loop(){
    // fade light values up
    for( int i=25; i<=100; i+=5 ){
        remote.setBrightness(i);
        delay(50);
    }

    // off/on twice
    delay(500);
    remote.toggle(0);

    delay(500);
    remote.toggle(1);

    delay(500);
    remote.toggle(0);    

    delay(500);
    remote.toggle(1);    

    // fade light values down
    for( int i=100; i>25; i-=5 ){
        remote.setBrightness(i);
        delay(50);
    }
}


