/*
 * sms_parser.h
 *
 *  Created on: 21.09.2013
 *      Author: Anton
 */

#ifndef SMS_PARSER_H_
#define SMS_PARSER_H_


uint8_t cleanBuf(uint8_t *buf, size_t size, uint8_t chars);


int checkSMSPass(const uint8_t *message, const size_t size);


static int checkTrustNumber(uint8_t *number);


int splitCommand(uint8_t *buf, const size_t size, const char dlm,  uint8_t *array[]);


/**
 *  \brief Converter to upper string
 *
 *  This function all characters into capital.
 *
 *  \param - str input string
 */
void convertToUpper(uint8_t *str);


/**
 *  \brief Find command in the array of cmd_list
 *
 *  This function return pointer to _cmd_t or NULL.
 *
 *  \param - cmd command name
 *  \retval - _cmd_t
 */
_cmd_t *getCmdBySmsName(uint8_t *cmd);


/**
 *  \brief Parse recieved sms message
 *
 *  This function takes sms structure, parse it and push to task queue.
 *
 * 	\param queue - linked list of tasks
 * 	\param sms - pointer to _sms_t structure
 * 	\param inception_id - inception id
 * 	\param tmap - pointer to task map
 * 	\retval ErrorNumber
 */
int parseSmsCmd(TaskQueue *queue, _sms_t *sms, uint8_t inception_id, TMapList *tmap);

#endif /* SMS_PARSER_H_ */
