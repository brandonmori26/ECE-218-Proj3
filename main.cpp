//=====[Libraries]=============================================================
#include "mbed.h"
#include "arm_book_lib.h"
#include "display.h"
#include "Ignition.h"
#include "Windshield.h"


int  main(){
    inputsInitIgnition();
    outputsInitIgnition();
    windshieldInit();
    displayInit();
    
    while(true)
    {
        ignitionUpdate();
        windshieldUpdate();
        windshieldDisplayUpdate();
        delay(SYSTEM_TIME_INCREMENT_MS);
    }
}
