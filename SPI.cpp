/*
 * SPI.cpp
 *
 * Created: 09/08/2022 02:45:02
 *  Author: Rodrigo
 */ 






#include "SPI.h"

#include "../SERIAL/SERIAL.h"



static  unsigned char SLAVEDADOTX[32],SLAVEDADORX[32],SLAVEDADOIDRX =0X00,SLAVEDADOIDTX =0X00, SLAVERECE =0X00,SLAVEENVI =0X00;

static void (*SLAVERECEBE)(unsigned char*, unsigned char);


//================================================================================
void Slave_RxTx_spi( void (*function)(unsigned char*, unsigned char) )
{
	SLAVERECE =1;
	SLAVEDADOIDRX =0X00;
	SLAVEENVI =0X00;
	SLAVERECEBE = function;	
}
//================================================================================
void Slave_Envia_spi(const unsigned char *dado, unsigned char qtd)
{
	SLAVEENVI =1;
	SLAVERECE =0;
	SPDR = dado[0];
	for (int i=0;i<qtd;i++)
	{
		SLAVEDADOTX[i] =dado[i];
	}
	
	SLAVEDADOIDTX =1;
}

//================================================================================

void Master_Habilitar_spi(BITORDEM ORD, MODO MODE,CLOCK CLK)
{
	if (CLK > 0X03)
	{
		SPSR |= (1 << SPI2X);
	}
	SPCR = (1 << SPE)|(ORD)|(1 << MSTR)|(MODE)|(CLK);
	DDRB |= SCK| MOSI | SS;
	PORTB |= MISO;

}
//================================================================================
unsigned char Master_envia_spi(unsigned char *dado, unsigned char qtd)
{
				
	for (int i= 0; i<qtd;i++)
	{
		SPDR =dado[i];
		while(!(SPSR &(1<<SPIF)) );
	}
	
	return 0;
}
//================================================================================

unsigned char Master_Recebe_spi(unsigned char *dado, unsigned char qtd)
{
	
	
	for (int i= 0; i<qtd;i++)
	{
		SPDR = 0xFF;
		while(!(SPSR &(1<<SPIF)) );
		dado[i] = SPDR;
		
	}
	
	
	return 0;
}
//================================================================================

void Slave_Habilitar_spi(BITORDEM ORD, MODO MODE)
{
	DDRB |= MISO;
	PORTB |= SCK|MOSI|SS;
	SPCR =(1<<SPE)|(1<<SPIE)| (ORD) | (MODE) ;
	
	PCICR |=(1<<PCIE0);
	PCMSK0 |=(1<<PCINT2);
	
	sei();
}


//=================================================================================


ISR(SPI_STC_vect)
{
	
	
	SLAVEDADORX[SLAVEDADOIDRX++] =SPDR;
		
	SPDR =SLAVEDADOTX[SLAVEDADOIDTX++];
			
}




ISR(PCINT0_vect)
{
	if((PINB & SS)&&(SLAVERECE==1))
	{
		PORTC ^=(1<<5);
		SLAVERECEBE(SLAVEDADORX,SLAVEDADOIDRX);
		SLAVEDADOIDRX =0X00;
	}
	
	if ((PINB & SS) && (SLAVEENVI==1))
	{
		SLAVEDADOIDTX =0x01;
		SLAVERECE =1;
		SLAVEENVI =0;
	}
	
}

