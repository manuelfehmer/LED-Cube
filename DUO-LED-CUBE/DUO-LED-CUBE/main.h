#ifndef MAIN_H
	#define MAIN_H
	
	#ifndef F_CPU
		#define F_CPU 20000000UL			//CPU Frequenz 20Mhz
	#endif
	
	#include <avr/io.h>
	#include <util/delay.h>
	#include <avr/interrupt.h>
	#include <avr/wdt.h>     		/*Bibliothek für Watchdog-Funktionen*/
	#include <util/twi.h>   		//TW-Defines
	
	


#endif
