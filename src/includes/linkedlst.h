#include "main.h"

typedef struct node_t {
	int key;
	struct node *next;
} Node;

typedef struct list_t {
	int size;
	node_t *head;
} List;

List *create(void);
void pshLst(List *list, int key);
void LLprt(List *list);
void LLclr(List *list);
int isEmpty(List *list);

List *create(void) {
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
	Node *node = malloc(sizeof(node_t));
	if (node == NULL) {
		perror(PROGRAM);
		exit(EXIT_FAILURE);
	}
	node->key = key;
	node->next = list->head;
	list->head = node;
	list->size++;
}

void prtLst(list_t* list) {
	if (isEmpty(list)) return;
	node_t* ptr = list->head;
	printf("\n -----------------------> Size = %d\n", list->size);
	while (ptr != NULL) {
		printf (" : %02d %02d", ptr->data.T1, ptr->data.T2);
		printf (" : %02d %02d", ptr->data.A, ptr->data.B);
		printf (" : %02d\n", ptr->data.value);
		ptr = ptr->next;
	}
}

void LLclr(list_t* list) {
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

int isEmpty(List *list) {
	return (list->head == NULL);
}