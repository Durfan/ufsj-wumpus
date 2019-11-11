#ifndef INCLUDE_LINKEDLST_H
#define INCLUDE_LINKEDLST_H

typedef struct node_t {
	int key;
	struct node_t *next;
} Node;

typedef struct list_t {
	int size;
	struct node_t *head, *tail;
} List;

List *iniLst(void);
int lstidR(List *list, int index);
int popLst(List *list);
void pshLst(List *list, int key);
void prtLst(List *list);
void clrLst(List *list);
int lstnil(List *list);
void pshTailLst(List *list, int key);
void CLEARLst(List *list);

//#pragma message __FILE__
#endif // INCLUDE_LINKEDLST_H
