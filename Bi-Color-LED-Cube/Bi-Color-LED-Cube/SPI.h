
#define DD_SS 4
#define DD_MOSI 5
#define DD_MISO 6
#define DD_SCK 7

void SPI_MasterInit();
void SPI_SlaveInit();
void SPI_MasterTransmit(char cData);
char SPI_SlaveReceive(void);

void SPI_MasterInit()
{ 	
	DDRB|=(1<<DD_SS);	
	/* Set MOSI and SCK output, all others input */
	DDRB = (1<<DD_MOSI)|(1<<DD_SCK);
	/* Enable SPI, Master, set clock rate fck/16 */
	SPCR = (1<<SPE)|(1<<MSTR)|(1<<SPR0);
	/* Enable SPI, Master, set clock rate fck/4 */
	//SPCR = (1<<SPE)|(1<<MSTR);
}

void SPI_SlaveInit()
{
	/* Set MISO output, all others input */
	DDRB = (1<<DD_MISO);
	/* Enable SPI */
	SPCR = (1<<SPE);
}

void SPI_MasterTransmit(char cData)
{
	/* Start transmission */
	PORTB&=~(1<<DD_SS);
	SPDR = cData;
	/* Wait for transmission complete */
	while(!(SPSR & (1<<SPIF)));
	PORTB|=(1<<DD_SS);
}

char SPI_SlaveReceive(void)
{
	/* Wait for reception complete */
	while(!(SPSR & (1<<SPIF)));
	/* Return data register */
	return SPDR;
}