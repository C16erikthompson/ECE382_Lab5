ECE382_Lab5
===========

#Day 1
##Questions
1)How long will it take the timer to roll over?   
a)64.53 millisecond   
2)How long does each timer count last?    
a)1 microsecond   

![alt test](http://i47.photobucket.com/albums/f189/erik_thompson2/lab5pic_zps2ab53bec.png)

##IR Data Packets
*New Pictures of data analyzer pending (I destroyed my phone with the pictures)
#####Pulse Lengths
| Pulse                      | Duration (ms) | Timer A counts |
|----------------------------|---------------|----------------|
| Start   logic 0 half-pulse | 9.062         | 8.971          |
| Start logic 1 half-pulse   | 4.453         | 4.419          |
| Data 1 logic 0 half-pulse  | 0.5937        | 0.592          |
| Data 1 logic 1 half-pulse  | 1.656         | 1.641          |
| Data 0 logic 0 half-pulse  | 0.5936        | 0.594          |
| Data 0 logic 1 half-pulse  | 0.5625        | 0.521          |
| Stop logic 0 half-pulse    | 0.6093        | 0.599          |
| Stop logic 1 half-pulse    | 36.406        | 39.27          |

####Length Samples With Means and Standard Deviations
|       | Data 1, logic 1 half-pulse | Data 0, logic 0, half pulse | Data 0, logic 1 half pulse |
|-------|----------------------------|-----------------------------|----------------------------|
|       | 1.641                      | 0.595                       | 0.53                       |
|       | 1.641                      | 0.594                       | 0.523                      |
|       | 1.636                      | 0.597                       | 0.528                      |
|       | 1.638                      | 0.593                       | 0.525                      |
|       | 1.639                      | 0.593                       | 0.519                      |
|       | 1.642                      | 0.599                       | 0.528                      |
|       | 1.64                       | 0.597                       | 0.525                      |
|       | 1.633                      | 0.592                       | 0.521                      |
|       | 1.642                      | 0.592                       | 0.527                      |
|       | 1.635                      | 0.594                       | 0.522                      |
| Mean  | 1.6387                     | 0.5946                      | 0.5248                     |
| StDev | 0.002968164                | 0.002244994                 | 0.003340659                |

####IR Code for Buttons
| Button | code (not including start and   stop bits) | Hex      |
|--------|--------------------------------------------|----------|
| 0      | 0010 0000 1101 1111 0000 1000 1111 0111    | 20DF08F7 |
| 1      | 0010 0000 1101 1111 1000   1000 0111 0111  | 20DF8877 |
| 2      | 0010 0000 1101 1111 0100 1000 1011 0111    | 20DF48B7 |
| 3      | 0010 0000 1101 1111 1100   1000 0011 0111  | 20DFC837 |
| Power  | 0010 0000 1101 1111 0001 0000 1110 1111    | 20DF10EF |
| Vol +  | 0010 0000 1101 1111 0100   0000 1011 1111  | 20DF40BF |
| Vol -  | 0010 0000 1101 1111 1100 0000 0011 1111    | 20DFC03F |
| Ch +   | 0010 0000 1101 1111 0000   0000 1111 1111  | 20DF00FF |
| Ch -   | 0010 0000 1101 1111 1000 0000 0111 1111    | 20DF807F |

#Day 2
##Required Functionality

Achieving required functionality was fairly easy requiring minimal changes to the code given.  In the main loop, if statements were added to the infinite loop that toggle the red and green LEDs with the press of the one button for the green LED and the two button for the red LED.  Code that implements this functionality appears below.
~~~
	while(1)  {
		if(packetIndex==34){
			if(irPacket == ONE){
				P1OUT ^= BIT6;
			}else if(irPacket == TWO){
				P1OUT ^= (BIT0);
			}
			newPacket = FALSE;
		}


	} // end infinite loop
~~~
	The other change that had to be made to this was altering the ISR that was given.  No major changes had to be made, just classifying pulseDuration as start, 0, or 1.
~~~
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
	 // end switch


	P2IFG &= ~BIT6;			// prevent immediate re-entry

} // end pinChange ISR
~~~

##A Functionality
###Hardware
![alt test](http://i47.photobucket.com/albums/f189/erik_thompson2/IMG_20141112_232831_476_zps3b39dd07.jpg)

###Program
Going from required to A functionality was a fairly easy process.  For my functionality, I did the etch-a-sketch program.  To do this, I used the .asm file from lab 4 and the code of the lab 4 main.c as the backbone for this program.  In place of the buttons, I used the IR classificaitons of the 1, 2, 3, and 0 buttons for left, up, right, and down respectively.  I also used the power button to toggle the color of the drawing cursor.  In addition to these minor changes, a block of code was included before the button reads that determines the IR signal passed to the reciever.  It does this by shifting in a one or a zero to an irpacket depending upon the current state of an array.  The code for this functionality is shown below.

~~~
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
~~~

#Debugging
When I initially completed the A functionality, I had an issue where moving the the cursor around the screen would leave "residual pixels" randomly about the screen.  Though I did eventually fix this issue, I am somewhat uncertain of what fixed it as I was just tinkering with my code and it eventually worked...  Though not an issue, my original iteration of the code made use of a number of global variables that were utilized as flags to check the status of the system.  Upon further review of the code, many of these flags were found to be redundant and my code was made much cleaner and concise.

#Functionality
I had my functionality checked by Dr.Coulston on Nov. 10.

#Documentation
C2C Terragnoli assisted me with understanding the logic ones and zeroes on the digital analyzer   
C2C Dustin Weisner helped me understand the IR packets and the ISR that had to be altered
