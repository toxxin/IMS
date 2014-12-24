/*
 * task_result_queue.h
 *
 *  Created on: 22.10.2013
 *      Author: Anton
 */

#ifndef TASK_RESULT_QUEUE_H_
#define TASK_RESULT_QUEUE_H_


typedef struct TaskResult
{
	uint8_t id;
	char *name;
	uint16_t code;
	char *result;
} TaskResult;


/** \struct TaskResultNode
 *  \brief This structure define result of a Task
 *  \var TaskResult::task_id
 *  Member 'task_id' contains unique number for each task.
 *  \var TaskResult::code
 *  Member 'code' contains result code of each task.
 *  \var TaskResult::result
 *  Member 'result' contains pointer to result string.
 */
typedef struct _TaskResultNode
{
	struct TaskResult *result;

	struct TaskResultNode *next;
	struct TaskResultNode *prev;
} TaskResultNode;


/** \struct TaskResultQueue
 *  \brief TaskResultQueue structure define typical queue interface
 *  \var TaskResultQueue::head
 *  Member 'head' contains pointer to the head of queue.
 *  \var TaskResultQueue::tail
 *  Member 'tail' contains pointer to the tail of queue.
 *  \var TaskResultQueue::add
 *  Member 'add' contains pointer to function which add new node to tail.
 *  \var TaskResultQueue::del
 *  Member 'del' contains pointer to function which remove node from head.
 *  \var TaskResultQueue::run
 *  Member 'run' contains pointer to function which run task in head.
 *  \var TaskResultQueue::printdbg
 *  Member 'printdbg' contains pointer to function which print debug info.
 *  \var TaskResultQueue::size
 *  Member 'size' contains number of nodes in queue.
 */
typedef struct TaskResultQueue
{
	TaskResultNode* head;
	TaskResultNode* tail;

	// add item to tail
	int (*add) (struct TaskResultQueue *queue, TaskResult* result);
	// get item from head and remove it from queue
	void (*del) (struct TaskResultQueue*);
	// size of this queue
	void (*printdbg) (struct TaskResultQueue*);
	// get task node by id
	TaskResultNode* (*getNodeById) (struct TaskResultQueue*, uint8_t id);
	// clean queue
	void (*clean) (struct TaskResultQueue*);

	int size;
} TaskResultQueue;


TaskResultNode* getTaskResultById(TaskResultQueue* queue, uint8_t id);
int addTaskResult(TaskResultQueue* queue, TaskResult* result);
void delTaskResult(TaskResultQueue* queue);
void printdbgResult(TaskResultQueue *queue);
void cleanTaskResult(TaskResultQueue *queue);

TaskResultQueue createTaskResultQueue(void);

#endif /* TASK_RESULT_QUEUE_H_ */
