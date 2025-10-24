#include <16F616.h>
#device ADC=8

#FUSES PUT                   	//Power Up Timer
#FUSES BROWNOUT              	//Reset when brownout detected

#use delay(internal=8MHz)

#define LED PIN_A2
#define DELAY 500


