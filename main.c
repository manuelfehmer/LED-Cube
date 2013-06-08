//#define DEBUG 1
#include "main.h"
#include "uart.h"
#include "font.h"
#include "59116.h"
#include "twi.h"
#include "cube.h"
#include "wii_nunchuck.h"
#include "snake.h"






//Funktionen ###########################################################
void Interrupt_init(void);
ISR (TIMER0_COMP_vect);				

//Variablen
#ifdef DEBUG
unsigned char read[32];
char kommentar[32][8]={"MODE1  \0","MODE2  \0","PWM 0  \0","PWM 1  \0","PWM 2  \0","PWM 3  \0","PWM 4  \0","PWM 5  \0","PWM 6  \0","PWM 7  \0","PWM 8  \0","PWM 9  \0","PWM 10 \0","PWM 11 \0","PWM 12 \0","PWM 13 \0","PWM 14 \0","PWM 15 \0","GRPPWM \0","GRPFREQ\0","LEDOUT0\0","LEDOUT1\0","LEDOUT2\0","LEDOUT3\0","SUBADR1\0","SUBADR2\0","SUBADR3\0","ALCALL \0","IREF   \0","EFLAG1 \0","EFLAG2 \0","dedem  \0"};
#endif



int main (void)
{
	//Muster Laden
	for(int i=0;i<8;i++)
		for(int j=0;j<8;j++)
			for(int k=0;k<8;k++)
				LED[i][j][k]=255;
	
	DDRA=0xff;				//PORTA als Ausgang
	uart_init();			//usart Initialisieren 
	TWI_init();				//TWI Bus Initialisieren
	uart_send_string("-----Power Up----------\n");
	TWI_59116_reset();
	TWI_59116_setup();
	Interrupt_init();
	sei();
	clear_cube(0xff);
	
	while(1)
	{
		effect_box_woopwoop(40,0);

		effect_stringfly2("SEBBO RT-CUSTOMZ");
		clear_cube(0xff);
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
		clear_cube(0xff);
		effect_rain(200);
		effect_telcstairs(0,50,0xff);
		effect_wormsqueeze (2, AXIS_Z, -1, 100, 50);
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
		


		clear_cube(0xff);
		for(int i=0;i<8;i++)			//	Pixel durchtesten
			for(int j=0;j<64;j++)
			{	LED[i][j/8][j%8]=0;
				_delay_us(500);
			}


	}


return 0;
}

ISR(TIMER0_COMP_vect)				//Timer 0 Compare Interrupt startet hier
{
	TCCR0&=~3;						//Timer aus
	TCNT0=0;						//Zählwert wieder auf 0
	OCR0=200;						//Interrupt Comparewert wieder laden
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
	TCCR0=(1<<FOC0); //Schaltet den Compare Modus ein ???????????????????? F0C0 löst doch eigentlich direkt aus 
	TCCR0|=3;						//Prescaler 64
	OCR0=98; 						//Comparewert-Initialisierung auf ca. 5 ms bis zum ersten Interupt
	TCNT0=0;						//Timerwert auf 0 initialisieren
//ENDE TIMER0
}



