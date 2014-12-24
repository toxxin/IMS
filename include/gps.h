/**
 * 	\file
 *	
 *	\brief GPS high level management
 *
 *	\page License
 *
 * THIS SOFTWARE IS PROVIDED "AS IS" AND ANY EXPRESSED OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
 * EXPRESSLY AND SPECIFICALLY DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH
 * DAMAGE.
 *
 *
 *  Created on: 29.08.2011
 *      Author: Anton Glukhov
 */

#ifndef GPS_H_
#define GPS_H_

#include <string.h>

/**
 * 	\brief 	Main struct for gps data
 * 	\param	longtitude_d 	- degrees of longtitude
 * 	\param	longtitude_m 	- minutes of longtitude
 * 	\param	latitude_d 		- digital of latitude
 * 	\param	latitude_m	 	- minutes of latitude
 * 	\param	ns			 	- N(north) or S(sourth) latitude
 * 	\param	ew			 	- E(east) or w(west) longtitude
 * 	\param	time_h		 	- hours of time
 * 	\param	time_m		 	- minutes of time
 * 	\param	time_s		 	- seconds of time
 * 	\param	height		 	- altitude
 * 	\param	satellites	 	- satellites number
 */
typedef struct
{
	uint16_t longitude_d;
	uint16_t latitude_d;
	uint16_t longitude_m;
	uint16_t latitude_m;
	char ns;
	char ew;
	uint8_t time_h;
	uint8_t time_m;
	uint8_t time_s;
	uint16_t height;
	uint8_t satellites;
} _str_locate_t;

extern _str_locate_t 	myGPSStruct;


#if 0
typedef struct
{
	char time[9];
	char latitude[7];
	char ns[1];
	char longtitude[8];
	char ew[1];
	char quality[1];
	char nsatellites[2];
	char dilution[3];
	char altitude[3];
	char units[1];
	char separation[3];
	char units_separation[1];
	char age[3];
	char differential[5];
	char checksum[2];
} _gga_locate_t;
#endif

#define GPS_BUF_SIZE 83

extern volatile uint8_t uart_buffer;

extern volatile uint8_t FLAG;

#define FLAG_USART	BIT(0)
#define FLAG_TIM0	BIT(1)
#define FLAG_TIM1	BIT(2)
#define FLAG_TIM2	BIT(3)
#define FLAG_TIM3	BIT(4)
#define WAIT_FLAG	BIT(5)
#define BUFOVF		BIT(6)	// Buffer overflow


/**
 * 	\brief	$GGA (NMEA) string buffer handler
 *
 * 	This function separates longitude, latitude, etc in $GGA string and fill
 * 	the global structure.
 *
 * 	\param string			- pointer to string in the format $xxGGA (nmea)
 *  \param str_dat			- pointer to the global structure
 * 	\retval ErrorNumber
 */
uint8_t ParseString(char *string, _str_locate_t *str_dat);


uint8_t WaitCharset(char chr);


/**
 * 	\brief	Receiving string from any port
 *
 * 	This function receives string, adds to the end following
 * 	characters ('\\r', '\\n' and '\\0') and put it to dst pointer.
 * 	\b Note \b that \b (p*) \b - \b function \b which \b are \b waiting \b for
 * 	\b special \b characters \b start \b of \b string.
 *
 *	\param dst				- target buffer
 * 	\param size				- size of buffer
 *  \param num				- port number
 *  \param (*p)				- pointer to a function that expects
 *  						  a special character
 * 	\retval ErrorNumber
 */
uint8_t getString(char *dst, size_t size, USART_TypeDef* num, uint8_t(*p)(char));


/**
 * 	\brief	Main function for getting current location
 *
 * 	This function calls another function such as getString, ParseString, etc.
 *
 * 	\retval ErrorNumber
 */
int getLocation(char *location);


/**
 * 	\brief	Checking GPS Module
 *
 * 	This function checks working of GPS module.
 *
 * 	\retval ErrorNumber
 */
uint8_t checkGPSModule(void);

#endif /* GPS_H_ */
