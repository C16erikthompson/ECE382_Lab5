//-----------------------------------------------------------------
// Name:	Coulston
// File:	lab5.h
// Date:	Fall 2014
// Purp:	Include file for the MSP430
//-----------------------------------------------------------------

//-----------------------------------------------------------------
// Page 76 : MSP430 Optimizing C/C++ Compiler v 4.3 User's Guide
//-----------------------------------------------------------------
typedef		unsigned char		int8;
typedef		unsigned short		int16;
typedef		unsigned long		int32;
typedef		unsigned long long	int64;

#define		TRUE				1
#define		FALSE				0

//-----------------------------------------------------------------
// Function prototypes found in lab5.c
//-----------------------------------------------------------------
void initMSP430();
__interrupt void pinChange (void);
__interrupt void timerOverflow (void);


//-----------------------------------------------------------------
// Each PxIES bit selects the interrupt edge for the corresponding I/O pin.
//	Bit = 0: The PxIFGx flag is set with a low-to-high transition
//	Bit = 1: The PxIFGx flag is set with a high-to-low transition
//-----------------------------------------------------------------

#define		IR_PIN			(P2IN & BIT6)
#define		HIGH_2_LOW		P2IES |= BIT6
#define		LOW_2_HIGH		P2IES &= ~BIT6


#define		averageLogic0Pulse	594
#define		averageLogic1Pulse	1638
#define		averageStartPulse	4453
#define		minLogic0Pulse		averageLogic0Pulse - 220
#define		maxLogic0Pulse		averageLogic0Pulse + 220
#define		minLogic1Pulse		averageLogic1Pulse - 220
#define		maxLogic1Pulse		averageLogic1Pulse + 220
#define		minStartPulse		averageStartPulse - 220
#define		maxStartPulse		averageStartPulse + 220

#define		PWR		0x20DF10EF
#define		ONE		0x20DF8877
#define		TWO		0x20DF48B7
#define		THR		0x20DFC837
#define		ZER		0x20DF08F7

#define		VOL_UP	0x20DF40BF
#define		VOL_DW	0x20DFC03F
#define		CH_UP	0x20DF00FF
#define		CH_DW	0x20DF807F
