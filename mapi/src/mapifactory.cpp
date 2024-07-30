#include "mapifactory.h"
#include "work_status.h"
#include "actual_values.h"
#include "delayA.h"
#include "delayC.h"
#include "board_temp.h"
#include "board_type.h"



MapiFactory::MapiFactory(Fred *fred)
{
    this->fred = fred;

    try
    {
        this->generateObjects();
    }
    catch (const exception& e)
    {
        Print::PrintError(e.what());
        exit(EXIT_FAILURE);
    }
}

MapiFactory::~MapiFactory()
{
    for (size_t i = 0; i < this->mapiObjects.size(); i++)
    {
        delete this->mapiObjects[i];
    }
    
}

/**
 * 
 * Registering mapi objects into FRED
 * 
**/
void MapiFactory::generateObjects(){
    WorkStatus* workStatus = new WorkStatus();
    this->fred->registerMapiObject("FIT_FRED/READOUTCARDS/FIT0/WORK_STATUS", workStatus);
    this->mapiObjects.push_back(workStatus);
    ActualValues* actualValues = new ActualValues();
    this->fred->registerMapiObject("FIT_FRED/READOUTCARDS/FIT0/ACTUAL_VALUES", actualValues);
    this->mapiObjects.push_back(actualValues);
    DelayA* delayA = new DelayA();
    this->fred->registerMapiObject("FIT_FRED/READOUTCARDS/FIT0/DELAY_A", delayA);
    this->mapiObjects.push_back(delayA);
    DelayC* delayC = new DelayC();
    this->fred->registerMapiObject("FIT_FRED/READOUTCARDS/FIT0/DELAY_C", delayC);
    this->mapiObjects.push_back(delayC);
    BoardTemp* boardTemp = new BoardTemp();
    this->fred->registerMapiObject("FIT_FRED/READOUTCARDS/FIT0/BOARD_TEMP", boardTemp);
    this->mapiObjects.push_back(boardTemp);
    BoardType* boardType = new BoardType();
    this->fred->registerMapiObject("FIT_FRED/READOUTCARDS/FIT0/BOARD_TYPE", boardType);
    this->mapiObjects.push_back(boardType);
}
