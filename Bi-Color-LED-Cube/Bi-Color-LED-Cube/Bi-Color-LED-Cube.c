/*
  ____  _         _____      _              _      ______ _____          _____      _          
 |  _ \(_)       / ____|    | |            | |    |  ____|  __ \        / ____|    | |         
 | |_) |_ ______| |     ___ | | ___  _ __  | |    | |__  | |  | |______| |    _   _| |__   ___ 
 |  _ <| |______| |    / _ \| |/ _ \| '__| | |    |  __| | |  | |______| |   | | | | '_ \ / _ \
 | |_) | |      | |___| (_) | | (_) | |    | |____| |____| |__| |      | |___| |_| | |_) |  __/
 |____/|_|       \_____\___/|_|\___/|_|    |______|______|_____/        \_____\__,_|_.__/ \___|
   ___            ___            ___                                                            
  / _ \          / _ \          / _ \                                                          
 | (_) | __  __ | (_) | __  __ | (_) |                                                         
  > _ <  \ \/ /  > _ <  \ \/ /  > _ <                                                          
 | (_) |  >  <  | (_) |  >  <  | (_) |                                                         
  \___/  /_/\_\  \___/  /_/\_\  \___/                                                          
 
 * Bi_Color_LED_Cube.c
 *
 * 
 *  Author: Manuel
 */ 

 
#include <avr/io.h>
#include "main.h"
#include "SPI.h"
#include "59116.h"
#include "twi.h"
#include "cube.h"

// Select Core as Master or Slave
//MASTER as MASTER
//SLAVE as SLAVE
#define MASTER	1
#define SLAVE	0


int main(void)
{
	TWI_init();					//TWI Bus Initialisieren
	TWI_59116_reset();
	TWI_59116_setup();
	
	#if MASTER
		DDRA=0xff;				//PORTA als Ausgang
		DDRD|=(1<<2);			// PD2 als Ausgang
		Interrupt_Master_Init();
		SPI_MasterInit();
		Ebene_ein=0;
		sei();
	#endif
	
	#if SLAVE
		DDRA=0x00;				//PORTA als Eingang
		DDRD&=~(1<<2);			// PD2 als Eingang
		Interrupt_Slave_Init();
		SPI_SlaveInit();
		Ebene_ein=0;
		sei();
    #endif
	
	while(1)
    {
        #if MASTER
			effect_rain(0,10);
		
		#endif
    }
}

#if MASTER
void Interrupt_Master_Init(void)
{
	// Timer-Interrupt Timer0
	TIMSK=(1<<OCIE0);					//Erlaubt Compare Interupt A
	TCCR0=(1<<FOC0);					//Schaltet den Compare Modus ein 
	TCCR0|=3;							//Prescaler 64
	OCR0=400; 							//Comparewert-Initialisierung auf ca. 5 ms bis zum ersten Interupt
	TCNT0=0;							//Timerwert auf 0 initialisieren
}


ISR(TIMER0_COMP_vect)					//Timer 0 Compare Interrupt startet hier
{
	TCCR0&=~3;							//Timer aus
	TCNT0=0;							//Z�hlwert wieder auf 0
	OCR0=400;
				//Interrupt Comparewert wieder laden
		
	//SPI_MasterTransmit(Ebene_ein%8);
	PORTD|=(1<<2);	//activate INT0
	
	I2C_Leds_ein(Ebene_ein%8);		//S�ulentreiber einschalten f�r n�chste Ebene
	PORTA=0xff;
	PORTA&=~(1<<Ebene_ein);
	Ebene_ein++;
	TCCR0|=3;							//Timer ein
	PORTD&=~(1<<2);											//deactivate INT0
}
#endif

#if SLAVE
void Interrupt_Slave_Init(void)
{ 
	// Externer Interrupt INT0
	GICR |= (1<<INT0);				//INT0 enable
	MCUCR = (1<<ISC00);				//INT0 rising edge
	MCUCR = (1<<ISC01);
}

ISR (INT0_vect)
{
	Ebene_ein = SPI_SlaveReceive();
	I2C_Leds_ein(Ebene_ein%8);		//S�ulentreiber einschalten f�r n�chste Ebene
	
}
#endif