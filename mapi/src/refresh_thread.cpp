#include "refresh_thread.h"
#include <thread>
#include <chrono>
#include <iostream>
#include "../../core/include/dim/dic.hxx"

void timerFunction() {
    while (true) {
        DimClient::sendCommand(std::string("FIT_FRED/READOUTCARDS/FIT0/TRIGGERS_REQ").c_str(), std::string("0,0").c_str());
        DimClient::sendCommand(std::string("FIT_FRED/READOUTCARDS/FIT0/MAIN_PANEL_REQ").c_str(), std::string("0,0").c_str());
        DimClient::sendCommand(std::string("FIT_FRED/READOUTCARDS/FIT0/WORK_STATUS_REQ").c_str(), std::string("0,0").c_str());
        DimClient::sendCommand(std::string("FIT_FRED/READOUTCARDS/FIT0/DELAY_A_REQ").c_str(), std::string("0,0").c_str());
        DimClient::sendCommand(std::string("FIT_FRED/READOUTCARDS/FIT0/DELAY_C_REQ").c_str(), std::string("0,0").c_str());
        DimClient::sendCommand(std::string("FIT_FRED/READOUTCARDS/FIT0/DELAY_LASER_REQ").c_str(), std::string("0,0").c_str());
        DimClient::sendCommand(std::string("FIT_FRED/READOUTCARDS/FIT0/ACTUAL_VALUES_REQ").c_str(), std::string("").c_str());
        DimClient::sendCommand(std::string("FIT_FRED/READOUTCARDS/FIT0/BOARD_TEMP_REQ").c_str(), std::string("").c_str());
        DimClient::sendCommand(std::string("FIT_FRED/READOUTCARDS/FIT0/BOARD_TYPE_REQ").c_str(), std::string("").c_str());
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

void startTimerThread() {
    std::thread timerThread(timerFunction);
    timerThread.detach();
}
