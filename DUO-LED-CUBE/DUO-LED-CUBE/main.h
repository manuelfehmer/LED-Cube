#ifndef MAIN_H
	#define MAIN_H
	
	#ifndef F_CPU
		#define F_CPU 20000000UL	//CPU Frequenz 20Mhz
	#endif
	
	#include <avr/io.h>
	#include <util/delay.h>
	#include <avr/interrupt.h>
	#include <avr/wdt.h>     		/*Bibliothek für Watchdog-Funktionen*/
	#include <util/twi.h>   		//TW-Defines
	
	#define Master 0
	#define Slave 1
	#define CORE 1				// define as Master use 0 otherwise Slave use 1
	


#endif
