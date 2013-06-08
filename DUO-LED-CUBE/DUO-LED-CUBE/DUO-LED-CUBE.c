/*
 * DUO_LED_CUBE.c
 *
 * Created: 08.06.2013 14:32:29
 *  Author: Manuel
 */ 


#define Master 0
#define Slave 1
#include "main.h"
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
char CORE =Master; // Master or Slave Core
//char CORE =Slave; // Master or Slave Core

int main (void)
{
	//Muster Laden
	for(int i=0;i<8;i++)
		for(int j=0;j<8;j++)
			for(int k=0;k<8;k++)
				LED[i][j][k]=0;
	if(CORE==Master){
		DDRA=0xff;				//PORTA als Ausgang
	}else{
		DDRA=0x00;				//PORTA als Eingang
	}
	TWI_init();				//TWI Bus Initialisieren
	TWI_59116_reset();
	TWI_59116_setup();
	Interrupt_init();
	sei();
	set_cube(0x00);
	
	while(1)
	{
		
				
		effect_blinky2(); //ok
		effect_planboing(AXIS_X,20); //ok
		effect_planboing(AXIS_Y,20); //ok
		effect_planboing(AXIS_Z,20); //ok
		for (uint8_t ii=0;ii<8;ii++)
			effect_box_shrink_grow (ii%4, ii & 4, 20); //ok
		sendvoxels_rand_z(100,10,20); // ok
		effect_random_sparkle (3,20,10); //ok
		effect_box_woopwoop(40,1); //naja
		effect_rain(300); //ok
		effect_wormsqueeze (2, AXIS_Z, 1, 100, 25); //ok
		for(int i=0;i<8;i++){			//	Pixel durchtesten
			for(int j=0;j<64;j++)
			{	LED[i][j/8][j%8]=0xff;
				_delay_us(500);
			}
			for(int j=0;j<64;j++)
			{	LED[i][j/8][j%8]=0;
				_delay_us(500);
			}
		}
/*
		effect_box_woopwoop(40,0);
		effect_telcstairs(0,50,0xff); //naja
		effect_stringfly2("RT-CUSTOMZ");
		set_cube(0xff);
		for(int h=255;h>20;h--)			//PWM Test
			for(int i=0;i<8;i++)
				for(int j=0;j<8;j++)
					for(int k=0;k<8;k++)
					{	cli();
						LED[i][j][k]=h;
						_delay_us(10);	
						sei();
					}
		for(int h=20;h<255;h++)
			for(int i=0;i<8;i++)
				for(int j=0;j<8;j++)
					for(int k=0;k<8;k++)
					{	cli();
						LED[i][j][k]=h;
						_delay_us(10);	
						sei();
					}
		clear_cube(0x00);
		//effect_rain(300);
		effect_telcstairs(0,50,0xff);
		//effect_wormsqueeze (2, AXIS_Z, -1, 100, 50);
		sendvoxels_rand_z(100,15,20);
		
		
		effect_box_woopwoop(40,0);
		for (uint8_t ii=0;ii<8;ii++)
			effect_box_shrink_grow (ii%4, ii & 4, 20);
		
		setplane(AXIS_X,0);
		effect_planboing(AXIS_X,20);
		effect_planboing(AXIS_Y,20);
		effect_planboing(AXIS_Z,20);
		_delay_ms(50);
		for(int i=0;i<7;i++)
		{	
			shift(AXIS_X,1);
			_delay_ms(50);
		}
		for(int i=0;i<7;i++)
		{	
			shift(AXIS_X,-1);
			_delay_ms(50);
		}
		


		set_cube(0);
		for(int i=0;i<8;i++)			//	Pixel durchtesten
			for(int j=0;j<64;j++)
			{	LED[i][j/8][j%8]=0xff;
				_delay_us(500);
			}
*/

	}


return 0;
}

ISR(TIMER0_COMP_vect)				//Timer 0 Compare Interrupt startet hier
{
	TCCR0&=~3;						//Timer aus
	TCNT0=0;						//Zählwert wieder auf 0
	OCR0=100;						//Interrupt Comparewert wieder laden
	I2C_Leds_ein(Ebene_ein%8);		//Säulentreiber einschalten für nächste Ebene
	Ebene_ein++;					//nächste Ebene bearbeiten
	TCCR0|=3;						//Timer ein
}

void Interrupt_init(void)
{ 
//############################################
//TIMER 0 einstellungen alle 5ms Interrupt
//############################################
	TIMSK=(1<<OCIE0);				//Erlaubt Compare Interupt A
	TCCR0=(1<<FOC0);				//Schaltet den Compare Modus ein 
	TCCR0|=3;						//Prescaler 64
	OCR0=98; 						//Comparewert-Initialisierung auf ca. 5 ms bis zum ersten Interupt
	TCNT0=0;						//Timerwert auf 0 initialisieren
//ENDE TIMER0
}



