//-----------------------------------------------------------------
// Name:	Erik Thompson
// File:	lab5.c
// Date:	Fall 2014
// Purp:	IR controlled etch-a-sketch
//-----------------------------------------------------------------

#include <msp430g2553.h>
#include "start5.h"

int16	packetData[34];
int8	packetIndex = 0;

// -----------------------------------------------------------------------
// -----------------------------------------------------------------------
void main(void) {

//------------------------------------------------------------------
//		Setup
//------------------------------------------------------------------

	//position of the block
	unsigned char color = 0xFF;
	unsigned char x, y;

	IFG1=0; 					// clear interrupt flag1
	WDTCTL=WDTPW+WDTHOLD; 		// stop WD

	BCSCTL1 = CALBC1_8MHZ;		//calibrate clock
	DCOCTL = CALDCO_8MHZ;


	init();
	initNokia();
	initMSP430();
	clearDisplay();
	drawBlock(y,x,color);

	x = 5;
	y = 4;

	initMSP430();

//----------------------------------------------------------------
//		Determine value of irPacket
//----------------------------------------------------------------

	while(1){
		if(packetIndex == 34){		//if full signal recieved
			_disable_interrupt();
			int32 irPacket = 0;
			int32 setBit = 0xF0000000;				//1 in the MSB

			char i;
			for(i = 2; i<34; i++){
				int16 current = packetData[i];		//current element

				if(current/10 < 100){
					irPacket &= ~setBit;
				}
				else {
					irPacket |= setBit;
				}
				setBit >>= 1;
				}

			packetIndex++;							//ensure the loop is not reentered

			initNokia();							//prepare to write to LCD

//----------------------------------------------------------------
//		Handle the buttonn presses
//----------------------------------------------------------------
			if (irPacket == PWR) {
							i++;
							if(i%2 == 0){
								color = 0xFF;
							}
							else{
								color = 0x00;
							}

							init();
							initNokia();
							initMSP430();
							drawBlock(y,x,color);

						}

						if (irPacket == TWO) {
							if (y>=1) y=y-1;

						} else if (irPacket == ZER) {
							if (y<=6) y=y+1;

						} else if (irPacket == ONE) {
							if (x>=1) x=x-1;

						} else if (irPacket == THR) {
							if (x<=10) x=x+1;

						}

//----------------------------------------------------
//----------------------------------------------------


			drawBlock(y, x, color);		//redraw the block
			initMSP430();						//reprepare to recieve IR
		}
	}
} // end main


// -----------------------------------------------------------------------
// In order to decode IR packets, the MSP430 needs to be configured to
// tell time and generate interrupts on positive going edges.  The
// edge sensitivity is used to detect the first incoming IR packet.
// The P2.6 pin change ISR will then toggle the edge sensitivity of
// the interrupt in order to measure the times of the high and low
// pulses arriving from the IR decoder.
//
// The timer must be enabled so that we can tell how long the pulses
// last.  In some degenerate cases, we will need to generate a interrupt
// when the timer rolls over.  This will indicate the end of a packet
// and will be used to alert main that we have a new packet.
// -----------------------------------------------------------------------
void initMSP430() {

	//setup interrupt pin
	P2SEL  &= ~BIT6;						// Setup P2.6 as GPIO not XIN
	P2SEL2 &= ~BIT6;
	P2DIR &= ~BIT6;
	P2IFG &= ~BIT6;						// Clear any interrupt flag
	P2IE  |= BIT6;						// Enable PORT 2 interrupt on pin change

	HIGH_2_LOW;

	TA0CCR0 = 0xFFFF;					// create a  roll-over period
	TACTL &= ~TAIFG;					// clear flag before enabling interrupts = good practice
	TACTL = ID_3 | TASSEL_2 | MC_1 | TAIE;		// Use 1:1 presclar off MCLK and enable interrupts

	_enable_interrupt();
}

// -----------------------------------------------------------------------
// Since the IR decoder is connected to P2.6, we want an interrupt
// to occur every time that the pin changes - this will occur on
// a positive edge and a negative edge.
//
// Negative Edge:
// The negative edge is associated with end of the logic 1 half-bit and
// the start of the logic 0 half of the bit.  The timer contains the
// duration of the logic 1 pulse, so we'll pull that out, process it
// and store the bit in the global irPacket variable. Going forward there
// is really nothing interesting that happens in this period, because all
// the logic 0 half-bits have the same period.  So we will turn off
// the timer interrupts and wait for the next (positive) edge on P2.6
//
// Positive Edge:
// The positive edge is associated with the end of the logic 0 half-bit
// and the start of the logic 1 half-bit.  There is nothing to do in
// terms of the logic 0 half bit because it does not encode any useful
// information.  On the other hand, we going into the logic 1 half of the bit
// and the portion which determines the bit value, the start of the
// packet, or if the timer rolls over, the end of the ir packet.
// Since the duration of this half-bit determines the outcome
// we will turn on the timer and its associated interrupt.
// -----------------------------------------------------------------------
#pragma vector = PORT2_VECTOR			// This is from the MSP430G2553.h file
__interrupt void pinChange (void) {

	int8	pin;
	int16	pulseDuration;			// The timer is 16-bits

	if (IR_PIN)
		pin=1;
	else
		pin=0;

		if(pin == 0){					// !!!!!!!!!NEGATIVE EDGE!!!!!!!!!!
			pulseDuration = TAR;
			TACTL = 0;
			packetData[packetIndex++] = pulseDuration;
			LOW_2_HIGH; 				// Setup pin interrupr on positive edge
		}
		else{							// !!!!!!!!POSITIVE EDGE!!!!!!!!!!!
			TAR = 0x0000;						// time starts at 0
			TA0CCR0 = 0xFF00;					// 16mS roll-over period
			TACTL &= ~TAIFG;
			TACTL = ID_3 | TASSEL_2 | MC_1 | TAIE;		// Use 1:1 presclar off MCLK and enable interrupts
			HIGH_2_LOW; 						// Setup pin interrupr on positive edge
		}
	} // end switch


	P2IFG &= ~BIT6;			// prevent immediate re-entry

} // end pinChange ISR



// -----------------------------------------------------------------------
// This interrupt is a way of reseting the reciever when something gets messed up
// It also resets packetIndex after the final pulse is recieved
// It is only active while IRPIN is high
//
// -----------------------------------------------------------------------
#pragma vector = TIMER0_A1_VECTOR			// This is from the MSP430G2553.h file
__interrupt void timerOverflow (void) {

	initMSP430();

	packetIndex = 0;

	TACTL &= ~TAIFG;		//clear flag

}
