
#define DD_SS 4
#define DD_MOSI 5
#define DD_MISO 6
#define DD_SCK 7

// aus CAN
#define MX_SPI_B
#define MX_CAN_PORT PORTB
#define MX_CAN_DDR DDRB
#define MX_CAN_PIN PINB

#define MX_AVR
#define MX_EE
#define MX_EE_SIZE 1024
#define MX_SPI
#define MX_SPI_B
#define MX_SPI_SDI 6
#define MX_SPI_SDO 5
#define MX_SPI_SCK 7


//common defines  125kBit/s  auf 16 oder 20 MHz
#define CAN_CHIPSEL  	4
#define CAN_INTERRUPT  	3
#define CAN_INT_MASK  	8
#define CNF1_VAL  		7
#define CNF2_VAL  		209
#define CNF3_VAL  		3
#define BFPCTRL_VAL    	12
#define TXRTSCTRL_VAL	0

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