#ifndef TLC59116_H
	#define TLC59116_H
	#include "main.h"
	#include <util/twi.h>   		//TW-Defines
	#include "twi.h"
	#include "cube.h"
	
	#define TLC59116 	0b11000000
	#define PWM_RESET	0b11010110
	#if(CORE == Master) 
	# define PWMDRIVER1 	0b11000000
	# define PWMDRIVER2 	0b11000010
	# define PWMDRIVER3 	0b11000100
	# define PWMDRIVER4 	0b11000110
	#else
	# define PWMDRIVER1 	0b11001000
	# define PWMDRIVER2 	0b11001010
	# define PWMDRIVER3 	0b11001100
	# define PWMDRIVER4 	0b11001110
	#endif
	
	void TWI_59116_auslesen(void);
	void TWI_59116_setup(void);
	void TWI_59116_reset(void);
	void I2C_Leds_ein(unsigned char Ebene);
	
#endif
