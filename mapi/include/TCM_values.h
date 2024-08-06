

#include <cstdint>



struct TypeTCM {
    struct ActualValues {
        int32_t DELAY_A         :16,  //┐
                                :16,  //┘00
                DELAY_C         :16,  //┐
                                :16,  //┘01
                LASER_DELAY     :16,  //┐
                                :16;  //┘02
        uint32_t attenSteps      :14,  //┐
                attenBusy       : 1,  //│
                attenNotFound   : 1,  //│03
                                :16,  //┘
                EXT_SW          : 4,  //┐
                                :28;  //┘04
        int32_t  boardTemperature:16,  //┐
                                :16,  //┘05
                                :32;  //]06
        uint32_t boardType       : 2,  //┐
                                : 6,  //│
                SERIAL_NUM      : 8,  //│07
                                :16;  //┘
        int32_t  VTIME_LOW       :10,  //┐
                                :22,  //┘08
                VTIME_HIGH      :10,  //┐
                                :22;  //┘09
        uint32_t T2_LEVEL_A      :16,  //┐
                                :16,  //┘0A
                T2_LEVEL_C      :16,  //┐
                                :16,  //┘0B
                T1_LEVEL_A      :16,  //┐
                                :16,  //┘0C
                T1_LEVEL_C      :16,  //┐
                                :16,  //┘0D
                ADD_C_DELAY     : 1,  //┐
                sidesCombMode   : 2,  //│
                EXTENDED_READOUT: 1,  //│
                corrCNTselect   : 4,  //│0E
                SC_EVAL_MODE    : 1,  //│
                FV0_MODE        : 1,  //│
                                :22,  //┘
                PLLlockC        : 1,  //┐
                PLLlockA        : 1,  //│
                systemRestarted : 1,  //│
                externalClock   : 1,  //│
                GBTRxReady      : 1,  //│
                GBTRxError      : 1,  //│
                GBTRxPhaseError : 1,  //│0F
                BCIDsyncLost    : 1,  //│
                droppingHits    : 1,  //│
                resetCounters   : 1,  //│
                forceLocalClock : 1,  //│
                resetSystem     : 1,  //│
                PMstatusChanged :20;  //┘
    } act;
    struct TempValues {
        unsigned int actualValues: 32;
                unsigned int boardTemp:    16;
                unsigned int boardType:    16;
                int delayA:                16;
                int delayC:                16;
                int delayLaser:            32;
                int mainPanel:             16;
                int trigger1rand:          32;
                int trigger1sign:          16;
                int trigger1cnt:           32;
                int trigger2rand:          32;
                int trigger2sign:          16;
                int trigger2cnt:           32;
                int trigger3rand:          32;
                int trigger3sign:          16;
                int trigger3cnt:           32;
                int trigger4rand:          32;
                int trigger4sign:          16;
                int trigger4cnt:           32;
                int trigger5rand:          32;
                int trigger5sign:          16;
                int trigger5cnt:           32;
                int triggers:              16;
                long long pmA0Status:      64;
                long long pmA1Status:      64;
                long long pmA2Status:      64;
                long long pmA3Status:      64;
                long long pmA4Status:      64;
                long long pmA5Status:      64;
                long long pmA6Status:      64;
                long long pmA7Status:      64;
                long long pmA8Status:      64;
                long long pmA9Status:      64;
                long long pmC0Status:      64;
                long long pmC1Status:      64;
                long long pmC2Status:      64;
                long long pmC3Status:      64;
                long long pmC4Status:      64;
                long long pmC5Status:      64;
                long long pmC6Status:      64;
                long long pmC7Status:      64;
                long long pmC8Status:      64;
                long long pmC9Status:      64;
                int vtimeLow:              32;
                int vtimeHigh:             32;
                int scLevelA:              32;
                int scLevelC:              32;
                int cLevelA:               32;
                int cLevelC:               32;
                long long mainPanelBits:   64;
    } temp;
};

extern TypeTCM tcm;