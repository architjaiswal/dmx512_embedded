/*
Target uC	: TM4C123GH6PM
Clock Source: 16 MHz primary oscillator
Clock Rate	: 40 MHz using PLL
UART1 is occupied for communication with the PC

*/

#ifndef DMX512_H_
#define DMX512_H_

// Shared variables with main.c
extern volatile unsigned char dmxData[513];
extern int maxDmxAddr;
extern volatile int txPhase;

#define pollStartByte 0xF0				// Start code for POLL
#define dataStartByte 0x00				// Start code for normal data



//-----------------------------------------------------------------------------
// Subroutines
//-----------------------------------------------------------------------------

/*********************************************
* Read the 9 pin dip switch as device address
*********************************************/
int readDevAdd();

/*********************************************
* Read the 10th dip switch for device mode
* Return: 1(Tx_mode) or 0(Rx_mode)
*********************************************/
int readDevMode();

/******************
* Clear dmxData[]
******************/
void clrDmxData(void);

/***************************
* Read user CMD and Executes
***************************/
void getCmd(void);

/************************
 * Generate Break signal.
************************/
void brkFunc();

/*******************
 * Send Start Byte.
 * Input: Startcode
*******************/
void sendStartByte(char);


/********************************************
* Read all 512 poll data (Blocking function)
* Return: Device address's data (true/false)
********************************************/
bool readPollData(int);

#endif /* DMX512_H_ */
