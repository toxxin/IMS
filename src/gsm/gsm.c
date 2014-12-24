/**
 * 	\file
 *	
 *	\brief GSM high level management.
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
 *  Created on: 17.06.2011
 *      Author: Anton Glukhov
 */

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "ims.h"

#define GSM_BUF_SIZE 32

volatile uint8_t gsm_module_config = 0;


_error_t checkSIMCard()
{
	return E_OK;
}


int updateGSMSignal(void)
{
	char tmpdata[20];
	char *ptr1, *ptr2;
	_egsm_t ret = ERR_UNKNOWN;

	ret = getModuleData("AT+CSQ\r", &tmpdata[0]);
	if (ret < 0)
		return ret;

	ptr1 = memmem(&tmpdata[0], 20, "+CSQ: ", 6);

	if (ptr1 != NULL)
	{
		ptr1 += 6;
		ptr2 = memchr(ptr1, ',', 20);
		if (ptr2 == NULL)
			return ERR_INCORRECT_VALUE;

		*ptr2 = '\0';

		GSMSignal.rssi = atoi(ptr1);
//		quality->rssi = atoi(ptr1);

		ptr1 = ptr2 + 1;

		if ((ptr2 = memchr(ptr1, '\0', 20)) == NULL)
		{
			if ((ptr2 = memchr(ptr1, '\r', 20)) == NULL)
				return ERR_INCORRECT_VALUE;
		}

		*ptr2 = '\0';

		GSMSignal.ber = atoi(ptr1);
//		quality->ber = atoi(ptr1);
	}
	else
	{
		ret = ERR_INCORRECT_VALUE;
	}

	return ret;
}


int getRegistrationState(_gsm_reg_t *reg)
{
	char tmpdata[20];
	int ret = ERR_UNKNOWN;
	char *ptr1, *ptr2;

	ret = getModuleData("AT+CREG?\r", &tmpdata[0]);
	if (ret < 0)
		return ret;

	ptr1 = memmem(&tmpdata[0], 20, "+CREG: ", 7);

	if (ptr1 != NULL)
	{
		ptr1 += 7;
		ptr2 = memchr(ptr1, ',', 20);
		if (ptr2 == NULL)
			return ERR_INCORRECT_VALUE;

		*ptr2 = '\0';

		reg->urcmode = atoi(ptr1);
		reg->regstatus = atoi(ptr2 + 1);
	}
	else
	{
		ret = ERR_INCORRECT_VALUE;
	}

	return ret;
}


int waitURC(uint8_t *URC, uint32_t ms)
{
	//     AT+CUSD=1,"*100#"
	int32_t cnt = 0;
	int ret = E_OK;
	uint8_t *buf;
	uint8_t attemps = 10;

	uint8_t *first, *second;	// double quotes in URC response

	buf = (uint8_t *)malloc(sizeof(uint8_t) * (350));
	if (buf == NULL)
		return ERR_MEM_AVAILABLE;

	Delay_ms(1000);

	while(attemps-- > 0)
	{
		cnt = getBuf(buf, 350, GSM_USART_PORT, 3000, 100);
		if (cnt > 0)
			break;
	}

	_DEBUG("cnt - %d", cnt);
	_DEBUG("urc - %x", *buf);
	_DEBUG("urc - %x", *(buf+1));

	if (cnt <= 0)
	{
		_DEBUG("Empty string");
		ret = ERR_EMPTY_STRING;
		goto buf_free;
	}

	if (memmem(buf, cnt, "+CUSD:", 6) == NULL)
	{
		_DEBUG("Unknown response!");
		ret = ERR_EMPTY_STRING;
		goto buf_free;
	}

	first = memmem(buf, cnt, "\"", 1);
	if (first == NULL)
	{
		_DEBUG("Can't find double quotes!");
		ret = ERR_EMPTY_STRING;
		goto buf_free;
	}

	second = memmem(first + 1, cnt - (first - buf) - 1, "\"", 1);
	if (second == NULL)
	{
		_DEBUG("Can't find double quotes!");
		ret = ERR_EMPTY_STRING;
		goto buf_free;
	}

	USART_BUF_TX(first + 1, second - first, USART_DBG_PORT);

	// TODO:: The USSD session can be aborted using command AT+CUSD=2.

//	getBufTimeout(uint8_t *buf, size_t len, USART_TypeDef* USARTx, uint32_t prems, uint32_t postms);
	_DEBUG("URC: %s", buf);

buf_free:
	free(buf);

	return ret;
}


int sendUSSD(char *USSD)
{
	int ret = E_OK;
	char buf[40];
	char cmd[30];

	_DEBUG("Sending USSD...", NULL);

	snprintf(&cmd[0], 30, "AT+CUSD=1,\"%s\"\r", USSD);
	_DEBUG("USSD cmd: %s", &cmd[0]);

	ret = getModuleData(cmd, &buf[0]);

	return ret;
}


int getUSDMode(void)
{
	int ret = E_OK;
	char buf[40];
	char cmd[30];

	_DEBUG("Getting USD mode...", NULL);
	snprintf(&cmd[0], 30, "AT+CUSD?\r");

	ret = getModuleData(cmd, &buf[0]);
	_DEBUG("ret - %d", ret);
	_DEBUG("usd-mode: %s", buf);

	return ret;
}


int setUSDMode(uint8_t mode)
{
	int ret = E_OK;
	char buf[40];
	char cmd[30];

	_DEBUG("Setting USD mode...", NULL);
	snprintf(&cmd[0], 30, "AT+CUSD=%d\r", mode);

	ret = getModuleData(cmd, &buf[0]);
	_DEBUG("ret - %d", ret);
	_DEBUG("usd-mode: %s", buf);

	return ret;
}


int getModuleData(uint8_t *cmd, uint8_t *data)
{
	int32_t cnt = 0;
	size_t ret = E_OK;
	uint8_t cmd_len = strlen((char*)cmd);
	uint8_t *pok, *perr;	/* pointer to OK part in buffer */

	/* buffer for receiving answer from GSM module */
	uint8_t *buf;
	buf = (uint8_t *)calloc(sizeof(uint8_t) * MAX_STRING_LEN, sizeof(uint8_t));
	if (buf == NULL)
		return ERR_MEM_AVAILABLE;

	USART_STR_TX(cmd, GSM_USART_PORT);

	cnt = getBuf(buf, MAX_STRING_LEN, GSM_USART_PORT, 50, 50);
	if (cnt <= 0)
	{
		ret = ERR_EMPTY_STRING;
		goto buf_free;
	}

	/* check echo command */
	if (CHKBIT_REG(gsm_module_config, CFG_ECHO_MODE))
	{
		if (NULL == memmem(buf, cnt, cmd, cmd_len))
		{
			ret = ERR_NO_ECHO;
			goto buf_free;
		}
	}

	/* check "OK" or return error number */
	if ((pok = memmem(buf, cnt, "OK", 2)) != NULL)
	{
		uint8_t *ppayload = CHKBIT_REG(gsm_module_config, CFG_ECHO_MODE) ? buf + cmd_len : buf;


		while ((*ppayload == '\r') || (*ppayload == '\n'))
			ppayload++;

		while (((*(pok - 1) == '\r') || (*(pok - 1) == '\n')) && (pok > ppayload))
			pok--;

		if (pok > ppayload)
		{
			*pok = '\0';	/* make null-terminated string */
			memcpy(data, ppayload, (pok - ppayload + 1));
			ret = (pok - ppayload + 1);
		}
		else if (pok == ppayload)
		{
			ret = 0;
			goto buf_free;
		}
		else
		{
			// unexpected case
			ret = -1;
			goto buf_free;
		}
	}
	else if ((perr = memmem(buf, cnt, "ERROR", 5)) != NULL)
	{
		uint8_t *ppayload = CHKBIT_REG(gsm_module_config, CFG_ECHO_MODE) ? buf + cmd_len : buf;

		while ((*ppayload == '\r') || (*ppayload == '\n'))
			ppayload++;

		if (NULL != memmem(buf, cnt, "ERROR", 5))
		{
			uint8_t new_size = (buf + cnt - 1) - ppayload;
			memcpy(data, buf, new_size);
			ret = ERR_GSM_ERROR;	/* add parsing errors OR MAKE COMMON MISTAKE AND PARSE THIS IN HIGH LEVEL */
			goto buf_free;
		}
		else
		{
			ret = ERR_UNKNOWN;
			goto buf_free;
		}
	}

buf_free:
	free(buf);

	return ret;
}


int getModuleDataTimeout(uint8_t *cmd, uint8_t *data)
{
	int32_t char_count = 0;
	int ret = E_OK;
	uint8_t cmd_len = strlen(cmd);
	uint8_t *pok, *perr;	/* pointer to OK part in buffer */

	/* buffer for receiving answer from GSM module */
	uint8_t *buf;
	buf = (uint8_t *)calloc(sizeof(uint8_t) * MAX_STRING_LEN, sizeof(uint8_t));
	if (buf == NULL)
		return ERR_MEM_AVAILABLE;

	USART_STR_TX(cmd, GSM_USART_PORT);

	if (memcmp(cmd, "AT+WIPBR=4,6,0", 14) == 0)
		char_count = getBuf(buf, MAX_STRING_LEN, GSM_USART_PORT, 20000, 3000);
	else if (memcmp(cmd, "AT+WIPDATA=1,1,1", 16) == 0)
		char_count = getBuf(buf, MAX_STRING_LEN, GSM_USART_PORT, 20000, 3000);
	else
		char_count = getBuf(buf, MAX_STRING_LEN, GSM_USART_PORT);

	if (char_count <= 0)
	{
		_DEBUG("nullstring");
		ret = ERR_EMPTY_STRING;
		goto buf_free;
	}

	/* check echo command */
	if (CHKBIT_REG(gsm_module_config, CFG_ECHO_MODE))
	{
		if (NULL == memmem(buf, char_count, cmd, cmd_len))
		{
			_DEBUG("noecho");
			ret = ERR_NO_ECHO;
			goto buf_free;
		}
	}

	/* check "OK" or return error number */
	if ((pok = memmem(buf, char_count, "OK", 2)) != NULL)
	{
		uint8_t *ppayload = CHKBIT_REG(gsm_module_config, CFG_ECHO_MODE) ? buf + cmd_len : buf;

		while ((*ppayload == '\r') || (*ppayload == '\n'))
			ppayload++;

		while ((*(pok - 1) == '\r') || (*(pok - 1) == '\n') && (pok > ppayload))
			pok--;


		if ((pok - ppayload) > 0)
		{
			*pok = '\0';	/* make null-terminated string */
			memcpy(data, ppayload, (pok - ppayload + 1));
			ret = (pok - ppayload + 1);
		}
		else if (pok == ppayload)
		{
			ret = 0;
			goto buf_free;
		}
		else
		{
			_DEBUG("h1");
			ret = ERR_UNKNOWN;
			goto buf_free;
		}
	}
	else if (perr = memmem(buf, char_count, "ERROR", 5) != NULL)
	{
		uint8_t *ppayload = CHKBIT_REG(gsm_module_config, CFG_ECHO_MODE) ? buf + cmd_len : buf;

		while ((*ppayload == '\r') || (*ppayload == '\n'))
			ppayload++;

		if (NULL != memmem(buf, char_count, "ERROR", 5))
		{
			uint8_t new_size = (buf + char_count - 1) - ppayload;
			memcpy(data, buf, new_size);
			ret = ERR_GSM_ERROR;	/* add parsing errors OR MAKE COMMON MISTAKE AND PARSE THIS IN HIGH LEVEL */
			goto buf_free;
		}
		else
		{
			ret = ERR_UNKNOWN;
			goto buf_free;
		}
	}

buf_free:
	free(buf);

	return ret;
}



int setModuleData(uint8_t *cmd)
{
	uint8_t char_count = 0;
	int ret = E_OK;

	/* buffer for receiving answer from GSM module */
	uint8_t *buf = malloc(sizeof(uint8_t) * MAX_STRING_LEN);
	if (buf == NULL)
		return ERR_MEM_AVAILABLE;

	USART_STR_TX(cmd, GSM_USART_PORT);

	char_count = getBuf(buf, MAX_STRING_LEN, GSM_USART_PORT);
	if (char_count < 0)
	{
		ret = ERR_EMPTY_STRING;
		goto buf_free;
	}

	/* allocation memory for separated strings */
	uint8_t **array = (uint8_t **)malloc(sizeof(uint8_t*) * MAX_SUBSTRING_LEN);
	if (array == NULL)
	{
		ret = ERR_MEM_AVAILABLE;
		goto buf_free;
	}

	splitString(buf, char_count, array);

	if (!memcmp(array[1], "OK", 2))
		ret = E_OK;
	else
		ret = -10;	//TODO:: parse error number if there is

	if (ret != E_OK)
		_DEBUG("Some err - %d", ret);

split_free:
	free (array);
buf_free:
	free (buf);

	return ret;
}


int getManufacturerId(uint8_t *data)
{
	return getModuleData((uint8_t *)"AT+CGMI\r", data);
}


int getModelId(uint8_t *data)
{
	return getModuleData((uint8_t *)"AT+CGMM\r", data);
}


int getRevision(uint8_t *data)
{
	return getModuleData((uint8_t *)"AT+CGMR\r", data);
}


int getSerialNumber(uint8_t *data)
{
	return getModuleData((uint8_t *)"AT+CGSN\r", data);
}


int getIMSI(uint8_t *data)
{
	return getModuleData((uint8_t *)"AT+CIMI\r", data);
}


int getCharacterSet(uint8_t *data)
{
	return getModuleData((uint8_t *)"AT+CSCS?\r", data);
}


uint8_t countChar(const uint8_t *buf, size_t len, uint8_t c)
{
	uint16_t i = 0;
	uint8_t cnt = 0;

	for (i = 0; i < len; ++i)
	{
		if (*buf == c)
			cnt++;
	}

	return cnt;
}


int checkGSMModule(void)
{
	int ret = E_OK;
	char data[30];

	_DEBUG("Check GSM module...");
	ret = getModuleData((uint8_t *)"AT\r", &data[0]);
	if ((memcmp(&data[0], "AT\0", 3) != 0) && (ret != 3))
		ret = -1;

	return ret;
}


void checkGSMNetwork(void)
{
	uint8_t i;
	_gsm_reg_t reg;

	for (i = 0; i < CHECKING_GSM_NETWORK; ++i)
	{
		updateGSMSignal();
		getRegistrationState(&reg);

		if ((GSMSignal.rssi > 1) && (GSMSignal.rssi !=  99) && (reg.regstatus == 1))
			break;

		_DEBUG("Waiting network registration...", NULL);

		//TODO::add progressing
		Delay_ms(2000);

		if (i == (CHECKING_GSM_NETWORK - 1))
		{
			_DEBUG("check GSM dead loop", NULL);
			while(1) {}
		}

	}

}


void gsmOn(void)
{
#ifndef DISCOVERY
	_DEBUG("Turn GSM on...");

	GPIO_HIGH(GSM_DRV_PORT, GSM_DRV_PIN);

	GPIO_LOW(GSM_RST_PORT, GSM_RST_PIN);
	GPIO_HIGH(MCU_GSM_PORT, MCU_GSM_PIN);
	Delay_ms(300);
	GPIO_LOW(GSM_ON_PORT, GSM_ON_PIN);
	Delay_ms(100);
	GPIO_HIGH(GSM_RST_PORT, GSM_RST_PIN);
	Delay_ms(700);
	GPIO_HIGH(GSM_ON_PORT, GSM_ON_PIN);
//  GPIO_HIGH(USART_GSM_DTR_PORT, USART_GSM_DTR_PIN);
//	while(GPIO_ReadInputDataBit(GSM_WISMO_PORT, GSM_WISMO_PIN) == 0);
#endif
}


void gsmOff(void)
{
#ifndef DISCOVERY
	_DEBUG("Turn GSM off...");
	GPIO_HIGH(GSM_DRV_PORT, GSM_DRV_PIN);
	GPIO_HIGH(MCU_GSM_PORT, MCU_GSM_PIN);
#endif
}


void gsmReset(void)
{
	_DEBUG("Reset GSM...");

	USART_STR_TX("AT&F\r", GSM_USART_PORT);
}





#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <limits.h>
#include <sys/types.h>

#include "coap.h"

static coap_tid_t id;

#include <assert.h>

#define htons(a)            ((((a)>>8)&0xff)|(((a)<<8)&0xff00))
#define ntohs(a)            htons(a)


coap_pdu_t *
coap_new_request(uint8_t message, uint8_t method, coap_list_t *options) {
  coap_pdu_t *pdu;
  coap_list_t *opt;

  if ( ! ( pdu = coap_new_pdu() ) )
    return NULL;

  pdu->hdr->type = message;
  pdu->hdr->id = htons(id++);
  pdu->hdr->code = method;

//  pdu->hdr->token_length = the_token.length;
//  if ( !coap_add_token(pdu, the_token.length, the_token.s)) {
//    debug("cannot add token to request\n");
//  }

  for (opt = options; opt; opt = opt->next) {
    coap_add_option(pdu, COAP_OPTION_KEY(*(coap_option *)opt->data),
		    COAP_OPTION_LENGTH(*(coap_option *)opt->data),
		    COAP_OPTION_DATA(*(coap_option *)opt->data));
  }

//  if (payload.length) {
//    if ((flags & FLAGS_BLOCK) == 0)
//      coap_add_data(pdu, payload.length, payload.s);
//    else
//      coap_add_block(pdu, payload.length, payload.s, block.num, block.szx);
//  }

  return pdu;
}


int coap_test(void)
{
	coap_pdu_t  *pdu;
	coap_pdu_t  *pdu_rcv;
	coap_context_t *ctx;
	coap_address_t dst;
	int id;
	coap_tid_t tid;

	uint32_t res;

	_DEBUG("\r\nCOAP start test.");

	coap_set_log_level(LOG_WARNING);

	id = rand() & INT_MAX;

	coap_tick_t now;

	dst.addr.addr = 1;
	strcpy(dst.ip, "109.173.124.250");
	dst.port = COAP_DEFAULT_PORT;
	dst.size = sizeof(dst);

	int ret = E_OK;
	char tmpdata[100];
	Bearer bearer;
	int cnt = 0;


	/* Internet connection */
	ret = getModuleData("AT+WIPCFG=1\r", &tmpdata[0]);
	if (ret < 0)
	{
		//TODO:: Add error handler
		_DEBUG("Error stack init");
		goto e_stack_clean;
	}

	ret = BearerDefInit(&bearer);
	if (ret < 0)
	{
		//TODO:: Add error handler
		_DEBUG("Error bearer init");
		goto e_bearer_close;
	}
	Delay_ms(500);

	ret = bearer.start(&bearer);
	if (ret < 0)
	{
		//TODO:: Add error handler
		_DEBUG("Error bearer start");
		goto e_bearer_close;
	}
	Delay_ms(500);
	Delay_ms(5000);


	ctx = coap_new_context(&dst);
	if (!ctx) {
		coap_log(LOG_EMERG, "Can't init contex!");
		goto e_bearer_stop;
	}




cl:
	pdu = coap_new_request(COAP_MESSAGE_CON, COAP_REQUEST_GET, NULL);
	if (!pdu)
		_DEBUG("pdu error");

	coap_add_option(pdu, COAP_OPTION_URI_PATH, 2, "ti");
	tid = coap_send(ctx, &dst, pdu);

	cnt = getBuf(&tmpdata[0], 100, GSM_USART_PORT, 2000, GSM_LAST_CHAR_TIMEOUT_MS);

	cnt = unescapeBuf(&tmpdata[0], cnt);

	pdu_rcv = coap_new_pdu();
	if (!coap_pdu_parse(&tmpdata[0], cnt, pdu_rcv))
	{
		_DEBUG("wrong data");
	}

	unsigned char *data;
	size_t len;

	if (coap_get_data(pdu_rcv, &len, &data))
	{
		_DEBUG("Length: %d", len);
		_DEBUG( "Correct data: %c", *data);
		_DEBUG("Correct data: %c", *(data + 1));
	}

	_DEBUG("\r\n");
	coap_show_pdu(pdu_rcv);

	coap_delete_pdu(pdu);
	coap_delete_pdu(pdu_rcv);

//	goto cl;



	pdu = coap_new_request(COAP_MESSAGE_CON, COAP_REQUEST_GET, NULL);
	if (!pdu)
		_DEBUG("pdu error");

	coap_add_option(pdu, COAP_OPTION_URI_PATH, 2, "ti");
//	coap_add_data(pdu, strlen("v=2,b=12&t=15&m=1"), "v=2,b=12&t=15&m=1");
	tid = coap_send(ctx, &dst, pdu);

	cnt = getBuf(&tmpdata[0], 100, GSM_USART_PORT, 2000, GSM_LAST_CHAR_TIMEOUT_MS);

	cnt = unescapeBuf(&tmpdata[0], cnt);

	pdu_rcv = coap_new_pdu();
	if (coap_pdu_parse(&tmpdata[0], cnt, pdu_rcv) <= 0)
	{
		_DEBUG("wrong data");
	}

	coap_show_pdu(pdu_rcv);

	coap_delete_pdu(pdu);
	coap_delete_pdu(pdu_rcv);




	coap_free_context(ctx);


e_bearer_stop:
	bearer.stop(&bearer);
e_bearer_close:
	bearer.close(&bearer);

e_stack_clean:
	/* cleaning up ip stack */
	getModuleData("AT+WIPCFG=0\r", &tmpdata[0]);

}


#include <pb_encode.h>
#include <pb_decode.h>
#include "ims.pb.h"


typedef enum
{
	BASE,
	FULL
} PackDetails;

Device_p fillDevice_p(PackDetails details)
{
	Device_p dev;

	strcpy(dev.sn, Device.sn);
	dev.hw_version = Device.hw_version;
	dev.sw_version_maj = Device.sw_version_maj;
	dev.sw_version_min = Device.sw_version_min;
	dev.time = Now();
	dev.uptime = 10;
	dev.holdtime = 70;
	dev.battery = 20;
	dev.time_interval = 1;
	dev.accel_state = 1;

	return dev;
}


GSMModule_p fillGSMModule_p(PackDetails details)
{
	GSMModule_p gm;

	gm.imsi = GSMModule.imsi;
	gm.sn = GSMModule.sn;

	strcpy(gm.manufacturer_id, "SIERRA");
	gm.has_manufacturer_id = 1;
	strcpy(gm.manufacturer_id, GSMModule.manufacturer_id);

	if (details == FULL)
	{
		gm.has_model_id = true;
		strcpy(gm.model_id, GSMModule.model_id);

		gm.has_manufacturer_id = true;
		strcpy(gm.manufacturer_id, GSMModule.manufacturer_id);

		gm.has_revision = true;
		strcpy(gm.revision, GSMModule.revision);
	}
	else
	{
		gm.has_model_id = false;
		gm.has_manufacturer_id = false;
		gm.has_revision = false;
	}

	return gm;
}


GSMSignal_p fillGSMSignal_p(PackDetails details)
{
	GSMSignal_p gs;

	gs.rssi = GSMSignal.rssi;

	if (details == FULL)
	{
		gs.has_ber = true;
		gs.ber = GSMSignal.ber;
	}

	return gs;
}


bool printfile_callback(pb_istream_t *stream, const pb_field_t *field, void **arg)
{
	RequestedPack_p pack = {};

    if (!pb_decode(stream, RequestedPack_p_fields, &pack))
        return false;

    switch (pack.pack)
	{
    	case RequestedPack_p_Pack_GSMMODULE:
    		_DEBUG("GSMMODULE request");
    		break;
    	case RequestedPack_p_Pack_GSMSIGNAL:
    		_DEBUG("GSMSIGNAL request");
    		break;
    	case RequestedPack_p_Pack_GSMCELL:
    		_DEBUG("GSMCELL request");
    		break;
    	case RequestedPack_p_Pack_GSMLOG:
    		_DEBUG("GSMLOG request");
    		break;
    	case RequestedPack_p_Pack_ACCEL:
    		_DEBUG("ACCEL request");
    		break;
    	case RequestedPack_p_Pack_DEVICE:
    		_DEBUG("DEVICE request");
    		break;
    	case RequestedPack_p_Pack_NETWORK:
    		_DEBUG("NETWORK request");
    		break;
    	default:
    		_DEBUG("Unknown request");
    		break;
	}

//    if (pack.pack == RequestedPack_p_Detail_BASE);
//    _DEBUG("pack.pack - %d", pack.pack);
//    _DEBUG("pack - %s", pack.uri);

    return true;
}


void hw_debug(coap_context_t *ctx, coap_address_t *dst)
{
	coap_pdu_t  *pdu, *pdu_rcv;
	coap_tid_t tid;
	uint8_t buf[150];
	int ret;
	uint8_t i;
	HWTestMessage_p hw_m;

	_DEBUG("Hardware debug...");

	pdu = coap_new_request(COAP_MESSAGE_CON, COAP_REQUEST_POST, NULL);
	if (!pdu)
	{
		_DEBUG("Can't create pdu!");
		return;
	}

	coap_add_option(pdu, COAP_OPTION_URI_PATH, strlen("hw_test"), (unsigned char *)"hw_test");

	pb_ostream_t ostream = pb_ostream_from_buffer(&buf[0], sizeof(buf));

	hw_m.gsm_module = fillGSMModule_p(FULL);

	hw_m.gsm_signal = fillGSMSignal_p(FULL);

	hw_m.device = fillDevice_p(FULL);

	if (!pb_encode(&ostream, HWTestMessage_p_fields, &hw_m))
	{
		_DEBUG("Protobuf: Can't encode message!");
		goto e_clear_pdus;
	}

	coap_add_data(pdu, ostream.bytes_written, &buf[0]);

	pdu_rcv = coap_new_pdu();

	for (i = 0; i < COAP_DEFAULT_MAX_RETRANSMIT; ++i)
	{
		tid = coap_send(ctx, dst, pdu);

		memset(&buf[0], 0, sizeof(buf));

		ret = getBuf(&buf[0], sizeof(buf), GSM_USART_PORT, COAP_DEFAULT_RESPONSE_TIMEOUT * 1000, GSM_LAST_CHAR_TIMEOUT_MS);
		if (ret > 0)
		{
			ret = unescapeBuf(&buf[0], ret);

			if (coap_pdu_parse(&buf[0], ret, pdu_rcv) <= 0)
				_DEBUG("Can't parse answer. Wrong data.");

			if ((pdu_rcv->hdr->id == tid) && (pdu_rcv->hdr->code == COAP_RESPONSE_201))
			{
				_DEBUG("Success.");
				goto e_clear_pdus;
			}
		}
	}

	_DEBUG("Failed.");

e_clear_pdus:
	coap_delete_pdu(pdu);
	coap_delete_pdu(pdu_rcv);
}


void sync(coap_context_t *ctx, coap_address_t *dst)
{
	coap_pdu_t  *pdu, *pdu_rcv;
	coap_tid_t tid;
	uint8_t buf[150];
	int ret;
	uint8_t i;
	Device_p device;

	_DEBUG("Synchronization...");

	pdu = coap_new_request(COAP_MESSAGE_CON, COAP_REQUEST_POST, NULL);
	if (!pdu)
	{
		_DEBUG("Can't create pdu!");
		return;
	}

	coap_add_option(pdu, COAP_OPTION_URI_PATH, strlen("sync"), (unsigned char *)"sync");

	pb_ostream_t ostream = pb_ostream_from_buffer(&buf[0], sizeof(buf));

	device = fillDevice_p(BASE);

	if (!pb_encode(&ostream, Device_p_fields, &device))
	{
		_DEBUG("Protobuf: Can't encode message!");
		goto e_clear_pdus;
	}

	coap_add_data(pdu, ostream.bytes_written, &buf[0]);

	pdu_rcv = coap_new_pdu();

	for (i = 0; i < COAP_DEFAULT_MAX_RETRANSMIT; ++i)
	{
		tid = coap_send(ctx, dst, pdu);

		memset(&buf[0], 0, sizeof(buf));

		ret = getBuf(&buf[0], sizeof(buf), GSM_USART_PORT, COAP_DEFAULT_RESPONSE_TIMEOUT * 1000, GSM_LAST_CHAR_TIMEOUT_MS);
		if (ret > 0)
		{
			ret = unescapeBuf(&buf[0], ret);

			if (coap_pdu_parse(&buf[0], ret, pdu_rcv) <= 0)
				_DEBUG("Can't parse answer. Wrong data.");

			if ((pdu_rcv->hdr->id == tid) && (pdu_rcv->hdr->code == COAP_RESPONSE_201))
			{
				_DEBUG("Success.");
				goto e_clear_pdus;
			}
		}

		if (i == 3)
		{
			_DEBUG("Failed.");
			goto e_clear_pdus;
		}
	}

	//TODO:: Add tasks in queue

e_clear_pdus:
	coap_delete_pdu(pdu);
	coap_delete_pdu(pdu_rcv);
}


void gsm_betta(void)
{
	coap_context_t *ctx;
	coap_address_t dst;

	Bearer bearer;
	int ret = E_OK;
	char buf[150];

	_gsm_quality_t signal;

	_DEBUG("\r\nGSM beta.");

//	coap_tick_t now;

//	if (getGSMSignal(&signal))
//	{
//		hw_m.gsm_signal.ber = signal.rssi;
//		hw_m.gsm_signal.rssi = signal.ber;
//	}

//	hw_m.gsm_signal.ber = 15;
//	hw_m.gsm_signal.rssi = 99;

	dst.addr.addr = 1;
	strcpy(dst.ip, "109.173.124.250");
	dst.port = COAP_DEFAULT_PORT;
	dst.size = sizeof(dst);

	/* Internet connection */
	ret = getModuleData("AT+WIPCFG=1\r", &buf[0]);
	if (ret < 0)
	{
		//TODO:: Add error handler
		_DEBUG("Error stack init!");
		goto e_stack_clean;
	}

	ret = BearerDefInit(&bearer);
	if (ret < 0)
	{
		//TODO:: Add error handler
		_DEBUG("Error bearer init!");
		goto e_bearer_close;
	}
	Delay_ms(500);

	Delay_ms(5000);


	ret = bearer.start(&bearer);
	if (ret < 0)
	{
		//TODO:: Add error handler
		_DEBUG("Error bearer start!");
		goto e_bearer_close;
	}
//	Delay_ms(6000);


	/* COAP initialization */
	ctx = coap_new_context(&dst);
	if (!ctx)
	{
		coap_log(LOG_EMERG, "Can't init contex!");
		goto e_bearer_stop;
	}

	Delay_ms(3000);

#define HW_DBG 1
#ifdef HW_DBG
	hw_debug(ctx, &dst);
#endif

	sync(ctx, &dst);

e_coap_ctx:
	_DEBUG("Clear contex...");
	coap_free_context(ctx);

e_bearer_stop:
	bearer.stop(&bearer);

e_bearer_close:
	bearer.close(&bearer);

e_stack_clean:
	/* cleaning up ip stack */
	getModuleData("AT+WIPCFG=0\r", &buf[0]);
}
