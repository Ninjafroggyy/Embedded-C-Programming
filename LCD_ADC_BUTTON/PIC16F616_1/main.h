#include <16F616.h>
#device ADC=8

#FUSES PUT                   	//Power Up Timer
#FUSES BROWNOUT              	//Reset when brownout detected
#FUSES NOMCLR                	//Master Clear pin used for I/O
#FUSES NOPROTECT             	//Code not protected from reading

#use delay(internal=8MHz)

#define LED PIN_A2
#define DELAY 500


