

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
};

extern TypeTCM tcm;