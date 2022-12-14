#include <stdio.h>
#include "list.h"

int main()
{
struct list *list=list_create();
int arr[5]={1,2,3,4,5};
for(int i=0;i<5;i++)
{
list_insert(list,&arr[i]);
}
print_list(list); // prints 5 4 3 2 1
list_reverse(list);
print_list(list); // prints 1 2 3 4 5
return 0;
}
