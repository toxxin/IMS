/**
 * 	\file
 *
 *	\brief SMS API
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
 *  Created on: 15.06.2011
 *      Author: Anton Glukhov
 */


#ifndef SMS_H_
#define SMS_H_


#define SMS_MESSAGE_SIZE		140

#define SMS_SIM_NUMBER			15

#define SMS_CMD_DELIMITER		','


typedef enum _sms_type_t
{
	SMS_TYPE_REC_UNREAD		= 0,
	SMS_TYPE_REC_READ		= 1,
	SMS_TYPE_STO_UNSENT		= 2,
	SMS_TYPE_STO_SENT		= 3,
	SMS_TYPE_ALL			= 4,
}_sms_type_t;


/** \struct _sms_t
 *  \brief This structure define SMS message \
 *  \var _sms_t::index
 *  Member 'index' define message index.
 *  \var _sms_t::oa
 *  Member 'oa' defines origin number - sender.
 *  \var _sms_t::da
 *  Member 'da' defines destination address - receiver.
 *  \var _sms_t::date
 *  Member 'date' define receiving date of message.
 *  \var _sms_t::data
 *  Member 'date' is a pointer to the buffer of data.
 *  \var _sms_t::sz
 *  Member 'sz' define the size of message.
 */
typedef struct _sms_t
{
	uint8_t index;
	uint8_t oa[12];		/* sender */
	uint8_t da[12];		/* destination address */
	time_t date;		/* unix time */
	uint8_t *data;
	size_t sz;
} _sms_t;


/**
 * 	\brief	Change SMS mode(PDU or TEXT)
 *
 * 	This function change current mode
 *
 * 	\param mode		- new SMS mode
 *
 * 	\retval ErrorNumber
 */
int setMessageFormat(uint8_t mode);


/**
 * 	\brief	Change showing params for sms in text mode
 *
 * 	This function change current mode for displaying params.
 *
* 	\param cmd		- ENABLE/DISABLE
 *
 * 	\retval ErrorNumber
 */
int showTextModeParams(FunctionalState cmd);


/**
 * 	\brief	SMS parser
 *
 * 	This function takes sms message and handle user commands.
 *
 * 	\param ErrNumber		- error number
 */
uint8_t sms_parser(char *message);


/**
 * 	\brief	Delete spaces
 *
 * 	This function takes sms message and delete all spaces between commands
 * 	and values.
 *
 * 	\retval void
 */
//static void del_space(char *message, uint8_t length);


/**
 * 	\brief	Checking new SMS
 *
 * 	This function return ERR_NEW_SMS if new message was received. ERR_NO_ERROR
 * 	if there is no new message.
 *
 * 	\retval ErrorNumber
 */
uint8_t checkSMS(void);


uint8_t readSMSMessage(void);


/**
 * 	\brief	Send SMS
 *
 * 	This function send SMS message and return error number.
 *
 * 	\retval ErrorNumber
 */
int sendSMSMessage(const char *, const char *);


/**
 * 	\brief	Delete all SMS messages
 *
 * 	This function delete all SMS messages and return error number.
 *
 * 	\retval ErrorNumber
 */
int deleteAllSMSMessage(void);


#endif /* SMS_H_ */
