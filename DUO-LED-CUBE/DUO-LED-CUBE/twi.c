#include "TWI.h"
#ifndef F_CPU
#define F_CPU 20000000UL
#endif

void TWI_init()
{
	//Adressports setzen (Chip-Selekt)
    DDRC  |= 0b00011111; //PC2..PC4 auf Ausgang=1, Rest auf Eingang 
    PORTC |= 0b00011100; //PC2..PC4 auf HIGH

 	TWSR = 0;                         // kein Prescaler also 1
  	TWBR = ((F_CPU/SCL_CLOCK)-16)/2;  // soll größer 10 bei 500kHz = 12

	TWCR |= (1<<TWEN);  //TWI enablen
}


unsigned char twi_start(unsigned char address)	//Reserviert den Bus und spricht die Slave Adresse an
{
//*************************************************************************	
// Start Anfrage schreiben und Bestätigen lassen vom Slave
//return 0 = Slave ist in Ordnung, 1= Fehler beim Verbindungsaufbau
//*************************************************************************
 	TWCR = (1<<TWINT) | (1<<TWSTA) | (1<<TWEN);	// START einleiten
	while(!(TWCR & (1<<TWINT)));				//warten bis erfolgreich übernommen
	if (((TWSR&0xF8)!=TW_START) && ((TWSR&0xF8) != TW_REP_START)) //Abrechen wenn start nicht gesetzt werden konnte
		return 1; 	
	TWDR = address;								//Adresse laden
	TWCR = (1<<TWINT) | (1<<TWEN);				//Adresse schreiben
	while(!(TWCR & (1<<TWINT)));				//warten bis Slave das Packet angenommen hat
	if ( ((TWSR&0xF8) != TW_MT_SLA_ACK) && ((TWSR&0xF8) != TW_MR_SLA_ACK) ) //Fehler aufgetreten Start abbrechen
		return 1; 

		return 0;								//alles OK Start hat funktioniert

}


void twi_start_wait(unsigned char address) //Reserviert den Bus sobald er frei ist und spricht die Slave Adresse an
{
    while ( 1 )
    {
	    
	    TWCR = (1<<TWINT) | (1<<TWSTA) | (1<<TWEN);// send START condition    	
    	while(!(TWCR & (1<<TWINT)));// wait until transmission completed
    	// check value of TWI Status Register. Mask prescaler bits.
    	if ( ((TW_STATUS & 0xF8) != TW_START) && ((TW_STATUS & 0xF8) != TW_REP_START)) 
			continue;
    	
    	TWDR = address;					// send device address
    	TWCR = (1<<TWINT) | (1<<TWEN);
    	while(!(TWCR & (1<<TWINT)));// wail until transmission completed
    
    	// check value of TWI Status Register. Mask prescaler bits.
    	if ( ((TW_STATUS & 0xF8) == TW_MT_SLA_NACK )||((TW_STATUS & 0xF8) ==TW_MR_DATA_NACK) ) 
    	{    	    
	        TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWSTO);  // device busy, send stop condition to terminate write operation 
	        while(TWCR & (1<<TWSTO));// wait until stop condition is executed and bus released
    	    continue;
    	}
    	//if( (TW_STATUS & 0xF8) != TW_MT_SLA_ACK) return 1;
    	break;
     }

}

unsigned char twi_write( unsigned char data )	//Sendet ein Byte 
{	
//*************************************************************************
// Ein Byte an I2C Slave senden   
//  Eingang:  1 byte welches gesendet werden soll
//  Return:   0 schreiben war OK
//            1 schreiben war fehlerhaft
//*************************************************************************/
	TWDR = data;					//Daten laden
	TWCR = (1<<TWINT) | (1<<TWEN);	//Daten losschicken
	while(!(TWCR & (1<<TWINT)));	//warten bis alle Datenbits raus sind

	if((TW_STATUS & 0xF8)!=TW_MT_DATA_ACK) 	//Checken ob Slave "Daten erhalten" gesendet hat
		return 1;		
	return 0;
}

void twi_stop(void)				//Beendet eine Übertragung und lässt den Bus wieder frei
{
	TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWSTO);// Stop Anfrage senden
	while(TWCR & (1<<TWSTO));	//Warten bis Stop Anfrage bearbeitet und Bus wieder frei ist
}

unsigned char twi_readAck(void) //Ein Byte lesen und weitere Daten erwarten
{
	TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWEA);
	while(!(TWCR & (1<<TWINT)));    

    return TWDR;

}/* i2c_readAck */

unsigned char twi_readNak(void) //Ein Byte lesen und danach die Kommunikation beenden 
{
	TWCR = (1<<TWINT) | (1<<TWEN);
	while(!(TWCR & (1<<TWINT)));
	
    return TWDR;

}/* i2c_readNak */