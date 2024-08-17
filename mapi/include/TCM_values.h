

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
        int electronicStatus:              16;
        unsigned int actualValues: 32;
                unsigned int boardTemp:    16;
                unsigned int boardType:    16;
                int delayA:                16;
                int delayC:                16;
                int delayLaser:            32;
                int mainPanel:             16;
                int trigger1rand:          32;
                int trigger1sign:          16;
                long long trigger1cnt:     64;
                int trigger2rand:          32;
                int trigger2sign:          16;
                long long trigger2cnt:     64;
                int trigger3rand:          32;
                int trigger3sign:          16;
                long long trigger3cnt:     64;
                int trigger4rand:          32;
                int trigger4sign:          16;
                long long trigger4cnt:     64;
                int trigger5rand:          32;
                int trigger5sign:          16;
                long long trigger5cnt:     64;
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
                long long sideAStatus:     64;
                long long sideCStatus:     64;
                int countersUpdRate:       32;
                float countersUpdRateValue;
                float trigger1rate;
                float trigger2rate;
                float trigger3rate;
                float trigger4rate;
                float trigger5rate;
                long long bkgrnd0Cnt:      64;
                long long bkgrnd1Cnt:      64;
                long long bkgrnd2Cnt:      64;
                long long bkgrnd3Cnt:      64;
                long long bkgrnd4Cnt:      64;
                long long bkgrnd5Cnt:      64;
                long long bkgrnd6Cnt:      64;
                long long bkgrnd7Cnt:      64;
                long long bkgrnd8Cnt:      64;
                long long bkgrnd9Cnt:      64;
                int oldTimeTrigger1:       32;
                int oldTimeTrigger2:       32;
                int oldTimeTrigger3:       32;
                int oldTimeTrigger4:       32;
                int oldTimeTrigger5:       32;
                int fpgaTemp:              32;
                int vPower1:               32;
                int vPower18:              32;
                int modeSettings:          32;
                long long laserDivider:    64;
                int extSwitches:           32;
                int attenuator:            32;
                int triggerModes:          32;
                long long dsTriggerMask:   64;
                int bcidOffset:            32;
                long long dgTriggerMask:   64;
                int triggerCounterTemp:    32;
                long long systemId:        64;
                long long trgPatternLSB:   64;
                long long trgPatternMSB:   64;
                long long cruOrbit:        64;
                long long cruBcFifos:      64;
                long long fifoCount:       64;
                long long selFirstHitDroppedOrbit: 64;
                long long selLastHitDroppedOrbit:  64;
                long long selHitsDropped:  64;
                int readoutRate:           32;
                int IPbusFIFOdata:         32;
                int eventsCount:           32;
                long long mcodeTime:       64;
                long long fwTime:          64;
                long long dgBunchPattern:  64;
                long long tgSingleValue:   64;
                long long tgPattern1:      64;
                long long tgPattern0:      64;
                long long tgContValue:     64;
                long long emulationRate:   64;
                int spiMask:               32;
                int triggersSuppressionControl: 32;
                long long averageTime:     64;
                int generatorFreqOffset:   32;
                long long tcmCountersValues: 64;
                int oldTimeBkgrnd0:       32;
                int oldTimeBkgrnd1:       32;
                int oldTimeBkgrnd2:       32;
                int oldTimeBkgrnd3:       32;
                int oldTimeBkgrnd4:       32;
                int oldTimeBkgrnd5:       32;
                int oldTimeBkgrnd6:       32;
                int oldTimeBkgrnd7:       32;
                int oldTimeBkgrnd8:       32;
                int oldTimeBkgrnd9:       32;
                long long bkgrnd0Rate:    64;
                long long bkgrnd1Rate:    64;
                long long bkgrnd2Rate:    64;
                long long bkgrnd3Rate:    64;
                long long bkgrnd4Rate:    64;
                long long bkgrnd5Rate:    64;
                long long bkgrnd6Rate:    64;
                long long bkgrnd7Rate:    64;
                long long bkgrnd8Rate:    64;
                long long bkgrnd9Rate:    64;
                long long laserPattern1:  64;
                long long laserPattern0:  64;
                
    } temp;
};

extern TypeTCM tcm;