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

#include <avr/pgmspace.h>

#include "include/pdu_converter.h"


//! Flash constant
const unsigned char PROGMEM hex_lookup[]	= hex_nmbr;              //!< Lookup table -> int 2 hex
const unsigned char PROGMEM mask[7]			= {1,3,7,15,31,63,127};  //!< Lookup table -> mask
const unsigned char PROGMEM power[7]		= {128,64,32,16,8,4,2};  //!< Lookup table -> 2^( 7 - i )

/* for IAR
const unsigned char __flash hex_lookup[]	= hex_nmbr;              //!< Lookup table -> int 2 hex
const unsigned char __flash mask[7]			= {1,3,7,15,31,63,127};  //!< Lookup table -> mask
const unsigned char __flash power[7]		= {128,64,32,16,8,4,2};  //!< Lookup table -> 2^( 7 - i )
*/


/*! \brief pcu_compress will take a user defined text code it into the PDU format
 *
 *  \param    *in           Pointer to user defined text
 *  \param    *discarded    Number of discarded chars, used later to calculate message length
 *  \param    ret[]         return array for the encoded message
 *
 *  \retval   i             Length of read string
 *
 */
int pdu_compress( unsigned char *in, unsigned char *discarded, unsigned char ret[] )
{

    //! Local variables
    int i,ii,iii;
    unsigned char encode_c, now_c, next_c;

    //Initialization
    *discarded = 0;

    for( i = ii = iii = 0; ( in[i] != '\0' ) && ( i < MESSAGE_LENGTH ); )   //Run through whole string
    {
        now_c = in[ i++ ];                                                  //This char
        next_c = in[ i ];                                                   //Next potentially '\0'

        //Last char?
        if( next_c == '\0' )
        {
            next_c = 0;
        }

        encode_c = ( now_c >> ii )+ ( mask[ ii ] & next_c )*power[ ii ];
        ret[ iii++ ] = hex_lookup[ ( encode_c >> 4 ) & 0x0F ];               //Insert first hex part
        ret[ iii++ ] = hex_lookup[ ( encode_c & 0x0F ) ];                    //Insert last hex part

        if( ii == 6 )                                                        //We have read a chunk of 7 chars, the next one will be discarded
        {
            ii = 0;
            i++;
            (*discarded)++;
        }

        else
        {
            ii++;
        }
    }

    ret[iii] = '\0';

    return i;
}


/*! \brief pdu_decompress will decode a message in PDU format into a readable string
 *
 *  \param    *compressed PDU encoded text
 *  \param    *decompressed Pointer to return array
 *
 *  \retval   iii Number of decoded chars
 */
int pdu_decompress( unsigned char *compressed, unsigned char *decompressed )
{

    //! Local variables
    int i,ii,iii;                                                           //String index
    unsigned char rest_c, ans_c ,dec_c , this_c, next_c;                    //Read and temorary variables

    for( i = ii = iii = rest_c = 0; (this_c = compressed[i++]) != '\0'; )   //Run through complete string
    {
        //Read:
        next_c = compressed[i++];                                           //Read from in buffer in AVR_SMS_com.c

        //Convert:
        dec_c = 16 * pdu_htoi( this_c ) + pdu_htoi( next_c );               //Decimal value of the two chars
        ans_c = dec_c & mask[6 - ii];                                       //Mask out the correct bits
        ans_c <<= ii;                                                       //Left shift proper correct of times
        decompressed[iii++] = ans_c + rest_c;                               //Store

        rest_c = (dec_c & ~mask[6 - ii]) >> ( 7 - ii );                     //Saving up for next time

        if( ii == 6)                                                        //Do carry
        {
            ii = 0;
            decompressed[ iii++ ] = rest_c;
            rest_c = 0;
        }

        else
        {
            ii++;
        }

    }

    decompressed[ iii ] = '\0';                                             //Terminate string in a proper manner
    return iii;                                                             //Return length
}


/*! \brief Converting ascii char string to int
 *
 *
 *  \note     Will return value as int.
 *
 *  \param    *a    ASCII string to be converted.
 *  \param    n     Return Integer.
 *
 *  \retval  int integer value
 */
int pdu_atoi( unsigned char *a )
{

    int i, n;                                                 //Help variables

    n = 0;                                                    //Init

    for( i=0; ( a[ i ] >= '0' ) && ( a[ i ] <= '9' ); ++i )   //Running through string converting from ascii to integer
    {
        n = 10*n + ( a[ i ] - '0' );                          //Adding value to return Integer
    }

    return n;
}


/*! \brief Convert hex to int
 *
 *  This function will take a hex in char format and return int value
 *
 *  \param    hex     Hex value to convert
 *
 *  \retval   int     Integer representation of hex-value
 */
int pdu_htoi( unsigned char hex )
{

    if( ( hex >= 'A' ) && ( hex <= 'F' ) )    //Test if hex is A-->F?
    {
        return hex - 'A' + 10;
    }
    else                                      //Must have something else then:0-->9
    {
        return hex - '0';
    }
}


/*! \brief Converting int [0->255] to hex value
 *
 *
 *  \note     Will return hex value as string.
 *
 *  \param    n     Integer value to be decoded.
 *  \param    ret   Return array.
 *
 */
void pdu_itoh( int n, unsigned char *ret )
{

    ret[ 0 ] = hex_lookup[ ( n >> 4 ) & 0x0F ];
    ret[ 1 ] = hex_lookup[ ( n & 0x0F ) ];
    ret[ 2 ] = '\0';
}
