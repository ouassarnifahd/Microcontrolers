#include <pic18f4550.h>

/** T0CON Register
*
*   +--------+--------+--------+--------+--------+--------+--------+--------+
*   | TMR0ON | T08BIT |  T0CS  |  T0SE  |  PSA   |  T0PS2 |  T0PS1 |  T0PS0 |
*   *--------+--------+--------+--------+--------+--------+--------+--------+
*
*   TMR0ON: Timer0 On/Off Control bit
*       1 = Enables Timer0
*       0 = Stops Timer0
*
*   T08BIT: Timer0 8-bit/16-bit Control bit
*       1 = Timer0 is configured as an 8-bit timer/counter
*           TMR0L = counter register
*       0 = Timer0 is configured as a 16-bit timer/counter
*           TMR0H,TMR0L = counter registers
*
*   T0CS: Timer0 Clock Source Select bit
*       1 = Transition on T0CKI pin
*       0 = Internal instruction cycle clock (CLKO)
*
*   T0SE: Timer0 Source Edge Select bit
*       1 = Increment on high-to-low transition on T0CKI pin
*       0 = Increment on low-to-high transition on T0CKI pin
*
*   PSA: Timer0 Prescaler Assignment bit
*       1 = Timer0 prescaler is NOT assigned. Timer0 clock input bypasses prescaler.
*       0 = Timer0 prescaler is assigned. Timer0 clock input comes from prescaler output.
*
*   T0PS2:T0PS0: Timer0 Prescaler Select bits
*       111 = 1:256 Prescale value
*       110 = 1:128 Prescale value
*       101 = 1:64  Prescale value
*       100 = 1:32  Prescale value
*       011 = 1:16  Prescale value
*       010 = 1:8   Prescale value
*       001 = 1:4   Prescale value
*       000 = 1:2   Prescale value
*
*/

/** RCON Register (for interrupt priorities)
*
*   +--------+--------+--------+--------+--------+--------+--------+--------+
*   |  IPEN  | SBOREN | —————— |   RI   |   TO   |   PD   |  POR   |  BOR   |
*   *--------+--------+--------+--------+--------+--------+--------+--------+
*
*   IPEN: Interrupt Priority Enable bit
*       1 = Enable priority levels on interrupts
*       0 = Disable priority levels on interrupts (PIC16CXXX Compatibility mode)
*
*   OTHERS: datasheet page 108
*
*/

/** INTCON Register
*
*   +--------+--------+--------+--------+--------+--------+--------+--------+
*   |  GIEH  |  GIEL  | TMROIE | INT0IE |  RBIE  | TMR0IF | INTOIF |  RBIF  |
*   *--------+--------+--------+--------+--------+--------+--------+--------+
*
*   GIE/GIEH: Global Interrupt Enable bit
*       When IPEN = 0:
*           1 = Enables all unmasked interrupts
*           0 = Disables all interrupts
*       When IPEN = 1:
*           1 = Enables all high priority interrupts
*           0 = Disables all high priority interrupts
*
*   PEIE/GIEL: Peripheral Interrupt Enable bit
*       When IPEN = 0:
*           1 = Enables all unmasked peripheral interrupts
*           0 = Disables all peripheral interrupts
*       When IPEN = 1:
*           1 = Enables all low priority peripheral interrupts
*           0 = Disables all low priority peripheral interrupts
*
*   TMR0IE: TMR0 Overflow Interrupt Enable bit
*       1 = Enables the TMR0 overflow interrupt
*       0 = Disables the TMR0 overflow interrupt
*
*   TMR0IF: TMR0 Overflow Interrupt Flag bit
*       1 = TMR0 register has overflowed (must be cleared in software)
*       0 = TMR0 register did not overflow
*
*   INT0IF: INT0 External Interrupt Flag bit
*       1 = The INT0 external interrupt occurred (must be cleared in software)
*       0 = The INT0 external interrupt did not occur
*
*   OTHERS: datasheet page 99
*
*/

/** INTCON2 Register
*
*   +--------+--------+--------+--------+--------+--------+--------+--------+
*   |  RBPU  | INTEDG0| INTEDG1| INTEDG2| —————— | TMR0IP | —————— |  RBIP  |
*   *--------+--------+--------+--------+--------+--------+--------+--------+
*
*   TMR0IP: TMR0 Overflow Interrupt Priority bit
*       1= Highpriority
*       0= Lowpriority
*
*   OTHERS: datasheet page 100
*
*/

#pragma code
#pragma interruptlow low_isr
void low_isr(void) {

    if(INTCONbits.TMR0IF)
    {
		/*** Clear Timer0 overflow flag ***/

    		INTCONbits.TMR0IF = 0;

		/*** resetting TMR0L. TMR0H is reset automaticaly ***/

    		TMR0L = 0xFF;

        // here the code to execute when timer0 finish counting

    }
}

#pragma code low_vector = 0x18
void interrupt_at_low_vector(void) {
   _asm goto low_isr _endasm
}

#pragma code
void Timer_Config(void) {
	/*** T0CON: Timer0=OFF, mode=16bits, synchro=Tcy, prescaler=ON, prescale value=1:256 ***/

    T0CON = 0x07; // OOOO_O111

	/*** TMR0: we have to use this formula time = Timer0 clock source x prescale value x count value and so TMR0H,TMR0L = 0xFFFF - count value ***/

    TMR0H = 0xFF;
    TMR0L = 0xFF;

	/*** INTERRUPTIONS: priority mode, Lowpriority, unmask TMR0, TMR0IF=0 ***/

    RCONbits.IPEN = 1;
    INTCON2bits.TMR0IP = 0;
    INTCONbits.TMR0IE = 1;
    INTCONbits.GIEH = 1;
    INTCONbits.GIEL = 1;
    INTCONbits.TMR0IF = 0;

	/*** Timer0 Start ***/

    T0CONbits.TMR0ON = 1;
}
