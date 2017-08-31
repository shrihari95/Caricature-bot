#include"UART.h"

char rbuf[RX_BUF_SIZE];
int rin;
int rout;
unsigned char lineReceived;

void UART_init()
{

	/* Initialize asynchronous serial port (SCI) for 9600 baud, no interrupts */
	SCIBDH = 0x00; //set baud rate to 9600
	SCIBDL = 0x0D; //24,000,000 / 16 / 156(9C) = 9600 (approx)  or //24,000,000 / 16 / 13(0D) = 115200 (approx)

  //SCIBDH = 0x13;
  //SCIBDL = 0x88;
  
	SCICR1 = 0x00;
	SCICR2 = 0x2C; //initialize SCI for RX_Int

	DDRB = 0x10; //set PB4 for output mode
	PORTB = 0x10; //assert DTR pin on COM port


	// Buffer Initializations
	rin = 0;  //where to write into next
	rout = 0; //where to read from next
	lineReceived = 0;
}

void UART_fillBuf()
{
	rbuf[0] = '4';
	rbuf[1] = '0';
	rbuf[2] = '0';
	rbuf[3] = '|';
	rbuf[4] = '5';
	rbuf[5] = '0';
	rbuf[6] = '0';
	rbuf[7] = '|';
	rbuf[8] = '6';
	rbuf[9] = '0';
	rbuf[10] = '0';
	rbuf[11] = '|';
	rbuf[12] = '0';
	rbuf[13] = '|';
	rbuf[14] = '0';
	rbuf[15] = CR;
	rbuf[16] = LF;
}

void UART_print(char* str) {

	char* s = str;

	while (*s != 0) {
		UART_outChar(*s++);
	}

}

void UART_println(char* str) {
	char* s = str;
	while (*s != 0) {
		UART_outChar(*s++);
	}
	UART_outChar(CR);
	UART_outChar(LF);
}


/*
***********************************************************************
Character I/O Library Routines for 9S12C32 (for debugging only)
***********************************************************************
Name:         inchar
Description:  inputs ASCII character from SCI serial port and returns it
Example:      char ch1 = inchar();
***********************************************************************
*/
char UART_inChar(void) {
	/* receives character from the terminal channel */
	while (!(SCISR1 & 0x20)); /* wait for input */
	return SCIDRL;
}

/*
***********************************************************************
Name:         outchar
Description:  outputs ASCII character x to SCI serial port
Example:      outchar('x');
***********************************************************************
*/
void UART_outChar(char x) {
	/* sends a character to the terminal channel */
	while (!(SCISR1 & 0x80));  /* wait for output buffer empty */
	SCIDRL = x;
}


/*
***********************************************************************
SCI (transmit section) interrupt service routine, fired when a
character is received
***********************************************************************
*/
interrupt 20 void SCI_ISR(void)
{



	// Service Recieve Interrupt
	if (((rin + 1) % RX_BUF_SIZE) != rout) {

		char c = UART_inChar();
		UART_outChar(c);

		if (c == CR)  // Discard Carriage Return
		{
		  //UART_outChar('C');
			return;
		}

		if (c == LF) {
		  //UART_outChar('L');
		  
		  UART_outChar(XOFF);
	    UART_println("XOFF");
			rbuf[rin] = 0; // Null Terminated
			
			lineReceived=1;
			
		}
		else {

			rbuf[rin] = c;
		}

		rin = (rin + 1) % RX_BUF_SIZE;

	}
	else
	{
	  UART_print("Buff-Overflow")	;
		UART_outChar(XOFF);
	}



}