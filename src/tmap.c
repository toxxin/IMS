/*
 * tmap.c
 *
 *  Created on: 04.12.2013
 *      Author: anton
 */

#include <stdlib.h>
#include <string.h>
#include "ims.h"


TMapList createTMapList(void)
{
	TMapList list;
	list.size = 0;
	list.head = NULL;
	list.tail = NULL;
	list.add = &addNode;
	list.del = &delNode;
	list.delByIncId = &delNodeByIncId;
	list.printdbg = &printdbgTMap;
	list.getTMapById = &getTMapById;
	list.getTaskIdByIncId = &getTaskIdByIncId;
	list.isEmpty = &isTMapListEmpty;
	list.clean = &cleanTMapList;
	return list;
}


int isTMapListEmpty(TMapList *list)
{
	return ((list->size == 0) || (list->head == NULL)) ? 1 : 0;
}


static int NewId(TMapList *list, uint8_t max)
{
	TMapNode *node = list->head;
	uint8_t i, k, flag;

	if (list->size == 0)
		return 0;

	for (i = 0; i < max; ++i)
	{
		flag = 0;
		node = list->head;

		for (k = 0; k < list->size; ++k)
		{
			if (node->node->id == i)
			{
				flag = 1;
				break;
			}

			node = (TMapNode *)node->next;
		}

		if (flag == 0)
			return i;
	}

	return ERR_NO_FREE_TASK_SLOT;
}


TMap *getTMapById(TMapList *list, uint8_t id)
{
	TMapNode *node = list->head;

	if ((list->size == 0) || (list->head == NULL))
		return NULL;

	while (node != NULL)
	{
		if (node->node->id == id)
			return node->node;
		else
			node = node->next;
	}

	return NULL;
}


int getTaskIdByIncId(TMapList *list, const uint8_t inception_id, uint8_t *id)
{
	int ret = E_OK;
	uint8_t tid_count = 0;

	if ((list->size == 0) || (list->head == NULL))
	{
		ret = 0;
	}
	else
	{
		TMapNode *node = list->head;

		while (node != NULL)
		{
			if (node->node->iid == inception_id)
			{
				*id = node->node->tid;
				id++;
				tid_count++;
			}

			node = (TMapNode*)node->next;
		}

		ret = tid_count;
	}

	return ret;
}


int addNode(TMapList *list, uint8_t iid, uint8_t tid)
{
	int ret = E_OK;

	_DEBUG("Adding tmap node...", NULL);

	// Create a new node
	TMapNode* n = (TMapNode*) malloc (sizeof(TMapNode));
	n->node = (TMap*) malloc (sizeof(TMap));

	ret = NewId(list, MAX_TASK_ID_POOL);
	if (ret < 0)
	{
		_DEBUG("Can't get new tmapid. error - %d", ret);
		goto clnbuf;
	}

	n->node->id = ret;

	n->node->iid = iid;
	n->node->tid = tid;

	n->next = NULL;

	// Define next link, head and tail
	if (list->head == NULL)
	{ // no head
		list->head = n;
		n->prev = NULL;
	}
	else
	{
		list->tail->next = n;
		n->prev = list->tail;
	}

	list->tail = n;
	list->size++;

	return ret;

clnall:
	free(n->node);
clnbuf:
	free(n);

	return ret;
}


void delNode(TMapList *list)
{
	printf("Deleting first tmap node...\n");

	if ((list->size == 0) || (list->head == NULL))
	{
		_DEBUG("Tmap list is empty...", NULL);
	}
	else
	{
		TMapNode* head = list->head;
		// move head pointer to next node, decrease size
		list->head = (TMapNode *)head->next;

		list->size--;
		// free the memory of original head
		free(head->node);
		free(head);
	}
}


void delNodeByIncId(TMapList *list, uint8_t inception_id)
{
	_DEBUG("Deleting all tmap nodes with inception id = %d.", inception_id);

	if (!(list->isEmpty(list)))
	{
		TMapNode *node = list->head;

		while (node != NULL)
		{
			if (node->node->iid == inception_id)
			{
				if (node == list->head)
				{
					list->head = node->next;
				}
				else if (node == list->tail)
				{
					list->tail = node->prev;
				}
				else
				{
					node->next->prev = node->prev;
					node->prev->next = node->next;
				}

				node = (TMapNode*)node->next;

				free(node->node);
				free(node);

				list->size--;
			}
		}
	}
}


void cleanTMapList(TMapList *list)
{
	_DEBUG("Cleaning tmap list...", NULL);

	while (list->size > 0)
		list->del(list);
}


void printdbgTMap(TMapList *list)
{
	_DEBUG("TMap list content:", NULL);

	if (list->size == 0)
		_DEBUG("No item in list.", NULL);
	else
	{ // has item(s)
		TMapNode* node = list->head;
		int i, size = list->size;
		_DEBUG("%d item(s):", list->size);
		_DEBUG("	id	iid  tid ", NULL);
		for (i = 0; i < size; i++)
		{
//			_DEBUG("id: %d", head->id);
//			_DEBUG("   iid: %d", head->node->iid);
//			_DEBUG("   tid: %d", head->node->tid);
			_DEBUG("	%u	%u    %u  ", node->node->id, node->node->iid, node->node->tid);

			node = (TMapNode*)node->next;
		}
	}
}
