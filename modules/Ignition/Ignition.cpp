//=====[Libraries]=============================================================
#include "mbed.h"
#include "arm_book_lib.h"

//=====[Declaration and initialization of private global variables]============
void inputsInit();
void outputsInit();
void checkIgnitionConditions();

//Declaration and initialization of public global objects
DigitalIn ignitionButton(BUTTON1);    // Simulates the ignition button
DigitalIn passengerPresent(D10);              // Passenger seat sensor
DigitalIn drivePresent(D11);              // Driver seat sensor
DigitalIn passengerSeatbelt(D12);              // Passenger seatbelt switch
DigitalIn driverSeatbelt(D13);              // Driver seatbelt switch


DigitalOut ignitionLed(LED1);         // Green LED: Ignition enabled
DigitalOut engineLed(LED2);           // Blue LED: Engine started
DigitalOut alarmBuzzer(D15);        // Alarm Buzzer
BufferedSerial uartUsb(USBTX, USBRX, 115200); // UART for messages

//=====[Implementations of public functions]===================================
void inputsInit()
{
    driverPresent.mode(PullDown);
    passengerPresent.mode(PullDown);
    driverSeatbelt.mode(PullDown);
    passengerSeatbelt.mode(PullDown);
}

void outputsInitIgnition()
{
    greenLED = OFF;
    blueLED = OFF;
    sirenPin = BUZZER_OFF;
}

void debounceIgnitionInit()
{
    if( ignitionButton ) {
        ignitionButtonState = BUTTON_UP;
    } else {
        ignitionButtonState = BUTTON_DOWN;
    }
}
void ignitionEnable()
{
    if (driverPresent && driverSeatbelt && passengerPresent && passengerSeatbelt){
        greenLED = ON;
    }
    else{
        greenLED = OFF;
    }
}

void welcomeMessage()
{
    if (driverPresent && !driverWelcomed){
        uartUsb.write("Welcome to enhanced alarm system model 218-W25\r\n\r\n", 50);
        driverWelcomed = true;
    }
    else if (!driverPresent){
        driverWelcomed = false;
    }
}

void errorMessage()
{

    uartUsb.write("Ignition Inhibited\r\n\r\n", 22);
    
    if(!driverPresent){
        uartUsb.write("Driver seat not occupied.\r\n\r\n", 29);
    } 
    if(!driverSeatbelt){
        uartUsb.write("Driver seatbelt not fastened.\r\n\r\n", 33);
    }
    if(!passengerPresent){
        uartUsb.write("Passenger seat not occupied.\r\n\r\n", 32);
    }
    if(!passengerSeatbelt){
        uartUsb.write("Passenger seatbelt not fastened.\r\n\r\n", 36);
    }
}

bool ignitionUpdate()
{
    welcomeMessage();
    ignitionEnable();
    if (!engineStarted && debounceIgnition()){
        uartUsb.write("Ignition attempted.\r\n\r\n", 23);
        if (!greenLED){
            sirenPin = BUZZER_ON;
            errorMessage();
            engineStarted = false;
        }
        else{
            sirenPin = BUZZER_OFF;
            greenLED = OFF;
            blueLED = ON;
            uartUsb.write("Engine started.\r\n\r\n", 19);
            engineStarted = true;
        }
    }
    else if (engineStarted && debounceIgnition()){
        uartUsb.write("Engine off.\r\n\r\n", 15);
        engineStarted = false;
        blueLED = OFF;
    }
    
    return engineStarted;
}
