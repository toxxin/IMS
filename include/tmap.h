/*
 * tmap.h
 *
 *  Created on: 04.12.2013
 *      Author: anton
 */


#ifndef TMAP_H_
#define TMAP_H_


/** \struct TMap
 *  \brief This structure defines table map for matching inceptions to task
 *  \var TMap::iid
 *  Member 'iid' contains unique number of matched inception.
 *  \var TaskResult::code
 *  Member 'tid' contains unique number of matched task.
 */
typedef struct TMap
{
	uint8_t id;
	uint8_t iid;
	uint8_t tid;
} TMap;


/** \struct TMapNode
 *  \brief This structure defines node of tmap list
 *  \var TMapNode::task_id
 *  Member 'TMap' contains pointer to tmap node.
 *  \var TMapNode::next
 *  Member 'next' contains pointer to next node in list.
 *  \var TaskResult::prev
 *  Member 'prev' contains pointer to previous node in list.
 */
typedef struct TMapNode
{
	struct TMap *node;

	struct TMapNode *next;
	struct TMapNode *prev;
} TMapNode;



typedef struct TMapList
{
	TMapNode* head;
	TMapNode* tail;

	// add item to tail
	int (*add) (struct TMapList *list,  uint8_t, uint8_t);
	// get item from head and remove it from list
	void (*del) (struct TMapList*);
	// remove all items by inception id
	void (*delByIncId) (struct TMapList*, uint8_t);
	// print debug info about nodes
	void (*printdbg) (struct TMapList*);
	// get Tmap by id
	TMap* (*getTMapById) (struct TMapList*, uint8_t);
	// get all task id by inception id
	int (*getTaskIdByIncId) (struct TMapList*, uint8_t, uint8_t*);
	// is empty list
	int (*isEmpty) (struct TMapList*);
	// clean list
	void (*clean) (struct TMapList*);

	int size;
} TMapList;


int addNode(TMapList *list, uint8_t iid, uint8_t tid);
void delNode(TMapList *list);
void delNodeByIncId(TMapList *list, uint8_t inception_id);
void printdbgTMap(TMapList *list);
TMap* getTMapById(TMapList *list, uint8_t id);
int getTaskIdByIncId(TMapList *list, const uint8_t inception_id, uint8_t *id);
int isTMapListEmpty(TMapList *list);
void cleanTMapList(TMapList *list);
TMapList createTMapList(void);


#endif /* TMAP_H_ */
