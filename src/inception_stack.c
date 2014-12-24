/*
 * inception_stack.c
 *
 *  Created on: 25.11.2013
 *      Author: Anton
 */

#include <stdlib.h>
#include <string.h>
#include "ims.h"

InceptionSMSStack createInceptionSMSStack(void)
{
	InceptionSMSStack stack;
	stack.size = 0;
	stack.head = NULL;
	stack.tail = NULL;
	stack.push = &pushInception;
	stack.pop = &popInception;
	stack.printdbg = &printdbgInception;
	stack.getNodeById = &getInceptionSmsById;
	stack.clean = &cleanInception;
	return stack;
}


static int NewId(InceptionSMSStack* stack, uint8_t max)
{
	InceptionSMSNode *node = stack->head;
	uint8_t i, k, flag;

	if (stack->size == 0)
		return 0;

	for (i = 0; i < max; ++i)
	{
		flag = 0;
		node = stack->head;

		for (k = 0; k < stack->size; ++k)
		{
			if (node->inception->id == i)
				flag = 1;
			node = (InceptionSMSNode *)node->next;
		}

		if (flag == 0)
			return i;
	}

	return ERR_NO_FREE_INCEPTION_SLOT;
}


InceptionSMSNode* getInceptionSmsById(InceptionSMSStack* stack, uint8_t id)
{
	InceptionSMSNode *node = stack->head;

	if ((stack->size == 0) || (stack->head == NULL))
		return NULL;

	while (node != NULL)
	{
		if (node->inception->id == id)
			return node;
		else
			node = node->next;
	}

	return NULL;
}


int pushInception(InceptionSMSStack* stack, uint8_t *oa, time_t date)
{
	int ret = E_OK;

	_DEBUG("Pushing inception...", NULL);

	// Create a new node
	InceptionSMSNode* n = (InceptionSMSNode*) malloc (sizeof(InceptionSMSNode));
	n->inception = (InceptionSMS*) malloc (sizeof(InceptionSMS));

	ret = NewId(stack, SMS_SIM_NUMBER);
	if (ret < 0)
	{
		_DEBUG("Can't get new inception id. error - %d", ret);
		goto clnbuf;
	}

	strncpy(n->inception->oa, oa, PHONE_LEN);

	n->inception->date = date;

	n->inception->id = ret;

	n->next = NULL;

	// Define next link, head and tail
	if (stack->head == NULL)
	{ // no head
		stack->head = n;
	}
	else
	{
		stack->tail->next = n;
	}
	stack->tail = n;
	stack->size++;

	return n->inception->id;

clnbuf:
	free(n->inception);
	free(n);

	return ret;
}


int popInception(InceptionSMSStack* stack, InceptionSMS *inception)
{
	_DEBUG("Popping task...", NULL);

	if ((stack->size == 0) || (stack->head == NULL))
	{
		_DEBUG("Stack is empty...", NULL);
		return ERR_SMS_INCEPTION_STACK_EMPTY;
	}

	InceptionSMSNode* head = stack->head;

	inception->id = head->inception->id;
	strncpy((char *)inception->oa, (char *)head->inception->oa, PHONE_LEN);
	inception->date = head->inception->date;

	stack->head = (InceptionSMSNode *)head->next;
	stack->size--;

	free(head->inception);
	free(head);

	return E_OK;
}


void cleanInception(InceptionSMSStack* stack)
{
	_DEBUG("Clean inceptions...", NULL);

	while(stack->size > 0)
	{
		InceptionSMS *inception;
		stack->pop(stack, inception);
	}
}


void printdbgInception(InceptionSMSStack* stack)
{
	_DEBUG("Inception stack content:", NULL);

	if (stack->size == 0)
		_DEBUG("No item in stack.", NULL);
	else
	{ // has item(s)
		InceptionSMSNode* head = stack->head;
		int i, size = stack->size;
		_DEBUG("%d item(s):", stack->size);
		for (i = 0; i < size; i++)
		{
			_DEBUG("id: %d", head->inception->id);
			_DEBUG("   oa: %s", head->inception->oa);
			_DEBUG("   date: %u", head->inception->date);

			head = (InceptionSMSNode *)head->next;
		}
	}
}
