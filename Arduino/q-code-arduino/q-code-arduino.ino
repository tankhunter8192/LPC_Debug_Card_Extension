//--------------------------------------------------------------------------------------------------------------------------------------------------------------------
//LPC Debug Card Readout
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------
//recommended board: Arduino DUE (for future features (in planning is per USB to transfer the data directly to a PC for faster logging)(not tested yet)
//reason: logic level is 3.3 and the LPC Debug Card uses also 3.3
//alternativ board: Arduino MEGA 2560 (easier circuit because the Hitachi HD44780 has 5.0V only)(not tested yet)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------
//not usable Boards: Arduino Uno (too few pins), Nano (too few pins), others are not tested
//used display driver: Hitachi HD44780
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------
//Important: The messages are for the ASUS WS X299 SAGE
//Other Mainboards may use other meanings for the specific codes
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------
//Used Hardware/Products:
//*ElmorLabs LPC Debug Card: https://elmorlabs.com/index.php/product/lpc-debug-card/
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------
//Used Software/Products:
//*Elegoo The Most Complete Starter Kit for MEGA: https://www.elegoo.com/tutorial/Elegoo%20The%20Most%20Complete%20Starter%20Kit%20for%20MEGA%20V1.0.2019.09.17.zip
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------
//Version: 0.01 (2019-11-10)
//*Initial version
//Version: 0.02 (2019-11-19)
//*add feature I2C for Raspberry Pi Readout
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------
//Everything is voiding warranty of the specific products since the project involves soldering!!!
//Legal Notice are written in the last lines of this file
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------

//includes
#include <Wire.h>
#include <String.h>
#include <LiquidCrystal.h>

//define Pins of LPC Debug Card board
/*
  The connections are [LPC Debug Card] and then [Arduino].
  For the pin reference visit:
  https://elmorlabs.com/index.php/forum/topic/the-pinout-of-the-two-7segement-displays/
  the Defines in the following are to be interpreted with : "[Lx_y] is [LEDxy]."
  Additional is the Arduino GND with the PC GND or LPC Debug Card GND to be connected.
*/
#define LED1A 38
#define LED1B 39
#define LED1C 40
#define LED1D 41
#define LED1E 42
#define LED1F 43
#define LED1G 44
#define LED1DP 45
#define LED2A 46
#define LED2B 47
#define LED2C 48
#define LED2D 49
#define LED2E 50
#define LED2F 51
#define LED2G 52
#define LED2DP 53

//define Pins of display
/*
  The circuit for the display:
   LCD RS pin to digital pin 7
   LCD Enable pin to digital pin 8
   LCD D4 pin to digital pin 9
   LCD D5 pin to digital pin 10
   LCD D6 pin to digital pin 11
   LCD D7 pin to digital pin 12
   LCD R/W pin to ground
   LCD VSS pin to ground
   LCD VCC pin to 5V
   10K resistor:
   ends to +5V and ground
   wiper to LCD VO pin (pin 3)
*/
#define RSpin 7
#define EnablePin 8
#define DigitalPin4 9
#define DigitalPin5 10
#define DigitalPin6 11
#define DigitalPin7 12

//define display limits
#define lenght 16     //>15
#define rows 2        //>01
#define delaytime 500

//define LED pins
/*
  Used for seperate displaying of errors
  The circuit is the Arduino pin then a resistor and then the LED after that it goes to GND 
*/
#define ERRORLEDPin 21


//I2C Output Address
#define Address 0x80

//global variables
LiquidCrystal lcd(RSpin, EnablePin, DigitalPin4, DigitalPin5, DigitalPin6, DigitalPin7);
int ERRORLED = 0;
char WireMessage[80];
void requestEvent(){
  Wire.print(WireMessage);
}

void setup() {
  // put your setup code here, to run once:
  lcd.begin(lenght, rows);
  pinMode(LED1A, INPUT);
  pinMode(LED1B, INPUT);
  pinMode(LED1C, INPUT);
  pinMode(LED1D, INPUT);
  pinMode(LED1E, INPUT);
  pinMode(LED1F, INPUT);
  pinMode(LED1G, INPUT);
  pinMode(LED1DP, INPUT);
  pinMode(LED2A, INPUT);
  pinMode(LED2B, INPUT);
  pinMode(LED2C, INPUT);
  pinMode(LED2D, INPUT);
  pinMode(LED2E, INPUT);
  pinMode(LED2F, INPUT);
  pinMode(LED2G, INPUT);
  pinMode(LED2DP, INPUT);
  pinMode(ERRORLEDPin, OUTPUT);
  digitalWrite(ERRORLEDPin, LOW);
  Wire.begin(Address);
  Wire.onRequest(requestEvent);
}

char GetChar(int cchar) { //0 = L1 segment 1 = L2 segment
  int start = 0;
  int inta, intb, intc, intd, inte, intf, intg, intdp;
  if (cchar) {
    //LED1 readout
    inta = digitalRead(LED1A);
    intb = digitalRead(LED1B);
    intc = digitalRead(LED1C);
    intd = digitalRead(LED1D);
    inte = digitalRead(LED1E);
    intf = digitalRead(LED1F);
    intg = digitalRead(LED1G);
    intdp = digitalRead(LED1DP);
  } else {
    //LED2 readout
    inta = digitalRead(LED2A);
    intb = digitalRead(LED2B);
    intc = digitalRead(LED2C);
    intd = digitalRead(LED2D);
    inte = digitalRead(LED2E);
    intf = digitalRead(LED2F);
    intg = digitalRead(LED2G);
    intdp = digitalRead(LED2DP);
  }

  if ((inta == HIGH) && (intb == HIGH) && (intc == HIGH) && (intd == HIGH) && (inte == HIGH) && (intf == HIGH) && (intg == LOW)) {
    return '0';
  } else if ((inta == LOW) && (intb == HIGH) && (intc == HIGH) && (intd == LOW) && (inte == LOW) && (intf == LOW) && (intg == LOW)) {
    return '1';
  } else if ((inta == HIGH) && (intb == HIGH) && (intc == LOW) && (intd == HIGH) && (inte == HIGH) && (intf == LOW) && (intg == HIGH)) {
    return '2';
  } else if ((inta == HIGH) && (intb == HIGH) && (intc == HIGH) && (intd == HIGH) && (inte == LOW) && (intf == LOW) && (intg == HIGH)) {
    return '3';
  } else if ((inta == LOW) && (intb == HIGH) && (intc == HIGH) && (intd == LOW) && (inte == LOW) && (intf == HIGH) && (intg == HIGH)) {
    return '4';
  } else if ((inta == HIGH) && (intb == LOW) && (intc == HIGH) && (intd == HIGH) && (inte == LOW) && (intf == HIGH) && (intg == HIGH)) {
    return '5';
  } else if ((inta == HIGH) && (intb == LOW) && (intc == HIGH) && (intd == HIGH) && (inte == HIGH) && (intf == HIGH) && (intg == HIGH)) {
    return '6';
  } else if ((inta == HIGH) && (intb == HIGH) && (intc == HIGH) && (intd == LOW) && (inte == LOW) && (intf == LOW) && (intg == LOW)) {
    return '7';
  } else if ((inta == HIGH) && (intb == HIGH) && (intc == HIGH) && (intd == HIGH) && (inte == HIGH) && (intf == HIGH) && (intg == HIGH)) {
    return '8';
  } else if ((inta == HIGH) && (intb == HIGH) && (intc == HIGH) && (intd == LOW) && (inte == LOW) && (intf == HIGH) && (intg == HIGH)) {
    return '9';
  } else if ((inta == HIGH) && (intb == HIGH) && (intc == HIGH) && (intd == LOW) && (inte == HIGH) && (intf == HIGH) && (intg == HIGH)) {
    return 'A';
  } else if ((inta == LOW) && (intb == LOW) && (intc == HIGH) && (intd == HIGH) && (inte == HIGH) && (intf == HIGH) && (intg == HIGH)) {
    return 'B'; //orginal: 'b' but 'B' is more readable on displays
  } else if ((inta == HIGH) && (intb == LOW) && (intc == LOW) && (intd == HIGH) && (inte == HIGH) && (intf == HIGH) && (intg == LOW)) {
    return 'C';
  } else if ((inta == LOW) && (intb == HIGH) && (intc == HIGH) && (intd == HIGH) && (inte == HIGH) && (intf == LOW) && (intg == HIGH)) {
    return  'D'; //orginal: 'd' but 'D'is more readable on displays
  } else if ((inta == HIGH) && (intb == LOW) && (intc == LOW) && (intd == HIGH) && (inte == HIGH) && (intf == HIGH) && (intg == HIGH)) {
    return 'E';
  } else if ((inta == HIGH) && (intb == LOW) && (intc == LOW) && (intd == LOW) && (inte == HIGH) && (intf == HIGH) && (intg == HIGH)) {
    return 'F';
  } else {
    return (char) - 1;
  }
  return (char) - 1;
}

void loop() {
  // put your main code here, to run repeatedly:
  char cbuffer[16];
  char message[60];
  char visible [4] = {0};
  visible[0] = GetChar(0);
  visible[1] = GetChar(1);
  sprintf(cbuffer, "%c", visible);
  lcd.print(cbuffer);
  
  switch (visible[0]) {
    case '0':
      switch (visible[1]) {
        case '0':
          strcpy(message, "NOT USED");
          break;
        case '1':
          strcpy(message, "NOT IN THE MANUAL");
          break;
        case '2':
          strcpy(message, "microcode");
          break;
        case '3':
          strcpy(message, "CACHE ENABLED - System is entering S3 sleep state");
          break;
        case '4':
          strcpy(message, "PCH initialization - System is entering S4 sleep state");
          break;
        case '5':
          strcpy(message, "NOT USED - System is entering S5 sleep state");
          break;
        case '6':
          strcpy(message, "CPU EARLY INIT");
          break;
        case '7':
          strcpy(message, "NOT USED");
          break;
        case '8':
          strcpy(message, "NOT USED");
          break;
        case '9':
          strcpy(message, "NOT USED");
          break;
        case 'A':
          strcpy(message, "NOT USED");
          break;
        case 'B':
          strcpy(message, "NOT USED");
          break;
        case 'C':
          strcpy(message, "NOT USED");
          break;
        case 'D':
          strcpy(message, "NOT USED");
          break;
        case 'E':
          strcpy(message, "NOT USED");
          break;
        case 'F':
          strcpy(message, "NOT USED");
          break;
      }
      break;
    case '1':
      switch (visible[1]) {
        case '0':
          strcpy(message, "PEI CORE IS STARTED");
          break;
        case '1':
          strcpy(message, "Pre-memory CPU INIT is started");
          break;
        case '2':
          strcpy(message, "Pre-memory CPU INIT is started");
          break;
        case '3':
          strcpy(message, "Pre-memory CPU INIT is started");
          break;
        case '4':
          strcpy(message, "Pre-memory CPU INIT is started");
          break;
        case '5':
          strcpy(message, "Pre-memory Sys Agent is started");
          break;
        case '6':
          strcpy(message, "Pre-memory Sys Agent is started");
          break;
        case '7':
          strcpy(message, "Pre-memory Sys Agent is started");
          break;
        case '8':
          strcpy(message, "Pre-memory Sys Agent is started");
          break;
        case '9':
          strcpy(message, "Pre-memory PCH INIT is started");
          break;
        case 'A':
          strcpy(message, "Pre-memory PCH INIT is started");
          break;
        case 'B':
          strcpy(message, "Pre-memory PCH INIT is started");
          break;
        case 'C':
          strcpy(message, "Pre-memory PCH INIT is started");
          break;
        case 'D':
          strcpy(message, "NOT USED");
          break;
        case 'E':
          strcpy(message, "NOT USED");
          break;
        case 'F':
          strcpy(message, "NOT USED");
          break;
      }
      break;
    case '2':
      switch (visible[1]) {
        case '0':
          strcpy(message, "NOT USED");
          break;
        case '1':
          strcpy(message, "NOT USED");
          break;
        case '2':
          strcpy(message, "NOT USED");
          break;
        case '3':
          strcpy(message, "NOT USED");
          break;
        case '4':
          strcpy(message, "NOT USED");
          break;
        case '5':
          strcpy(message, "NOT USED");
          break;
        case '6':
          strcpy(message, "NOT USED");
          break;
        case '7':
          strcpy(message, "NOT USED");
          break;
        case '8':
          strcpy(message, "NOT USED");
          break;
        case '9':
          strcpy(message, "NOT USED");
          break;
        case 'A':
          strcpy(message, "NOT USED");
          break;
        case 'B':
          strcpy(message, "Memory Initialization");
          break;
        case 'C':
          strcpy(message, "Memory Initialization");
          break;
        case 'D':
          strcpy(message, "Memory Initialization");
          break;
        case 'E':
          strcpy(message, "Memory Initialization");
          break;
        case 'F':
          strcpy(message, "Memory Initialization");
          break;
      }
      break;
    case '3':
      switch (visible[1]) {
        case '0':
          strcpy(message, "NOT USED - System is waking up from the S3 sleep state");
          break;
        case '1':
          strcpy(message, "Memory started");
          break;
        case '2':
          strcpy(message, "CPU post-memory initialization");
          break;
        case '3':
          strcpy(message, "CPU post-memory initialization");
          break;
        case '4':
          strcpy(message, "CPU post-memory initialization");
          break;
        case '5':
          strcpy(message, "CPU post-memory initialization");
          break;
        case '6':
          strcpy(message, "CPU post-memory initialization");
          break;
        case '7':
          strcpy(message, "Post-memory Sys Agent init is started");
          break;
        case '8':
          strcpy(message, "Post-memory Sys Agent init is started");
          break;
        case '9':
          strcpy(message, "Post-memory Sys Agent init is started");
          break;
        case 'A':
          strcpy(message, "Post-memory Sys Agent init is started");
          break;
        case 'B':
          strcpy(message, "Post-memory PCH init is started");
          break;
        case 'C':
          strcpy(message, "Post-memory PCH init is started");
          break;
        case 'D':
          strcpy(message, "Post-memory PCH init is started");
          break;
        case 'E':
          strcpy(message, "Post-memory PCH init is started");
          break;
        case 'F':
          strcpy(message, "NOT USED");
          break;
      }
      break;
    case '4':
      switch (visible[1]) {
        case '0':
          strcpy(message, "NOT USED - System is waking up from the S4 sleep state");
          break;
        case '1':
          strcpy(message, "NOT USED");
          break;
        case '2':
          strcpy(message, "NOT USED");
          break;
        case '3':
          strcpy(message, "NOT USED");
          break;
        case '4':
          strcpy(message, "NOT USED");
          break;
        case '5':
          strcpy(message, "NOT USED");
          break;
        case '6':
          strcpy(message, "NOT USED");
          break;
        case '7':
          strcpy(message, "NOT USED");
          break;
        case '8':
          strcpy(message, "NOT USED");
          break;
        case '9':
          strcpy(message, "NOT USED");
          break;
        case 'A':
          strcpy(message, "NOT USED");
          break;
        case 'B':
          strcpy(message, "NOT USED");
          break;
        case 'C':
          strcpy(message, "NOT USED");
          break;
        case 'D':
          strcpy(message, "NOT USED");
          break;
        case 'E':
          strcpy(message, "NOT USED");
          break;
        case 'F':
          strcpy(message, "DXE IPL is started");
          break;
      }
      break;
    case '5':
      switch (visible[1]) {
        case '0':
          strcpy(message, "Mem INIT ERROR: Invalid mem typ or incompatible mem speed");
          ERRORLED = 1;
          break;
        case '1':
          strcpy(message, "Mem INIT ERROR: Invalid mem typ or incompatible mem speed");
          ERRORLED = 1;
          break;
        case '2':
          strcpy(message, "Mem INIT ERROR: Invalid mem typ or incompatible mem speed");
          ERRORLED = 1;
          break;
        case '3':
          strcpy(message, "Mem INIT ERROR: Invalid mem typ or incompatible mem speed");
          ERRORLED = 1;
          break;
        case '4':
          strcpy(message, "Unspecified memory initialization ERROR");
          ERRORLED = 1;
          break;
        case '5':
          strcpy(message, "Memory not installed");
          ERRORLED = 1;
          break;
        case '6':
          strcpy(message, "Invalid CPU type or Speed");
          ERRORLED = 1;
          break;
        case '7':
          strcpy(message, "CPU mismatch");
          ERRORLED = 1;
          break;
        case '8':
          strcpy(message, "CPU selftest failed or possible CPU cache ERROR");
          ERRORLED = 1;
          break;
        case '9':
          strcpy(message, "CPU micro-code not found or micro-code update is failed");
          ERRORLED = 1;
          break;
        case 'A':
          strcpy(message, "Internal CPU ERROR");
          ERRORLED = 1;
          break;
        case 'B':
          strcpy(message, "Reset PPI is not available");
          ERRORLED = 1;
          break;
        case 'C':
          strcpy(message, "Reserved for future AMI ERROR Codes");
          ERRORLED = 1;
          break;
        case 'D':
          strcpy(message, "Reserved for future AMI ERROR Codes");
          ERRORLED = 1;
          break;
        case 'E':
          strcpy(message, "Reserved for future AMI ERROR Codes");
          ERRORLED = 1;
          break;
        case 'F':
          strcpy(message, "Reserved for future AMI ERROR Codes");
          ERRORLED = 1;
          break;
      }
      break;
    case '6':
      switch (visible[1]) {
        case '0':
          strcpy(message, "DXE Core is started");
          break;
        case '1':
          strcpy(message, "NVRAM initialization");
          break;
        case '2':
          strcpy(message, "Installation of the PCH Runtime Services");
          break;
        case '3':
          strcpy(message, "CPU DXE initialization is started");
          break;
        case '4':
          strcpy(message, "CPU DXE initialization is started");
          break;
        case '5':
          strcpy(message, "CPU DXE initialization is started");
          break;
        case '6':
          strcpy(message, "CPU DXE initialization is started");
          break;
        case '7':
          strcpy(message, "CPU DXE initialization is started");
          break;
        case '8':
          strcpy(message, "PCI host bridge initialization");
          break;
        case '9':
          strcpy(message, "System Agent DXE initialization is started");
          break;
        case 'A':
          strcpy(message, "System Agent DXE SMM initialization is started");
          break;
        case 'B':
          strcpy(message, "System Agent DXE initialization (System Agent module specific");
          break;
        case 'C':
          strcpy(message, "System Agent DXE initialization (System Agent module specific");
          break;
        case 'D':
          strcpy(message, "System Agent DXE initialization (System Agent module specific");
          break;
        case 'E':
          strcpy(message, "System Agent DXE initialization (System Agent module specific");
          break;
        case 'F':
          strcpy(message, "System Agent DXE initialization (System Agent module specific");
          break;
      }
      break;
    case '7':
      switch (visible[1]) {
        case '0':
          strcpy(message, "PCH DXE initialization is started");
          break;
        case '1':
          strcpy(message, "PCH DXE SMM initialization is started");
          break;
        case '2':
          strcpy(message, "PCH devices initialization");
          break;
        case '3':
          strcpy(message, "PCH DXE Initialization (PCH module specific)");
          break;
        case '4':
          strcpy(message, "PCH DXE Initialization (PCH module specific)");
          break;
        case '5':
          strcpy(message, "PCH DXE Initialization (PCH module specific)");
          break;
        case '6':
          strcpy(message, "PCH DXE Initialization (PCH module specific)");
          break;
        case '7':
          strcpy(message, "PCH DXE Initialization (PCH module specific)");
          break;
        case '8':
          strcpy(message, "ACPI module initialization");
          break;
        case '9':
          strcpy(message, "CSM initialization");
          break;
        case 'A':
          strcpy(message, "Reserved for future AMI DXE codes");
          break;
        case 'B':
          strcpy(message, "Reserved for future AMI DXE codes");
          break;
        case 'C':
          strcpy(message, "Reserved for future AMI DXE codes");
          break;
        case 'D':
          strcpy(message, "Reserved for future AMI DXE codes");
          break;
        case 'E':
          strcpy(message, "Reserved for future AMI DXE codes");
          break;
        case 'F':
          strcpy(message, "Reserved for future AMI DXE codes");
          break;
      }
      break;
    case '8':
      switch (visible[1]) {
        case '0':
          strcpy(message, "NOT USED");
          break;
        case '1':
          strcpy(message, "NOT USED");
          break;
        case '2':
          strcpy(message, "NOT USED");
          break;
        case '3':
          strcpy(message, "NOT USED");
          break;
        case '4':
          strcpy(message, "NOT USED");
          break;
        case '5':
          strcpy(message, "NOT USED");
          break;
        case '6':
          strcpy(message, "NOT USED");
          break;
        case '7':
          strcpy(message, "NOT USED");
          break;
        case '8':
          strcpy(message, "NOT USED");
          break;
        case '9':
          strcpy(message, "NOT USED");
          break;
        case 'A':
          strcpy(message, "NOT USED");
          break;
        case 'B':
          strcpy(message, "NOT USED");
          break;
        case 'C':
          strcpy(message, "NOT USED");
          break;
        case 'D':
          strcpy(message, "NOT USED");
          break;
        case 'E':
          strcpy(message, "NOT USED");
          break;
        case 'F':
          strcpy(message, "NOT USED");
          break;
      }
      break;
    case '9':
      switch (visible[1]) {
        case '0':
          strcpy(message, "Boot Devices Selection (BDS) phase is started");
          break;
        case '1':
          strcpy(message, "Driver connecting is started");
          break;
        case '2':
          strcpy(message, "PCI Bus initialization is started");
          break;
        case '3':
          strcpy(message, "PCI Bus Hot Plug Controller Initialization");
          break;
        case '4':
          strcpy(message, "PCI Bus Enumeration");
          break;
        case '5':
          strcpy(message, "PCI Bus Request Resources");
          break;
        case '6':
          strcpy(message, "PCI Bus Assign Resources");
          break;
        case '7':
          strcpy(message, "Console Output devices connect");
          break;
        case '8':
          strcpy(message, "Console Input devices connect");
          break;
        case '9':
          strcpy(message, "Super IO Initialization");
          break;
        case 'A':
          strcpy(message, "USB initialization is started");
          break;
        case 'B':
          strcpy(message, "USB Reset");
          break;
        case 'C':
          strcpy(message, "USB Detect");
          break;
        case 'D':
          strcpy(message, "USB Enable");
          break;
        case 'E':
          strcpy(message, "Reserved for future AMI codes");
          break;
        case 'F':
          strcpy(message, "Reserved for future AMI codes");
          break;
      }
      break;
    case 'A':
      switch (visible[1]) {
        case '0':
          strcpy(message, "IDE initialization is started");
          break;
        case '1':
          strcpy(message, "IDE Reset");
          break;
        case '2':
          strcpy(message, "IDE Detect");
          break;
        case '3':
          strcpy(message, "IDE Enable");
          break;
        case '4':
          strcpy(message, "SCSI initialization is started");
          break;
        case '5':
          strcpy(message, "SCSI Reset");
          break;
        case '6':
          strcpy(message, "SCSI Detect");
          break;
        case '7':
          strcpy(message, "SCSI Enable");
          break;
        case '8':
          strcpy(message, "Setup Verifying Password");
          break;
        case '9':
          strcpy(message, "Start of Setup");
          break;
        case 'A':
          strcpy(message, "Reserved for ASL - System has transitioned into ACPI mode. Interrupt is in APIC mode");
          break;
        case 'B':
          strcpy(message, "Setup Input Wait");
          break;
        case 'C':
          strcpy(message, "Reserved for ASL - System has transitioned into ACPI mode. Interrupt is in PIC mode");
          break;
        case 'D':
          strcpy(message, "Ready to Boot event");
          break;
        case 'E':
          strcpy(message, "Legacy Boot event");
          break;
        case 'F':
          strcpy(message, "Exit Boot Services event");
          break;
      }
      break;
    case 'B':
      switch (visible[1]) {
        case '0':
          strcpy(message, "Runtime Set Virtual Address MAP Begin");
          break;
        case '1':
          strcpy(message, "Runtime Set Virtual Address MAP End");
          break;
        case '2':
          strcpy(message, "Legacy Option ROM Initialization");
          break;
        case '3':
          strcpy(message, "System Reset");
          break;
        case '4':
          strcpy(message, "USB hot plug");
          break;
        case '5':
          strcpy(message, "PCI bus hot plug");
          break;
        case '6':
          strcpy(message, "Clean-up of NVRAM");
          break;
        case '7':
          strcpy(message, "Configuration Reset (reset of NVRAM settings)");
          break;
        case '8':
          strcpy(message, "Reserved for future AMI codes");
          break;
        case '9':
          strcpy(message, "Reserved for future AMI codes");
          break;
        case 'A':
          strcpy(message, "Reserved for future AMI codes");
          break;
        case 'B':
          strcpy(message, "Reserved for future AMI codes");
          break;
        case 'C':
          strcpy(message, "Reserved for future AMI codes");
          break;
        case 'D':
          strcpy(message, "Reserved for future AMI codes");
          break;
        case 'E':
          strcpy(message, "Reserved for future AMI codes");
          break;
        case 'F':
          strcpy(message, "Reserved for future AMI codes");
          break;
      }
      break;
    case 'C':
      switch (visible[1]) {
        case '0':
          strcpy(message, "NOT USED");
          break;
        case '1':
          strcpy(message, "NOT USED");
          break;
        case '2':
          strcpy(message, "NOT USED");
          break;
        case '3':
          strcpy(message, "NOT USED");
          break;
        case '4':
          strcpy(message, "NOT USED");
          break;
        case '5':
          strcpy(message, "NOT USED");
          break;
        case '6':
          strcpy(message, "NOT USED");
          break;
        case '7':
          strcpy(message, "NOT USED");
          break;
        case '8':
          strcpy(message, "NOT USED");
          break;
        case '9':
          strcpy(message, "NOT USED");
          break;
        case 'A':
          strcpy(message, "NOT USED");
          break;
        case 'B':
          strcpy(message, "NOT USED");
          break;
        case 'C':
          strcpy(message, "NOT USED");
          break;
        case 'D':
          strcpy(message, "NOT USED");
          break;
        case 'E':
          strcpy(message, "NOT USED");
          break;
        case 'F':
          strcpy(message, "NOT USED");
          break;
      }
      break;
    case 'D':
      switch (visible[1]) {
        case '0':
          strcpy(message, "CPU initialization ERROR");
          ERRORLED = 1;
          break;
        case '1':
          strcpy(message, "System Agent initialization ERROR");
          ERRORLED = 1;
          break;
        case '2':
          strcpy(message, "PCH initialization ERROR");
          ERRORLED = 1;
          break;
        case '3':
          strcpy(message, "Some of the Architectural Protocols are not available");
          ERRORLED = 1;
          break;
        case '4':
          strcpy(message, "PCI resource allocation ERROR. Out of Resources");
          ERRORLED = 1;
          break;
        case '5':
          strcpy(message, "No Space for Legacy Option ROM");
          ERRORLED = 1;
          break;
        case '6':
          strcpy(message, "No Console Output Devices are found");
          ERRORLED = 1;
          break;
        case '7':
          strcpy(message, "No Console Input Devices are found");
          ERRORLED = 1;
          break;
        case '8':
          strcpy(message, "Invalid password");
          ERRORLED = 1;
          break;
        case '9':
          strcpy(message, "ERROR: loading Boot Option (LoadImage returned ERROR)");
          ERRORLED = 1;
          break;
        case 'A':
          strcpy(message, "Boot Option is failed (StartImage returned ERROR)");
          ERRORLED = 1;
          break;
        case 'B':
          strcpy(message, "Flash update is failed");
          ERRORLED = 1;
          break;
        case 'C':
          strcpy(message, "Reset protocol is not available");
          ERRORLED = 1;
          break;
        case 'D':
          strcpy(message, "NOT USED");
          break;
        case 'E':
          strcpy(message, "NOT USED");
          break;
        case 'F':
          strcpy(message, "NOT USED");
          break;
      }
      break;
    case 'E':
      switch (visible[1]) {
        case '0':
          strcpy(message, "S3 Resume ist started - S3 Resume PPI is called by the DXE IPL");
          break;
        case '1':
          strcpy(message, "S3 Boot Script execution");
          break;
        case '2':
          strcpy(message, "Video repost");
          break;
        case '3':
          strcpy(message, "OS S3 wake vector call");
          break;
        case '4':
          strcpy(message, "Reserved for future AMI progress codes");
          break;
        case '5':
          strcpy(message, "Reserved for future AMI progress codes");
          break;
        case '6':
          strcpy(message, "Reserved for future AMI progress codes");
          break;
        case '7':
          strcpy(message, "Reserved for future AMI progress codes");
          break;
        case '8':
          strcpy(message, "S3 Resume FAILED");
          ERRORLED = 1;
          break;
        case '9':
          strcpy(message, "S3 Resume PPI not found");
          ERRORLED = 1;
          break;
        case 'A':
          strcpy(message, "S3 Resume Boot Script ERROR");
          ERRORLED = 1;
          break;
        case 'B':
          strcpy(message, "S3 OS Wake ERROR");
          ERRORLED = 1;
          break;
        case 'C':
          strcpy(message, "Reserved for future AMI error codes");
          ERRORLED = 1;
          break;
        case 'D':
          strcpy(message, "Reserved for future AMI error codes");
          ERRORLED = 1;
          break;
        case 'E':
          strcpy(message, "Reserved for future AMI error codes");
          ERRORLED = 1;
          break;
        case 'F':
          strcpy(message, "Reserved for future AMI error codes");
          ERRORLED = 1;
          break;
      }
      break;
    case 'F':
      switch (visible[1]) {
        case '0':
          strcpy(message, "Recovery condition triggered by firmware (Auto recovery)");
          break;
        case '1':
          strcpy(message, "Recovery condition triggered by user (Forced recovery)");
          break;
        case '2':
          strcpy(message, "Recovery process started");
          break;
        case '3':
          strcpy(message, "Recovery firmware image is found");
          break;
        case '4':
          strcpy(message, "Recovery firmware image is loaded");
          break;
        case '5':
          strcpy(message, "Reserved for future AMI progress codes");
          break;
        case '6':
          strcpy(message, "Reserved for future AMI progress codes");
          break;
        case '7':
          strcpy(message, "Reserved for future AMI progress codes");
          break;
        case '8':
          strcpy(message, "Recovery PPI is not avialable");
          ERRORLED = 1;
          break;
        case '9':
          strcpy(message, "Recovery capsule is not found");
          ERRORLED = 1;
          break;
        case 'A':
          strcpy(message, "Invalid recovery capsule");
          ERRORLED = 1;
          break;
        case 'B':
          strcpy(message, "Reserved for future AMI error codes");
          ERRORLED = 1;
          break;
        case 'C':
          strcpy(message, "Reserved for future AMI error codes");
          ERRORLED = 1;
          break;
        case 'D':
          strcpy(message, "Reserved for future AMI error codes");
          ERRORLED = 1;
          break;
        case 'E':
          strcpy(message, "Reserved for future AMI error codes");
          ERRORLED = 1;
          break;
        case 'F':
          strcpy(message, "Reserved fir future AMI error codes");
          ERRORLED = 1;
          break;
      }
      break;
  }


  strcpy(WireMessage,message);
  if (ERRORLED) {
    digitalWrite(ERRORLEDPin, HIGH);
    ERRORLED = 0;
  }
  int h = 0;
  while (h * lenght < strlen(message)) {
    for (int i = 1; i < rows; ++i) {
      lcd.setCursor(0, i);
      char line[lenght];
      for (int j = 0 ; j < lenght; ++j) {
        line[j] = message[j];
      }
      lcd.print(line);
    }
    delay(delaytime);
    ++h;
  }

}
/*
  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
  Lesser General Public License for more details.
*/
