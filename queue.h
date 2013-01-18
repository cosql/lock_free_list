#ifndef __QUEUE_H__
#define __QUEUE_H__

#include <stdlib.h>

#include "common.h"

typedef struct QueueNode {
	int val;
	struct QueueNode *next;
} QueueNode;

typedef struct Queue {
	QueueNode * head;
	QueueNode * tail;
} Queue;

QueueNode * newQueueNode(int val) {
	QueueNode* node =  (QueueNode *) malloc(sizeof(QueueNode));
	node->val = val;
	node->next = NULL;

	return node;
}

Queue* newQueue() {
	Queue* queue = (Queue*) malloc(sizeof(Queue));
	queue->head = newQueueNode(0);
	queue->tail = queue->head;

	return queue;
}

void deleteQueueNode(QueueNode *node) {
	free(node);
	return;
}

void EnQueue(Queue* queue, int val) {
	QueueNode *newNode = newQueueNode(val);
	QueueNode* tail = queue->tail;
	QueueNode* oldTail = tail;
	
	
	do {
		while (tail->next != NULL) 
			tail = tail->next;
	} while (!casX((volatile void **)&tail->next, (void *)NULL, (void *) newNode));
	
	casX((volatile void**)&(queue->tail), (void *)oldTail, (void *)newNode);
	return;
}

int Dequeue(Queue* queue, int* val) {
	QueueNode* oldHead = queue->head->next;
	QueueNode* head = oldHead;

	do {
		oldHead = queue->head->next;
		head = queue->head->next;
		if (head == NULL) {
			return -1;
		}
	} while (!casX((volatile void **)&queue->head->next, (void *)head, (void *)head->next));

	*val = oldHead->val;
	deleteQueueNode(oldHead);
	return 0;
}

#endif
