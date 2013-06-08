#ifndef TWI_H
	#define TWI_H
	#include "main.h"
	#include <avr/io.h>
	#include <util/twi.h>   		//TW-Defines
	
	
	#define I2C_READ    1
	#define I2C_WRITE   0
	#define SCL_CLOCK  760000L			//800kHz I2C Frequenz	
	
	void TWI_init();
	unsigned char twi_start(unsigned char address);	//Reserviert den Bus und spricht die Slave Adresse an
	void twi_start_wait(unsigned char address); //Reserviert den Bus sobald er frei ist und spricht die Slave Adresse an
	void twi_stop(void);				//Beendet eine Übertragung und lässt den Bus wieder frei
	unsigned char twi_write( unsigned char data );	//Sendet ein Byte 
	unsigned char twi_readNak(void);	
	unsigned char twi_readAck(void); //Ein Byte lesen und weitere Daten erwarten
	
	
	
	
#endif
