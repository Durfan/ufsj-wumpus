#ifndef INCLUDE_LINKEDLST_H
#define INCLUDE_LINKEDLST_H

typedef struct node_t {
	int key;
	struct node_t *next;
} Node;

typedef struct list_t {
	int size;
	struct node_t *head;
} List;

List *iniLst(void);
int lstidx(List *list, int index);
void pshLst(List *list, int key);
void prtLst(List *list);
void clrLst(List *list);
int lstnil(List *list);
int lstidxR(List *list, int index);

//#pragma message __FILE__
#endif // INCLUDE_LINKEDLST_H
