#include <18F25K22.h>
#device ADC=8

#FUSES NOWDT                 	//No Watch Dog Timer
#FUSES PRIMARY               	//Primary clock is system clock when scs=00
#FUSES FCMEN                 	//Fail-safe clock monitor enabled
#FUSES IESO                  	//Internal External Switch Over mode enabled
#FUSES PUT                   	//Power Up Timer
#FUSES BROWNOUT              	//Reset when brownout detected
#FUSES BORV19                	//Brownout reset at 1.9V
#FUSES CCP3B5                	//CCP3 input/output multiplexed with RB5
#FUSES HFOFST                	//High Frequency INTRC starts clocking CPU immediately
#FUSES TIMER3C0              	//Timer3 Clock In is on pin C0
#FUSES CCP2B5                	//CCP2 input/output multiplexed with RB5
#FUSES NOPROTECT             	//Code not protected from reading
#FUSES NOWRT                 	//Program memory not write protected
#FUSES NOEBTR                	//Memory not protected from table reads
#FUSES NOMCLR

#use delay(internal=16MHz)

#define LED PIN_C3
#define DELAY 500


