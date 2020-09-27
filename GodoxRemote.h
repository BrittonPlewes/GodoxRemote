
#ifndef GodoxRemote_h
#define GodoxRemote_h


class GodoxRemote {
    public:
        GodoxRemote( uint8_t p );

        void setChannel( uint8_t chan );
        void setGroup( uint8_t group );

        void setBrightness( int val );
        void toggle( bool tog );

        void setRepeatDelay( int r );
        void setRepeatNum( int n );

    private:
        uint8_t _pin;

        // group/channel/value
        uint8_t _channel;
        uint8_t _group;
        uint8_t _val;

        // timing
        int _repeatDelay;
        int _repeatNum;

        // commands
        void _setCmd( bool i, uint8_t cmd, uint8_t val, uint8_t chk );
        void _runCmd(uint8_t c, uint8_t repeat);

        bool _cmd[2][32] = {
            {0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0, 1,1,0,0, 0,0,0,0, 0,0,0,0},        
            {0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0, 1,1,0,0, 0,0,0,0, 0,0,0,0}
        };        
};


#endif
