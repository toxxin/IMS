/*
 * task_result_queue.c
 *
 *  Created on: 22.10.2013
 *      Author: Anton
 */

#include <stdlib.h>
#include <string.h>
#include "ims.h"

TaskResultQueue createTaskResultQueue(void)
{
	TaskResultQueue queue;
	queue.size = 0;
	queue.head = NULL;
	queue.tail = NULL;
	queue.add = &addTaskResult;
	queue.del = &delTaskResult;
	queue.printdbg = &printdbgResult;
	queue.getNodeById = &getTaskResultById;
	queue.clean = &cleanTaskResult;
	return queue;
}


TaskResultNode* getTaskResultById(TaskResultQueue* queue, uint8_t id)
{
	TaskResultNode *node = queue->head;

	if ((queue->size == 0) || (queue->head == NULL))
		return NULL;

	while (node != NULL)
	{
		if (node->result->id == id)
			return node;
		else
			node = node->next;
	}

	return NULL;
}


int addTaskResult(TaskResultQueue *queue, TaskResult* result)
{
	int ret = E_OK;

	_DEBUG("Adding result... %s", result->name);

	// Create a new node
	TaskResultNode* n = (TaskResultNode*) malloc (sizeof(TaskResultNode));
	n->result = (TaskResult*) malloc (sizeof(TaskResult));
	n->result->name = (char*) malloc (sizeof(char) * strlen(result->name) + 1);
	n->result->result = (char*) malloc (sizeof(char) * strlen(result->result) + 1);

	n->result->id = result->id;
	n->result->code = result->code;
	strcpy(n->result->name, result->name);
	strcpy(n->result->result, result->result);

	n->next = NULL;

	// Define next link, head and tail
	if (queue->head == NULL)
	{ // no head
		queue->head = n;
		n->prev = NULL;
	}
	else
	{
		queue->tail->next = n;
		n->prev = queue->tail;
	}

	queue->tail = n;
	queue->size++;

	return ret;
}


void delTaskResult(TaskResultQueue *queue)
{
	printf("Deleting first task result...\n");

	if ((queue->size == 0) || (queue->head == NULL))
	{
		_DEBUG("Task queue is empty...", NULL);
	}
	else
	{
		// get the first item
		TaskResultNode* head = queue->head;
		// move head pointer to next node, decrease size
		queue->head = (TaskResultNode *)head->next;
		queue->head->prev = NULL;

		queue->size--;
		// free the memory of original head
		free(head->result->name);
		free(head->result->result);
		free(head->result);
		free(head);
	}
}


void cleanTaskResult(TaskResultQueue *queue)
{
	_DEBUG("Clean result queue...", NULL);

	while (queue->size > 0)
		queue->del(queue);
}


void printdbgResult(TaskResultQueue *queue)
{
	_DEBUG("Task result queue content:", NULL);

	if (queue->size == 0)
		_DEBUG("No item in queue.", NULL);
	else
	{ // has item(s)
		TaskResultNode* head = queue->head;
		int i, size = queue->size;
		_DEBUG("%d item(s):", queue->size);
		for (i = 0; i < size; i++)
		{
			_DEBUG("id: %d", head->result->id);
			_DEBUG("   name: %s", head->result->name);
			_DEBUG("   code: %u", head->result->code);
			_DEBUG("   result: %s", head->result->result);

			head = (TaskResultNode *)head->next;
		}
	}
}
