/*
 * task_queue.c
 *
 *  Created on: 24.09.2013
 *      Author: Anton
 */

#include <stdlib.h>
#include <string.h>
#include "ims.h"

TaskQueue createTaskQueue(void)
{
	TaskQueue queue;
	queue.size = 0;
	queue.head = NULL;
	queue.tail = NULL;
	queue.add = &addTask;
	queue.del = &delTask;
	queue.run = &runTask;
	queue.printdbg = &printdbgTask;
	queue.getNodeById = &getTaskById;
	return queue;
}


static int NewId(TaskQueue* queue, uint8_t max)
{
	TaskNode *node = queue->head;
	uint8_t i, k, flag;

	if (queue->size == 0)
		return 0;

	for (i = 0; i < max; ++i)
	{
		flag = 0;
		node = queue->head;

		for (k = 0; k < queue->size; ++k)
		{
			if (node->id == i)
			{
				flag = 1;
				break;
			}
			node = (TaskNode *)node->next;
		}

		if (flag == 0)
			return i;
	}

	return ERR_NO_FREE_TASK_SLOT;
}


TaskNode* getTaskById(TaskQueue* queue, uint8_t id)
{
	/* add malloc definition */

	TaskNode *node = queue->head;

	if ((queue->size == 0) || (queue->head == NULL))
		return NULL;

	while (node != NULL)
	{
		if (node->id == id)
			return node;
		else
			node = node->next;
	}

	return NULL;
}


int addTask(TaskQueue *queue, char *name, uint8_t cmd_id, time_t date,
									int(*handler)(char *, char *), char *args)
{
	int ret = E_OK;

	_DEBUG("Adding task... %s", name);

	// Create a new node
	TaskNode* n = (TaskNode*) malloc (sizeof(TaskNode));
	n->name = (char*) malloc (strlen(name) + 1);

	if (args != NULL)
	{
		n->args = (char*) malloc (strlen(args) + 1);
		strcpy(n->args, args);
	}
	else
	{
		n->args = NULL;
	}

	strcpy(n->name, name);

	n->date = date;

	n->cmd_id = cmd_id;

	n->handler = handler;

	ret = NewId(queue, MAX_TASK_ID_POOL);
	if (ret < 0)
	{
		_DEBUG("Can't get new taskid. error - %d", ret);
		goto clnbuf;
	}

	n->id = ret;

	n->next = NULL;

	// Define next link, head and tail
	if (queue->head == NULL)
	{ // no head
		queue->head = n;
	}
	else
	{
		queue->tail->next = n;
	}
	queue->tail = n;
	queue->size++;

	return n->id;

clnbuf:
	free(n->name);
	free(n->args);
	free(n);

	return ret;
}


void delTask(TaskQueue *queue)
{
	printf("Deleting first task...\n");

	if ((queue->size == 0) || (queue->head == NULL))
	{
		_DEBUG("Task queue is empty...", NULL);
	}
	else
	{
		TaskNode* head = queue->head;
		// move head pointer to next node, decrease size
		queue->head = (TaskNode *)head->next;

		queue->size--;
		// free the memory of original head
		free(head->name);
		free(head->args);
		free(head);
	}
}


TaskResult *runTask(TaskQueue *queue)
{
	int ret = E_OK;
	TaskResult *result = (TaskResult*) malloc (sizeof(TaskResult));

	result->result = NULL;

	int res_size = 0;

	if (queue->size == 0)
	{
		_DEBUG("No item in queue.", NULL);

		//TODO::change code
		result->id = 0;
		result->code = 400;		// queue is empty
		result->name = NULL;
	}
	else
	{
		char res_data[50] = "";

		TaskNode* head = queue->head;

		_DEBUG("test. args - %s", head->args);
		ret = head->handler(head->args, &res_data[0]);
		result->result = (char*) malloc (strlen(res_data) + 1);
		strcpy(result->result, &res_data[0]);
		result->code = 200;

		result->name = (char*) malloc (strlen(head->name) + 1);

		result->id = head->id;
		strcpy(result->name, head->name);
	}

	return result;
}


void printdbgTask(TaskQueue *queue)
{
	_DEBUG("Task queue content:", NULL);

	if (queue->size == 0)
		_DEBUG("No item in queue.", NULL);
	else
	{ // has item(s)
		TaskNode* head = queue->head;
		int i, size = queue->size;
		_DEBUG("%d item(s):", queue->size);
		for (i = 0; i < size; i++)
		{
			_DEBUG("id: %d", head->id);
			_DEBUG("   name: %s", head->name);
			_DEBUG("   date: %lld", (long long)head->date);
			_DEBUG("   args: %s", head->args);

			head = (TaskNode *)head->next;
		}
	}
}
