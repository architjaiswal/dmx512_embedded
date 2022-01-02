
#ifndef UART0_H_
#define UART0_H_

// Initialize with 115200 8N1 w/ 16-level FIFO
void initUart0();

// Blocking function that writes a serial character when the UART buffer is not full
void putcUart0(char);

// Blocking function that writes a string when the UART buffer is not full
void putsUart0(const char*);

// Blocking function that returns with serial data once the buffer is not empty
// Return: The character
char getcUart0();

#endif /* UART0_H_ */
