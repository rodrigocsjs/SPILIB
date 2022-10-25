/*
 * SPI.h
 *
 * Created: 09/08/2022 02:44:47
 *  Author: Rodrigo
 */ 


#ifndef SPI_H_
#define SPI_H_

#include <avr/io.h>
#include <avr/interrupt.h>


#define  F_CPU 16000000UL
#include <util/delay.h>

#define  SCK (1<<5)
#define  MISO (1<<4)
#define  MOSI (1<<3)
#define  SS (1<<2)

enum MODO
{
	  MODE0 = 0X00, //CPOL 0 CPHA 0
	  MODE1 = 0X08, //CPOL 1 CPHA 0
	  MODE2 = 0X0C, //CPOL 1 CPHA 1
	  MODE3 = 0X04 //CPOL 0 CPHA 1
};


enum CLOCK
{
	  CLOCK0 = 0X00, // Fosc/4   SPR1 0 SPR0 0
	  CLOCK1 = 0X01, // Fosc/16  SPR1 0 SPR0 1
	  CLOCK2 = 0X02, // Fosc/64  SPR1 1 SPR0 0
	  CLOCK3 = 0X03, // Fosc/128 SPR1 1 SPR0 1

	  CLOCK4 = 0X04, // Fosc/2 SPI2X 1  SPR1 0 SPR0 0
	  CLOCK5 = 0X05, // Fosc/8 SPI2X 1  SPR1 0 SPR0 1
	  CLOCK6 = 0X06, // Fosc/32 SPI2X 1 SPR1 1 SPR0 0
	  CLOCK7 = 0X07 // Fosc/64 SPI2X 1 SPR1 1 SPR0 1
};



enum BITORDEM
{
  LSB = 0X20,  //LSB PRIMEIRO DORD 1
  MSB  =0X00   //MSB PRIMEIRO DORD 0
};

void Master_Habilitar_spi(BITORDEM ORD, MODO MODE,CLOCK CLK);
unsigned char Master_envia_spi(unsigned char *dado, unsigned char qtd);
unsigned char Master_Recebe_spi(unsigned char *dado, unsigned char qtd);


void Slave_Habilitar_spi(BITORDEM ORD, MODO MODE);

void Slave_RxTx_spi( void (*function)(unsigned char*, unsigned char) );
void Slave_Envia_spi(const unsigned char *dado, unsigned char qtd);

#endif /* SPI_H_ */