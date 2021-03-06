/*
 * DUO_LED_CUBE.c
 *
 * Created: 08.06.2013 14:32:29
 *  Author: Manuel
 */ 

#include "main.h"
#include "SPI.h"
//#include "uart.h"
#include "font.h"
#include "59116.h"
#include "twi.h"
#include "cube.h"
//#include "gameoflife.h"
#include "eeprom.h"

//Funktionen ###########################################################
void Interrupt_init(void);
ISR (TIMER0_COMP_vect);				

//Variablen


int main (void)
{
	TWI_init();					//TWI Bus Initialisieren
	TWI_59116_reset();
	TWI_59116_setup();
	
	if(CORE==Master){
		DDRA=0xff;				//PORTA als Ausgang
		DDRD|=(1<<2);			// PD2 als Ausgang
		Interrupt_init();
		sei();
		SPI_MasterInit();
		Ebene_ein=0;
		
		while(1)
		{
		
				
			effect_blinky2(1); //ok
			effect_planboing(0,AXIS_Z,20); //ok
			effect_planboing(1,AXIS_X,20); //ok
			effect_planboing(0,AXIS_Y,20); //ok
			effect_planboing(1,AXIS_Z,20); //ok
			for (uint8_t ii=0;ii<8;ii++)
				effect_box_shrink_grow (0,ii%4, ii & 4, 20); //ok
			//sendvoxels_rand_z(100,10,20); // ok
			effect_random_sparkle (1,3,20,10); //ok
			effect_box_woopwoop(1,40,1); //naja
			effect_rain(0,300); //ok
			//effect_wormsqueeze (2, AXIS_Z, 1, 100, 25); //ok
			for(int i=0;i<8;i++){			//	Pixel durchtesten
				for(int j=0;j<64;j++)
				{	LED[0][i][j/8][j%8]=0xff;
					_delay_us(500);
				}
				for(int j=0;j<64;j++)
				{	LED[0][i][j/8][j%8]=0;
					_delay_us(500);
				}
			}
		}
	
	}else{
		DDRA=0x00;				//PORTA als Eingang
		DDRD&=~(1<<2);			// PD2 als Eingang
		SPI_SlaveInit();
		Ebene_ein=0;
		while(1)
		{
			
			I2C_Leds_ein(Ebene_ein%8);		//S�ulentreiber einschalten f�r n�chste Ebene
							
		}
	}

	
return 0;
}
/*
ISR (INT0_vect)
{
	if(CORE == Slave){
		//Ebene_ein= SPI_SlaveReceive();
		I2C_Leds_ein(Ebene_ein%8);		//S�ulentreiber einschalten f�r n�chste Ebene
		Ebene_ein++;
	}	
}*/

ISR(TIMER0_COMP_vect)					//Timer 0 Compare Interrupt startet hier
{
	TCCR0&=~3;							//Timer aus
	TCNT0=0;							//Z�hlwert wieder auf 0
	OCR0=100;
	if(CORE == Master){					//Interrupt Comparewert wieder laden
		PORTD|=(1<<2);
		//SPI_MasterTransmit(Ebene_ein%8);
		I2C_Leds_ein(Ebene_ein%8);		//S�ulentreiber einschalten f�r n�chste Ebene
		Ebene_ein++;
		PORTD&=~(1<<2);
	}			
	TCCR0|=3;							//Timer ein
}

void Interrupt_init(void)
{ 
//############################################
/*// Extern Interrupt
	if(CORE == Slave){
		GICR |= (1<<INT0);				//INT0 enable
		MCUCR=(1<<ISC00);				//INT0 rising edge
		MCUCR=(1<<ISC01);				
	}	*/
//############################################
//TIMER 0 einstellungen alle 5ms Interrupt
//############################################
	TIMSK=(1<<OCIE0);					//Erlaubt Compare Interupt A
	TCCR0=(1<<FOC0);					//Schaltet den Compare Modus ein 
	TCCR0|=3;							//Prescaler 64
	OCR0=1000; 							//Comparewert-Initialisierung auf ca. 5 ms bis zum ersten Interupt
	TCNT0=0;							//Timerwert auf 0 initialisieren
//ENDE TIMER0
}



