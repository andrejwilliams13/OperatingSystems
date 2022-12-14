#include<stdio.h>
#include<stdlib.h>

struct list* list_create();
void list_free(struct list* list);
void list_insert(struct list* list, void* val);
void list_remove(struct list* list, void* val, int (*cmp)(void* a, void* b));
int list_position(struct list* list, void* val, int (*cmp)(void* a, void* b));
void list_reverse(struct list* list);

struct link
{
void* val;
struct link* next;
};

struct list
{
struct link* head;
};

struct list* list_create()
{
struct list *list=(struct list *)malloc(sizeof(struct list));
list->head=NULL;
return list;
}

void list_free(struct list* list)
{
struct link *link=list->head;
while(link)
{
struct link *next=link->next;
free(link);
link=next;
}
list->head=NULL;
}

void list_insert(struct list* list, void* val)
{
struct link *new_node=(struct link *)malloc(sizeof(struct link));
new_node->val=val;
new_node->next=list->head;
list->head=new_node;
}

void list_remove(struct list* list, void* val, int (*cmp)(void* a, void* b))
{
struct link *link=list->head;
struct link *prev=NULL;
while(link)
{
if(*(int *)(link->val)==*(int *)(val))
{
break;
}
prev=link;
link=link->next;
}
if(!link)
{
return;
}
if(!prev)
{
list->head=list->head->next;
}
else
{
prev->next=link->next;
free(link);
}
}

int list_position(struct list* list, void* val, int (*cmp)(void* a, void* b))
{
int index=0;
for(struct link *link=list->head;link;link=link->next)
{
if(*(int *)(link->val)==*(int *)(val))
{
return index;
}
index++;
}
return -1;
}

void list_reverse(struct list* list)
{
struct link *prev=NULL;
struct link *curr=list->head;
struct link *next=NULL;
while(curr)
{
next=curr->next;
curr->next=prev;
prev=curr;
curr=next;
}
list->head=prev;
}

// function that prints the list
void print_list(struct list *list)
{
if(!list)
{
return;
}
printf("List: ");
if(!list->head)
{
printf("Empty\n");
return;
}
for(struct link *link=list->head;link;link=link->next)
{
printf("%d ",*(int *)(link->val));
}
printf("\n");
}
