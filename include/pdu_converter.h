/*! \file ********************************************************************
*
* Atmel Corporation
*
* - File              : AVR_api.c
* - Compiler          : IAR EWAAVR 4.11a
*
* - Support mail      : avr@atmel.com
*
* - Supported devices : All devices with a UART/USART can be used.
*                       The example is written for ATmega128
*
* - AppNote           : AVR323 - Interfacing GSM modems
*
* - Description       : Example of how to use AT-Commands to control a GSM modem
*
* $Revision: 1.1 $
* $Date: Tuesday, November 08, 2005 12:26:20 UTC $
*****************************************************************************/


#ifndef PDU_CONVERTER_H_
#define PDU_CONVERTER_H_

#define MESSAGE_ENCODED_SIZE	70	//!< Size of SMS payload
#define MESSAGE_LENGTH			80	//!< Max allowed text in SMS
#define hex_nmbr	"0123456789ABCDEF"	//!< Flash string container, look up table


int pdu_compress( unsigned char *in, unsigned char *discarded, unsigned char ret[] );
int pdu_decompress( unsigned char *compressed, unsigned char *decompressed );

int pdu_atoi( unsigned char *a );
int pdu_htoi( unsigned char hex );
void pdu_itoh( int n, unsigned char *ret );

#endif /* PDU_CONVERTER_H_ */
