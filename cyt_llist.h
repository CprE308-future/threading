

#ifndef CYT_LLIST_H
#define CYT_LLIST_H



#define LLIST(object, name) \
struct {object * prev; object * next;} name

#define LLIST_CREATE_HEAD(object, list, name) \
struct { struct {object * prev; object * next; } list; } _llist_##name, * name = &_llist_##name;

// initialize a new list by making next and prev point back to self
#define LLIST_INIT(list, obj) \
do { \
	obj->list.next = (typeof(obj->list.next))obj; \
	obj->list.prev = (typeof(obj->list.prev))obj; \
} while(0);

// insert obj2 after obj1 in the list list
#define LLIST_INSERT_AFTER(list, obj1, obj2) \
do { \
	obj2->list.next = (__typeof__(obj2->list.next))obj1->list.next; \
	obj2->list.prev = (__typeof__(obj2->list.prev))obj1; \
	obj1->list.next->list.prev = (__typeof__(obj1->list.next->list.prev))obj2; \
	obj1->list.next = (__typeof__(obj1->list.next))obj2; \
} while(0);

// insert obj2 before obj1 in the list list
#define LLIST_INSERT_BEFORE(list, obj1, obj2) \
do { \
	obj2->list.next = (__typeof__(obj2->list.next))obj1; \
	obj2->list.prev = (__typeof__(obj2->list.prev))obj1->list.prev; \
	obj1->list.prev->list.next = (__typeof__(obj1->list.prev->list.next))obj2; \
	obj1->list.prev = (__typeof__(obj1->list.prev))obj2; \
} while(0);

// remove obj from the list
#define LLIST_REMOVE(list, obj) \
do { \
	obj->list.prev->list.next = (__typeof__(obj->list.prev->list.next))obj->list.next; \
	obj->list.next->list.prev = (__typeof__(obj->list.next->list.prev))obj->list.prev;\
} while(0);

// get the next object in the list
#define LLIST_NEXT(list, obj) obj->list.next

// get the previous object in the list
#define LLIST_PREV(list, obj) obj->list.prev


#define LLIST_FOREACH(list, head, i) \
for(i=head->list.next; (__typeof__(head))i != head; i=LLIST_NEXT(list, i))
//LLIST_NEXT(list, head)
#endif

