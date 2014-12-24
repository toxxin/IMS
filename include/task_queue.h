/**
 * 	\file
 *
 *	\brief Queue of tasks in general
 *
 *	\page License
 *
 *  Created on: 24.09.2013
 *      Author: Anton
 */

#ifndef TASK_QUEUE_H_
#define TASK_QUEUE_H_

#include "task_result_queue.h"


#define MAX_TASK_ID_POOL	10


/** \struct TaskNode
 *  \brief This structure define node in TaskQueue
 *  \var TaskNode::id
 *  Member 'id' contains unique number for each task.
 *  \var TaskNode::name
 *  Member 'name' contains name of each node for debug process.
 *  \var TaskNode::cmd_id
 *  Member 'cmd_id' contains cmd id from _cmd_t structure.
 *  \var TaskNode::date
 *  Member 'date' contains date of sms.
 *  \var TaskNode::args
 *  Member 'args' contains pointer to cmd arguments.
 *  \var TaskNode::next
 *  Member 'next' contains pointer to the next eement of structure
 */
typedef struct _TaskNode
{
	uint8_t id;
	char *name;				/* debug name from cmd structure */
	uint8_t cmd_id;			/* from cmd - used for merge */
	time_t date;
	int (*handler)(char *, char *);
	char *args;

	struct TaskNode *next;
} TaskNode;


/** \struct TaskQueue
 *  \brief TaskQueue structure define typical queue interface
 *  \var TaskQueue::head
 *  Member 'head' contains pointer to the head of queue.
 *  \var TaskQueue::tail
 *  Member 'tail' contains pointer to the tail of queue.
 *  \var TaskQueue::add
 *  Member 'add' contains pointer to function which add new node to tail.
 *  \var TaskQueue::del
 *  Member 'del' contains pointer to function which remove node from head.
 *  \var TaskQueue::run
 *  Member 'run' contains pointer to function which run task in head.
 *  \var TaskQueue::printdbg
 *  Member 'printdbg' contains pointer to function which print debug info.
 *  \var TaskQueue::size
 *  Member 'size' contains number of nodes in queue.
 */
typedef struct TaskQueue
{
	TaskNode* head;
	TaskNode* tail;

	int (*add) (struct TaskQueue*, char*, uint8_t, time_t date,
									int(*handler)(char *, char *), char *args); // add item to tail
	// get item from head and remove it from queue
	void (*del) (struct TaskQueue*);
	// run task in queue
	TaskResult* (*run) (struct TaskQueue*);
	// size of this queue
	void (*printdbg) (struct TaskQueue*);
	// get task node by id
	TaskNode* (*getNodeById) (struct TaskQueue*, uint8_t id);

	int size;
} TaskQueue;


int addTask(TaskQueue* queue, char* name, uint8_t cmd_id, time_t date,
									int(*handler)(char *, char *), char *args);
void delTask(TaskQueue* queue);
TaskResult* runTask(TaskQueue* queue);
void printdbgTask(TaskQueue *queue);
TaskNode* getTaskById(TaskQueue* queue, uint8_t id);
TaskQueue createTaskQueue();

#endif /* TASK_QUEUE_H_ */
