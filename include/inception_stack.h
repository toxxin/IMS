/*
 * inception_stack.h
 *
 *  Created on: 25.11.2013
 *      Author: Anton
 */

#ifndef INCEPTION_STACK_H_
#define INCEPTION_STACK_H_

/** \struct InceptionSMS
 *  \brief This structure define SMS inception \
 *  \var InceptionSMS::id
 *  Member 'id' define inception index.
 *  \var InceptionSMS::oa
 *  Member 'oa' defines origin number - sender.
 *  \var InceptionSMS::date
 *  Member 'date' defines timestamp of receiving sms.
 */
typedef struct InceptionSMS
{
	uint8_t id;
	uint8_t oa[12];
	time_t	date;
} InceptionSMS;


typedef struct InceptionSMSNode
{
//	uint8_t id;
//	uint8_t oa[12];
//	time_t	date;

	InceptionSMS *inception;

	struct InceptionSMSNode *next;
} InceptionSMSNode;


typedef struct InceptionSMSStack
{
	InceptionSMSNode* head;
	InceptionSMSNode* tail;

	int (*push) (struct InceptionSMSStack*, uint8_t*, time_t);

	int (*pop) (struct InceptionSMSStack*, InceptionSMS *inception);

	InceptionSMSNode* (*getNodeById) (struct InceptionSMSStack*, uint8_t id);

	void (*printdbg) (struct InceptionSMSStack*);

	void (*clean) (struct InceptionSMSStack*);

	int size;
} InceptionSMSStack;


InceptionSMSNode* getInceptionSmsById(InceptionSMSStack* stack, uint8_t id);
int pushInception(InceptionSMSStack* stack, uint8_t *oa, time_t date);
int popInception(InceptionSMSStack* stack, InceptionSMS *inception);
void printdbgInception(InceptionSMSStack* stack);
void cleanInception(InceptionSMSStack* stack);

InceptionSMSStack createInceptionSMSStack(void);

#endif /* INCEPTION_STACK_H_ */
