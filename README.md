# dmx512_embedded
DMX 512 Stage lighting controller 


It can be programmed to communicate with other microcontroller devices through I2C communication protocol and can also issue master controlling commands to change the color and ON/OFF timing of stage lights. 

After deploying this software on a low-end TI TM4C microcontroller, the device can act as either a controller (master) or slave to perform a timing intensive asynchronous communication interface based on the DMX512-A protocol with EF3 topology

In master mode, it can command 512 slave devices along with a UART communication. 
