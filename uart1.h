

#ifndef UART1_H_
#define UART1_H_

// Initialize UART1 with 250k 8N2 w/ UART1 Interrupt enable and 1-level FIFO
void initUart1(void);

// Blocking function that writes a serial character when the UART buffer is not full
void putcUart1(char);

// Blocking function that writes a string when the UART buffer is not full
void putsUart1(const char*);

// Blocking function that returns with serial data once the buffer is not empty
// Returns: The character
char getcUart1(void);

// Enable Tx Interrupt
void enableU1TxINT(void);

// Enable Receiver (Frame error & Rx) Interrupt
void enableU1RxINT(void);

// Disable Transmit Interrupt
void disableU1TxINT(void);
// Disable Receiver (Frame error & Rx) Interrupt
void disableU1RxINT(void);

#endif /* UART1_H_ */
