/*
DMX512 Communication

Target uC	: TM4C123GH6PM
Clock Source: 16 MHz primary oscillator
Clock Rate	: 40 MHz using PLL
UART0 & 1 occupied!


CONTROLLER Code:
Get command(string) from PC using RS-232, parse it and send data through RS-485 to other slave devices.


-----------------------------------------------------------------------------
 Hardware Target
-----------------------------------------------------------------------------

Hardware configuration:
Blue LED: PF2 powers the blue LED
Red LED: PF1 drives an NPN transistor that powers the red LED
Green LED: PF3 drives an NPN transistor that powers the green LED
Pushbutton: SW1 pulls pin PF4 low (internal pull-up is used)
UART Interface:
  U0TX (PA1) and U0RX (PA0) are connected to the 2nd controller
  The USB on the 2nd controller enumerates to an ICDI interface and a virtual COM port
  Configured to 115,200 baud, 8N1 (one stop bit and no parity bit)
*/



#include "main.h"


//-----------------------------------------------------------------------------
// Interrupt Subroutine
//-----------------------------------------------------------------------------
void Uart1Isr()
{
	if(U1_TX_INT)
	{
		if(txPhase == 0)
		{
			brkFunc();
		}
		else if(txPhase == 1)
		{
			sendStartByte(dataStartByte);
		}
		else
		{
			putcUart1(dmxData[txPhase-1]);
		}
		txPhase = (txPhase+1)%(maxDmxAddr+2);	// Ring counter
		U1_TX_INT_FLAG = 1; 					// Clear the flag
	}
	else if(U1_FRAME_INT)						// Frame error?
	{
		if(getcUart1() == 0)					// Is it break?
		{
			rxPhase = 1;
			U1_FRAME_INT_FLAG = 1;
		}
	}
	else if(U1_RX_INT)							
	{
		char temp;
		temp = UART1_DR_R & 0xFF;				// Read UART1 Rx data

		if(rxPhase == 1)
		{
			if(temp == dataStartByte)
				rxPhase = 2;
			else if(temp == pollStartByte)
			{
				poll_receive = true;
				rxPhase = 0;
				disableU1RxINT();
			}
			else
				rxPhase = 0;					// Look for break

		}
		else if(rxPhase>1)
		{
			if((rxPhase-1) == deviceAdd)
			{
				if(temp>0)
					BLUE_LED = 1;
				else
					BLUE_LED = 0;

				rxPhase = 0;
			}
			else
				rxPhase++;
		}
	}
}

//-----------------------------------------------------------------------------
// Main
//-----------------------------------------------------------------------------

int main(void)
{
	// Local variables
	char buffer[6];
	int deviceMode;						//***************** Not sure yet if this needs to be global***********//


	// Initialize hardware
	initHw();
    initUart0();
    initUart1();
    clrDmxData();

    // Step 1
    RED_LED = 1;
    waitMicrosecond(250000);
    RED_LED = 0;
    waitMicrosecond(250000);
    waitPbPress();				// Wait for PB press



	// Display device mode

    deviceMode = readDevMode();

    if(deviceMode == TX_MODE)
    {
    	dmxTxDEN = 1;
    	waitMicrosecond(100);	// Stabilization time
    	txPhase = 1;

    	putsUart0("Tx Mode\r\n");

    	enableU1TxINT();
    	brkFunc();

    	while(1)
		{
			getCmd();
		}
    }
    else
    {
    	putsUart0("Rx Mode\r\n");
    	deviceAdd = readDevAdd();
    	ltoa(deviceAdd,buffer);
    	putsUart0("Device Address: ");  putsUart0(buffer);  putsUart0("\r\n");

    	dmxTxDEN = 0;
    	waitMicrosecond(100);	// Stabilization time

    	poll_receive=false;
    	disableU1TxINT();
    	enableU1RxINT();

    	while(1)
    	{
    		deviceAdd = readDevAdd();
    		if(poll_receive)
    		{
    			if(readPollData(deviceAdd))
    			{
    				putcUart0('1');
    				dmxTxDEN = 1; 			  // DMX Write On
					waitMicrosecond(1);		  // Give time to properly convert from read to write mode
					brkFunc();				  // Send Break
					while(U1_TX_BUSY);

					UART1_IBRD_R = 10;        // Increase the Baud Rate (250Kbps)
					UART1_LCRH_R = UART_LCRH_WLEN_8 | UART_LCRH_STP2;
					// Everytime we change baudrate we must write to LCRH
					// Configure for 8N2 w/ 1-level FIFO

					//redTimeout = 250;		  // Set RED LED, indicate break is sent
					//LATBbits.LATB5 = 1;
					dmxTxDEN = 0; 				// DMX Read On
    			}
    			else
    				putcUart0('0');
    			poll_receive = false;
    			enableU1RxINT();

    		}
    	}

    }


}
