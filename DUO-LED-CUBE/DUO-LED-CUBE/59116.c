#include "59116.h"

void TWI_59116_setup(void)
{
	for(int i=0;i<4;i++)
	{
		twi_start_wait(TLC59116|(i<<1));		// set device address and write mode
		twi_write(0x80); 	//Mode 1 Register
		twi_write(129);	//Mode 2 Register	
		twi_write(0x00);	//Mode 2 Register	

		for(uint8_t i=0;i<=15;i++)					//LED 0-15 beschreiben
		{	twi_write(0x00);	//PWM Register
		}
	  	twi_write(0xFF); //GRPPWM reg.
	    twi_write(0);	 //GRPFREQ reg.
	    twi_write(0xAA); //ledout0 reg.
	    twi_write(0xAA); //ledout1 reg.
	    twi_write(0xAA); //ledout2 reg.
	    twi_write(0xAA); //ledout3 reg.
	    twi_write(0xD2); //SUBADR1 reg.
	    twi_write(0xD4); //SUBADR2 reg.
	    twi_write(0xD8); //SUBADR3 reg.
	    twi_write(0xD0); //ALLCALLADR reg.
		twi_stop();                             	// set stop conditon = release bus
	}
}
void TWI_59116_reset(void)
{
	twi_start_wait(PWM_RESET+I2C_WRITE);		// set device address and write mode
	twi_write(0xA5);
	twi_write(0x5A);
	twi_stop();     
}

void I2C_Leds_ein(unsigned char Ebene)
{
	if(CORE == Master)
	{
	
		twi_start_wait(PWMDRIVER1+I2C_WRITE);
		twi_write(0x82);					
		for(int x=0; x<=15;x++)				
		{	twi_write(LED[Ebene][x/8][x%8]);
		}
		twi_start(PWMDRIVER2+I2C_WRITE);
		twi_write(0x82);					
		for(int x=16; x<=31;x++)				
		{	twi_write(LED[Ebene][x/8][x%8]);
		}
		twi_start(PWMDRIVER3+I2C_WRITE);
		twi_write(0x82);					
		for(int x=32; x<=47;x++)				
		{	twi_write(LED[Ebene][x/8][x%8]);
		}
		twi_start(PWMDRIVER4+I2C_WRITE);
		twi_write(0x82);					
		for(int x=48; x<=63;x++)				
		{	twi_write(LED[Ebene][x/8][x%8]);
		}
		PORTA=0xff;
		twi_stop();
		PORTA&= ~(1<<Ebene);
	}else{
		//Ebene ^= PINA;
		twi_start_wait(PWMDRIVER1+I2C_WRITE);
		twi_write(0x82);					
		for(int x=0; x<=15;x++)				
		{	twi_write(LED[Ebene][x/8][x%8]);
		}
		twi_start(PWMDRIVER2+I2C_WRITE);
		twi_write(0x82);					
		for(int x=16; x<=31;x++)				
		{	twi_write(LED[Ebene][x/8][x%8]);
		}
		twi_start(PWMDRIVER3+I2C_WRITE);
		twi_write(0x82);					
		for(int x=32; x<=47;x++)				
		{	twi_write(LED[Ebene][x/8][x%8]);
		}
		twi_start(PWMDRIVER4+I2C_WRITE);
		twi_write(0x82);					
		for(int x=48; x<=63;x++)				
		{	twi_write(LED[Ebene][x/8][x%8]);
		}
		twi_stop();
		
	}
	
}
