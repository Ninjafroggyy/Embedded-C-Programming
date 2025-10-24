#include <18F45K50.h>
#device ADC=8

#FUSES PRIMARY               	//Primary clock is system clock when scs=00
#FUSES FCMEN                 	//Fail-safe clock monitor enabled
#FUSES IESO                  	//Internal External Switch Over mode enabled
#FUSES PUT                   	//Power Up Timer
#FUSES BROWNOUT              	//Reset when brownout detected
#FUSES BORV19                	//Brownout reset at 1.9V
#FUSES NOLPBOR               	//Low-Power Brownout reset is disabled
#FUSES PBADEN                	//PORTB pins are configured as analog input channels on RESET
#FUSES T3CKC0                	//T3 Clock In is on C0
#FUSES SDOB3                 	//SDO is on RB3
#FUSES NOMCLR                	//Master Clear pin used for I/O
#FUSES STVREN                	//Stack full/underflow will cause reset
#FUSES NOLVP                 	//No low voltage prgming, B3(PIC16) or B5(PIC18) used for I/O
#FUSES NOXINST               	//Extended set extension and Indexed Addressing mode disabled (Legacy mode)
#FUSES NOPROTECT             	//Code not protected from reading
#FUSES NOCPB                 	//No Boot Block code protection
#FUSES NOCPD                 	//No EE protection
#FUSES NOWRT                 	//Program memory not write protected
#FUSES NOWRTC                	//Configuration registers not write protected
#FUSES NOWRTB                	//Boot block not write protected
#FUSES NOWRTD                	//Data EEPROM not write protected
#FUSES NOEBTR                	//Memory not protected from table reads
#FUSES NOEBTRB               	//Boot block not protected from table reads

#use delay(internal=16MHz)
#define But1	PIN_B0
#define But2	PIN_B1
#define But3	PIN_B2
#define But4	PIN_B3
#define But5	PIN_B4
#define LED1	PIN_E0
#define LED2	PIN_E1
#define LED3	PIN_E2

#use rs232(baud=9600, parity=N, xmit=PIN_e0, rcv=PIN_e1, bits=8, stream=PORT1, errors)


#define LED PIN_E2
#define DELAY 500


