# PIC18 Family - 8-bit Microchip Microcontrolers

## Architecture: Harvard
    Read-Address-Bus:        21-bit -> 2M Addressable-Memory-Location (write only)
    Write-Address-Bus:       12-bit -> 4k Addressable-Memory-Location (write only)
    Instruction-Address-Bus: 16-bit (read only)
    Data-Address-Bus:         8-bit (read/write)

    PROGRAM MEMORY 2Mb (2048 Kb)
                ||
        INSTRUCTION 16-bit
                \/
     /----------------------\
     |        1.FETCH       |
     |----------------------|
     |       2.DECODE       |
     |       3.EXECUTE      |
     |      4.WRITE-BACK    |
     \----------------------/
                /\
          DATA  ||  16-bit
                \/
     DATA MEMORY 4Kb (4096-bit)
    [BSR(0x0-0xF) / SFR(0x0|0xF)]

## PERIPHERALS: Hardware limits

+ Control registers: Protocols
+ Work registers   : Protocols + DATA received/transmitted

## GPIO: General purpose Input Output

+ #### LATx : WRITE TO REGISTER
+ #### PORTx: READ FROM REGISTER
+ #### TRISx: IO ENCODING REGISTER

## INTERRUPTIONS: (USING GLOBAL VARIABLES)

+ ### IRQ: Interrupt ReQuest
    + #### IF: Interrupt Flag
    + #### IE: Interrupt Enable
        + ##### GIE: Global Interrupt Enable
    + #### IP: Interrupt Priority

            /* system - disable PIC16 legacy mode */
                RCONbits.IPEN = 1;
            /* timer0 - interrupt configuration – high priority vector */
                INTCONbits.TMR0IF = 0;
                INTCON2bits.TMR0IP = 1;
                INTCONbits.TMR0IE = 1;
            /* system - global interrupt enable */
                INTCONbits.GIEH=1;

+ ### Interruption Vectors:
        Memory-Addresses containing: goto ISR

+ ### Context "commuting":
        Save mainProgram -> ISR -> Restore mainProgram
+ ### ISR: Interrupt Service Routine (function|software)

            /* ISR - high level Interrupt Service Routine */
                void interrupt high_priority high_isr (void) {
                    /* check for timer1 IRQ */
                        if( INTCONbits.TMR0IF ) {
                            /* IRQ acknowledgment */
                                INTCONbits.TMR0IF = 0; /* ACK */

                            /* user ISR processing */
                        }
                }
            /* program entry point */
                void main(void) {
                    timer0_init();
                    while(1) {
                        /* user application scheduler */
                    }
                }
## Others
+ FUNCTION CALL: PC, GOTO, RETURN
+ OFFLINE SCHEDULER:
