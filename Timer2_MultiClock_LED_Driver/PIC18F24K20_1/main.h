#include <18F24K20.h>
#device ADC=10

#FUSES FCMEN                 	//Fail-safe clock monitor enabled
#FUSES IESO                  	//Internal External Switch Over mode enabled
#FUSES PUT                   	//Power Up Timer
#FUSES NOBROWNOUT            	//No brownout reset
#FUSES BORV27                	//Brownout reset at 2.7V
#FUSES NOPBADEN              	//PORTB pins are configured as digital I/O on RESET
#FUSES LPT1OSC               	//Timer1 configured for low-power operation
#FUSES HFOFST                	//High Frequency INTRC starts clocking CPU immediately
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

#use delay(internal=32MHz)

#define LED PIN_A7
#define DELAY 500


