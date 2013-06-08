//RS232 Defining
#include "uart.h"


void uart_init(void)
{
	UBRRH = UBRR_VAL >> 8;					//Baudrate festlegen
  	UBRRL = UBRR_VAL & 0xFF;				//Baudrate festlegen
	UCSRB=(1<<RXEN)|(1<<TXEN|(1<<RXCIE));	//Enable receiver and transmitter Interrupt enable
	UCSRC = (1<<URSEL)|(1<<USBS)|(1<<UCSZ1)|(1<<UCSZ0);//URSEL=1: Zum Beschreiben von UCSRC nötig; USBS=1: 2 Stop Bits; UCSZ0 und UCSZ1 =1: 8 Bit Modus
}
void uart_send_char(uint8_t Data)
{
	while (!(UCSRA & (1<<UDRE)));
	UDR = Data;
} 
void uart_send_string(char *s)
{
    while (*s)/* so lange *s != '\0' also ungleich dem "String-Endezeichen" */
    {   uart_send_char(*s);
        s++;
    }
}
char uart_recv_char(void)
{
    while (!(UCSRA & (1<<RXC)));   // warten bis Zeichen verfuegbar
    return UDR;                   // Zeichen aus UDR an Aufrufer zurueckgeben
}

void uart_send_int(uint8_t Data)
{
//		uart_send_char(Data/1000+'0');	//Sendet die Tausenderstelle der zu übertragenden Zahl
//		Data-=Data/1000*1000;			//Zieht die Tausenderstelle ab
		uart_send_char(Data/100+'0');	//Sendet die Hunderterstelle
		Data-=Data/100*100;				//Zieht die Hunderterstelle ab
		uart_send_char(Data/10+'0');	//Sendet die Zehnerstelle
		Data-=Data/10*10;				//Zieht die Zehnerstelle ab
		uart_send_char(Data/1+'0');		//Sendet die Einerstelle
}
void uart_send_int2(uint16_t Data)
{
		uart_send_char(Data/1000+'0');	//Sendet die Tausenderstelle der zu übertragenden Zahl
		Data-=Data/1000*1000;			//Zieht die Tausenderstelle ab
		uart_send_char(',');
		uart_send_char(Data/100+'0');	//Sendet die Hunderterstelle
		Data-=Data/100*100;				//Zieht die Hunderterstelle ab
		uart_send_char(Data/10+'0');	//Sendet die Zehnerstelle
		Data-=Data/10*10;				//Zieht die Zehnerstelle ab
		uart_send_char(Data/1+'0');		//Sendet die Einerstelle
}
void uart_send_data(uint8_t *cData,uint8_t len)	//uart_send_data(Data,sizeof(Data));
{
  while(len-- > 0)
  {
    uart_send_char(*cData);  // shift out data
    cData++;
  }

}
