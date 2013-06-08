#ifndef UART_H
	#define UART_H
	
	#include <avr/io.h>
	#include "main.h"
	

	#define BAUD 115200UL      // Baudrate // Berechnungen für Baudrate
	#define UBRR_VAL ((F_CPU+BAUD*8)/(BAUD*16)-1)   // clever runden 
	#define BAUD_REAL (F_CPU/(16*(UBRR_VAL+1)))     // Reale Baudrate
	#define BAUD_ERROR ((BAUD_REAL*1000)/BAUD) 		// Fehler in Promille, 1000 = kein Fehler.
	#if ((BAUD_ERROR<980) || (BAUD_ERROR>1020))
	  #error Systematischer Fehler der Baudrate grösser 1% und damit zu hoch! 
	#endif  
	#define UART_MAXSTRLEN 65
	
	//Funktionen
	void uart_init(void);
	void uart_send_char(uint8_t Data);
	void uart_send_string(char *s);
	char uart_recv_char(void);
	void uart_send_int(uint8_t Data);
	void uart_send_int2(uint16_t Data);
	void uart_send_data(uint8_t *cData,uint8_t len);
	
#endif
