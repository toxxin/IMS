/**
 * 	\file
 *	
 *	\brief Parsing and printing errors
 *
 *	\page License
 *
 * THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR IMPLIED
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
 *  Created on: 24.06.2011
 *      Author: Anton Glukhov
 */

#if (ARCH == AVR)
//	#include <avr/io.h>
#endif

#include <stdio.h>
#include "include/ims.h"

#include "errno.h"

_errordesc errordesc[] = {
    { E_OK, "No error" },
    { ERR_UNKNOWN, "Unknown error" },
    { ERR_INIT_FAIL, "Initialization fail" },
    { ERR_LENGTH, "Length problem" },
    { ERR_UNK_SIGN, "Unknown sign" },
    { ERR_UNK_PACKAGE, "Unknown package" },
    { ERR_ASCII_TO_GSM_CONVERT, "Can't convert ASCII to GSM" },
    { ERR_MEM_AVAILABLE, "Not enough memory" },
    { ERR_TIME_OUT, "Time out" },
    { ERR_INCORRECT_VALUE, "Incorrect value" },
    { ERR_ARRAY_IS_EMPTY, "Array is empty" },
    { ERR_EMPTY_STRING, "Empty string" },
    { ERR_BUF_FULL, "Buffer is full" },
    { ERR_ARRAY_IS_EMPTY, "Array is empty" },
};


_eblinkdesc eblinkdesc[] = {
		{ EB_NO_ERR, 			0 },	// 0b00000000
		{ EB_PWR, 				1 },	// 0b00000001
		{ EB_EEPROM, 			2 },	// 0b00000010
		{ EB_BAD_FIRMWARE,  	3 },	// 0b00000011
		{ EB_CURRENT_MEASURE, 	4 },	// 0b00000100
		{ EB_GSM_MOD, 			5 },	// 0b00000101
		{ EB_GPS_MOD, 			6 },	// 0b00000110
		{ EB_ACCEL, 			7 },	// 0b00000111
		{ EB_NO_SIM, 			8 },	// 0b00001000
		{ EB_PWR, 				9 },	// 0b00001001
		{ EB_UNKNOWN,			10 },	// 0b00001010
		{ EB_RES1, 				11 },	// 0b00001011
		{ EB_RES2, 				12 },	// 0b00001100
		{ EB_RES3, 				13 },	// 0b00001101
		{ EB_RES4, 				14 },	// 0b00001110
		{ EB_RES5, 				15 },	// 0b00001111
};


_errorgsmdesc errgsmdesc[] = {
    { EGSM_GSM_NOT_ALLOW, "Operation not allowed" },
    { EGSM_INVALID_OPTION, "Invalid option" },
    { EGSM_INVALID_OPTION_VAL, "Invalid option value" },
    { EGSM_NOT_ENOUGH_MEM, "Not enough memory" },
    { EGSM_NOT_ALLOW_FOR_TCPIP, "Operation not allowed in the current TCP/IP stack state" },
    { EGSM_DEV_ALREADY_OPEN, "Device already open" },
    { EGSM_NETINTERF_NOT_AVAIL, "Network interface not available" },
    { EGSM_OPERATION_NOT_ALLOWED, "Operation not allowed on the considered bearer" },
    { EGSM_BCF_LINE_BUSY, "Bearer connection failure : line busy" },
    { EGSM_BCF_NO_ANSWER, "Bearer connection failure : no answer" },
    { EGSM_BCF_NO_CARRIER, "Bearer connection failure : no carrier" },
    { EGSM_BCF_NO_SIM, "Bearer connection failure : no SIM card present" },
    { EGSM_BCF_SIM_NOT_READY, "Bearer connection failure : SIM not ready" },
    { EGSM_BCF_GPRS_FAILURE, "Bearer connection failure : GPRS network failure" },
    { EGSM_BCF_PPP_LCP_NEG, "Bearer connection failure : PPP LCP negotiation failed" },
    { EGSM_BCF_PPP_AUTH, "Bearer connection failure : PPP authentication failed" },
};


/**
 * 	\brief	Return error message by error number.
 *
 * 	This function return error message by error number.
 *
 * 	\retval
 */
char *getErrorMessageByNum(const error_t num)
{
	uint8_t i = 0;

	for (i = 0; i < abs(ERR_LAST_ERROR); ++i)
	{
		if (errordesc[i].code == num)
			return errordesc[i].message;
	}

	for (i = 0; i < abs(ERR_LAST_ERROR); ++i)
	{
		if (errordesc[i].code == ERR_UNKNOWN)
			return errordesc[i].message;
	}

	return errordesc[0].message;
}


/**
 * 	\brief	Error blinking by board led.
 *
 * 	This function generate blink code by fatal error number.
 *
 * 	\retval
 */
uint8_t getCodeblinkByErrorCode(_eblink_t code)
{
	uint8_t i;

	code &= BLINKCODE_MASK;

	for (i = 0; i < (2^BLINK_NUMBERS); ++i)
	{
		if (code == eblinkdesc[i].code)
			return eblinkdesc[i].bcode;
	}

	for (i = 0; i < (2^BLINK_NUMBERS); ++i)
	{
		if (eblinkdesc[i].code == EB_UNKNOWN)
			return eblinkdesc[i].bcode;
	}

	return 0;
}


/**
 * 	\brief	Setup blinking error settings.
 *
 * 	This function setup error blink code by timer settings & error code.
 *
 * 	\retval
 */
//TODO:: change blink code via eblink_mask
//Hardware link to timer, 2nd arg - error code
void setBlinkErrorTim(uint8_t bcode)
{
	// setup tim using SHORT_BLINK_MS & LONG_BLINK_MS
	// dead loop ??
	//
}


/**
 * 	\brief	Cleanup blinking error settings.
 *
 * 	This function cleanup and reset blink error state & timer settings.
 *
 * 	\retval
 */
//Hardware link to timer, 2nd arg - error code
void clearBlinkErrorTim()
{
	// clear blink error state
	// stop & clear tim config
}



void ErrHandler(uint8_t ErrNumber)
{
#if defined(DEBUG) && defined(PRINTF)
	printf_P("error: ");
#endif

	switch (ErrNumber)
	{
	case ERR_TIME_OUT:
#if defined(DEBUG) && defined(PRINTF)
		printf_P("tim");
#endif
		break;

	case ERR_LENGTH:
#if defined(DEBUG) && defined(PRINTF)
		printf_P("len");
#endif
		break;

	case ERR_MEM_AVAILABLE:
#if defined(DEBUG) && defined(PRINTF)
		printf_P("mem");
#endif
		break;

	case E_OK:
#if defined(DEBUG) && defined(PRINTF)
		printf_P("OK");
#endif
		break;

	default:
#if defined(DEBUG) && defined(PRINTF)
		printf_P("unk");
#endif
		break;
	}

#if defined(DEBUG) && defined(PRINTF)
	printf_P("\r");
#endif
	//TODO:: Error handler and code blinking
}


//void ErrCodeBlink(uint8_t ErrNumber)
//{
//	switch(ErrNumber)
//	{
////	case ERR_NO_SIM:	ErrBlink(1, 1, 0);
////	case Unregistered:	ErrBlink(1, 1, 1);
//	}
//}
