/*
This is a simple library that uses a 433mhz RF antenna to "spoof" an RC-A5 remote, used by most consumer Godox camera lights.

LIMITATIONS
Currently this only works on channels 1/2, as the complete signal setup hasn't been decoded.
Also temperature has not been implemented yet, as I don't have a light that uses it to test (yet).



SIGNAL STRUCTURE    

 { GRP }  { CHAN } {    value     } {cmd} {  temp   }  {   checksum   }
[0,0,0,1, 0,0,0,0, 0,0,0,1, 1,0,0,1, 0,0, 0,0,1,1,0,0, 1,0,1,0, 0,1,0,1]  - set 1A brightness to 25
   A         1           25         val     4400k           ?????

I have *no* idea what the checksum corresponds to, it's not a simple checksum/crc8, and doesn't seem to be affected by cmd/temp...
These values were extracted manually, and stored in a lookup table in GodoxRemote_codes.h

*/



#include "Arduino.h"
#include "GodoxRemote_codes.h"
#include "GodoxRemote.h"



GodoxRemote::GodoxRemote( uint8_t p ){
    // set pin
    _pin = p;

    // set defaults
    setChannel(0);
    setGroup(0);

    // timing
    _repeatDelay = 40; //ms
    _repeatNum   = 3;

    pinMode( _pin, OUTPUT );
    digitalWrite( _pin, LOW );

}


// setters
void GodoxRemote::setChannel( uint8_t chan ){
    _channel = chan;
    // set the 4bit values
    for( int p=3; p>=0; p--){
        // channel
        _cmd[0][p+4] = bitRead( _channel, 3-p );
        _cmd[1][p+4] = bitRead( _channel, 3-p );
    }    
}

void GodoxRemote::setGroup( uint8_t group ){
    _group = group;

    for( int p=3; p>=0; p--){
        // group
        _cmd[0][p]   = bitRead( _group+1, 3-p ); 

        // toggle works for all group members in channel
        _cmd[1][p]   = bitRead( 255, 3-p ); 
    }        
}

void GodoxRemote::setRepeatDelay( int r ){
    _repeatDelay = r;
}

void GodoxRemote::setRepeatNum( int n ){
    _repeatNum = n;
}




void GodoxRemote::setBrightness( int val ){
    _val = val;

    _setCmd( 0, godox_cmds[2], val, godox_lightCodes[_channel][_group][_val-25] );

    // send the command
    _runCmd(0, _repeatNum);
}


void GodoxRemote::toggle( bool tog ){   
    _setCmd( 1, godox_cmds[tog], godox_toggleCodes[_channel][0], godox_toggleCodes[_channel][1] );

    if( tog ){
        setBrightness(_val);
    }
    
    _runCmd(1, _repeatNum);

    if( tog ){
        _runCmd(0, _repeatNum);
    }
}




/////////////////////////////////////////////////////////////////////////////////////////
// actual functions

void GodoxRemote::_setCmd (bool i, uint8_t cmd, uint8_t val, uint8_t chk ){
    // set the 4bit values
    for( int p=3; p>=0; p--){
        // command
        _cmd[i][p+16] = bitRead( cmd, 3-p );        
    }

    // set the 8bit values
    for( int p=7; p>=0; p--){
        // light value
        _cmd[i][p+8] = bitRead( val, 7-p);

        // checksum
        _cmd[i][p+24] = bitRead( chk, 7-p );
    }
}



void GodoxRemote::_runCmd(uint8_t c, uint8_t repeat){   
    for( int p=0; p<repeat; p++){ 
        digitalWrite(_pin, HIGH);
        delayMicroseconds(1250);

        for( int i=0; i<32; i++ ){
            digitalWrite(_pin, LOW);
            delayMicroseconds(godox_bits[ _cmd[c][i] ]);            
            digitalWrite(_pin, HIGH);
            delayMicroseconds(750);       
        }

        // last low bit
        digitalWrite(_pin, LOW);    
        delayMicroseconds(godox_bits[0]);            
        digitalWrite(_pin, HIGH);
        delayMicroseconds(750);   

        // delay
        digitalWrite(_pin, LOW);
        delay( _repeatDelay );     
    }
}
