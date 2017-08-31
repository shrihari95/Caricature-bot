
/* Hardware Drivers */
#include"Drivers/base.h"
#include"Drivers/TIM/TIM.h"
#include"Drivers/SPI/SPI.h"
#include"Drivers/UART/UART.h"

/* Motion Control System */
#include"NCK/Interpolator/Interpolator.h"
#include"NCK/Interpretor/Interpretor.h"

#include<hidef.h>
#include<stdlib.h>

// User variables
int leftpb = 0;    // left pushbutton flag
int rghtpb = 0;    // right pushbutton flag
int prevLpb = 0;
int prevRpb = 0;

// Shared Variable
int theta[AxisCount] = { 512,512,512,0,0 };
char str[RX_BUF_SIZE] = "";

void scanButtons();

/* System Initializations  */
void initialize()
{
	//Sub-system initializations

	BASE_init();
	SPI_init();
	TIM_init();
	UART_init();

	Interpolator_init();
	Interpretor_init();

}


void loop()
{

	if (lineReceived) {

		// Debug Output
		//UART_println(rbuf);

		// Process the received line
		char* ptr = str;
		int i = 0;

		while (rbuf[rout] != 0) {

			char c = rbuf[rout];

			if (c != '|') {
				*ptr++ = c;
			}
			else {
				*ptr = 0;
				theta[i++] = atoi(str);
				ptr = str;
			}

			// Increment rout
			rout = (rout + 1) % RX_BUF_SIZE;

		}
		// For NuLL Char
		rout = (rout + 1) % RX_BUF_SIZE;

		// Sned the received angular position to Motors
		SPI_TX(theta, AxisCount);
		//delay(3000);
		lineReceived = 0;


		// Recover from Emergerncy Stop
		if (!leftpb) {
			UART_outChar(XON);
			UART_println("XON");
		}

	}


	if (rghtpb) {

		if (leftpb) {

			leftpb = 0;
			UART_outChar(XON);
			UART_println("XON");

			rghtpb = 0;
		}

	}

}


// Main Entrypoint
/* please make sure that you never leave main */
void main(void)
{
	// Disable Interrupts for Initialization
	DisableInterrupts;

	// Initialize all sub-systems
	initialize();

	// Enable back Interrupts
	EnableInterrupts;

	// Loop forever
	for (;;)
	{

		//_FEED_COP(); /* feeds the dog */
		//COP off; RTI and COP stopped in BDM-mode

		loop();

	}

}


void scanButtons() {
	PTT_PTT1 ^= 1;


	// Read Butons
	if (PTAD_PTAD7 < prevLpb)
	{
		leftpb = 1;
	}
	prevLpb = PTAD_PTAD7;

	if (PTAD_PTAD6 < prevRpb)
	{
		rghtpb = 1;
	}
	prevRpb = PTAD_PTAD6;
}
