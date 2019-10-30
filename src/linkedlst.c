#include "includes/main.h"

List *iniLst(void) {
	List *list = malloc(sizeof(List));
	if (list == NULL) {
		perror(PROGRAM);
		exit(EXIT_FAILURE);
	}
	list->size = 0;
	list->head = NULL;
	return list;
}

void pshLst(List *list, int key) {
	Node *node = malloc(sizeof(Node));
	if (node == NULL) {
		perror(PROGRAM);
		exit(EXIT_FAILURE);
	}
	node->key = key;
	node->next = list->head;
	list->head = node;
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