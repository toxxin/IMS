/**
 * 	\file
 *	
 *	\brief XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
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
 *  Created on: 01.07.2011
 *      Author: Anton Glukhov
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "ims.h"

int setGPRSConnection(Socket *sd)
{
	int32_t cnt = 0;
	size_t ret = E_OK;
	char cmd[30];

	/* buffer for receiving answer from GSM module */
	uint8_t *buf;
	buf = (uint8_t *)malloc(sizeof(uint8_t) * MAX_STRING_LEN);
	if (buf == NULL)
		return ERR_MEM_AVAILABLE;

	snprintf(&cmd[0], 30, "AT+WIPDATA=%d,%d,1\r\0", sd->proto, sd->id);
	USART_STR_TX(&cmd[0], GSM_USART_PORT);

	cnt = getBuf(buf, MAX_STRING_LEN, GSM_USART_PORT, 3000, GSM_LAST_CHAR_TIMEOUT_MS);

	if ((memcmp(buf, "CONNECT", 7) != NULL) && (cnt > 7))
		ret = E_OK;
	else
		ret = -1;
	//TODO:: change error number

buf_free:
	free (buf);

	return ret;
}


uint16_t escapeBuf(const uint8_t *src, uint8_t *buf, size_t len)
{
	uint16_t l = len;
	uint16_t i;

	for (i = 0; i < len; ++i)
	{
		if ((*src == 0x03) || (*src == 0x10))
		{
			*(buf++) = 0x10;
			l++;
		}

		*(buf++) = *(src++);
	}

	return l;
}


uint16_t unescapeBuf(uint8_t *buf, size_t len)
{
	uint16_t i;

	for (i = 1; i < len; ++i)
	{
		if (((*buf == 0x10) && (*(buf + 1) == 0x03)) ||
				((*buf == 0x10) && (*(buf + 1) == 0x10)))
		{
			memmove(buf, buf + 1, len - i);
			len--;
		}
		buf++;
	}

	return len;
}
