#include "main.h"
//#include <stdlib.h>
//#include <stdio.h>
//#include "linkedlst.h"

List *iniLst(void) {
	List *list = malloc(sizeof(List));
	if (list == NULL) {
	//	perror(PROJECT_NAME);
	//	exit(EXIT_FAILURE);
	}
	list->size = 0;
	list->head = NULL;
	list->tail = NULL;
	return list;
}

int lstidx(List *list, int index) {
	if (index >= 0 && index < list->size) {
		Node *node = list->head;
		for (int i=0; i<index; i++)
			node = node->next;
		return node->key;
	}
	return -1;
}

int lstidR(List *list, int index) {
	if (index >= 0 && index < list->size) {
		Node *node = list->head;
		int key;

		if (list->head->next == NULL) {
			list->head = NULL;
			list->tail = NULL;
			key = node->key;
			free(node);
		} else {
			if (index == 0) {
				key = node->key;
				list->head = node->next;
				free(node);
			}
			else {
				Node *aux = list->head;
				for (int i=0; i < index; i++) {
					node = aux;
					aux = node->next;
				}
				key = aux->key;
				node->next = aux->next;
				free(aux);
			}
		}
		list->size--;
		return key;
	}
	return -1;
}

int popLst(List *list) {
	if (list->size > 0) {
		Node *node = list->head;
		int key = node->key;
		list->head = node->next;
		if(list->size == 1){
			list->tail = NULL;
		}
		list->size--;
		free(node);
		return key;
	}
	return -1;
}

void pshTailLst(List *list, int key){
	Node *node = malloc(sizeof(Node));
	if (node == NULL) {
	//	perror(PROJECT_NAME);
	//	exit(EXIT_FAILURE);
	}
	node->key = key;
	node->next = NULL;
	if(list->head == NULL || list->tail == NULL){
		list->head = node;
		list->tail = node;
			list->size++;
	}else{
		list->tail->next = node;
		list->tail = node;
	list->size++;
	}
}

void pshLst(List *list, int key) {
	Node *node = malloc(sizeof(Node));
	if (node == NULL) {
	//	perror(PROJECT_NAME);
	//	exit(EXIT_FAILURE);
	}

	node->key = key;
	node->next = list->head;
	list->head = node;
	if(list->tail == NULL) list->tail = node;
	list->size++;
}

void prtLst(List *list) {
	if (lstnil(list)) return;
	Node *ptr = list->head;
	while (ptr != NULL) {
		printf (" %02d", ptr->key);
		ptr = ptr->next;
	}
	putchar(0x0A);
}

void clrLst(List *list) {
	if (list->head == NULL) {
		free(list);
		return;
	}
	Node *delNode;
	while (list->head != NULL) {
		delNode = list->head;
		list->head = list->head->next;
		free(delNode);
	}
	free(list);
}

int lstnil(List *list) {
	return (list->head == NULL);
}
