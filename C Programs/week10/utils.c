#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"

int approxEqual(float el, float refval, float tolerance)
{
    if(el < refval - tolerance || el > refval + tolerance)		//Check to compare the values passed in.
        return 0;

    return 1;
}

llist* add_list(llist* node, int r, int c, float val,int tid)
{
	llist* current = malloc(sizeof(llist));

	if(!current)
	{
		printf("Unable to allocate memory : add_list(llist*, %d, %d, %f)\n", r, c, val);
		exit(1);
	}

	if(node)
	{
		node->next = current;              	//Add new node to the tail of the previous node.
		current->prev = node;
	}
	else
	{
		node = current;                   	//First node in the list.
		current->prev = NULL;
	}

	current->r = r;                       	//Allocate data.
	current->c = c;
	current->val = val;
	current->tid = tid;
	current->next = NULL;                  	//Indicate the end of the list.

	return current;
}

llist* get_head_list(llist* node)
{
	llist* head=NULL;
	llist* current=node;

	while(current)
	{
		head = current;
		current = current->prev;       		//Go one node back.
	}

	return head;
}

llist* clear_list(llist* node)
{
	llist* current = get_head_list(node);   //Go to start of the list.
	llist* temp;
	while (current)
	{
		temp = current->next;
		free(current);
		current = temp;
	}
	return NULL;                            //The list now points to NULL.
}

void print_list(llist* node)
{
	llist*	current = get_head_list(node);  //Go to start of the list.
	llist*	temp;
	while (current)
	{
        temp = current->next;
        fprintf(stdout, "r=%d, c=%d: %.6f, (thread= %d)\n", current->r, current->c, current->val, current->tid);
        current = temp;
	}
}

char flipChar(char c)
{
    if('a' <= c && c <= 'z')                //A valid Lowercase.
        return 'z' - c + 'a';

    else if('A' <= c && c <= 'Z')           //A valid Uppercase.
        return 'Z' - c + 'A';

    else if('0' <= c && c <= '9')           //A valid Digit.
        return '9' - c + '0';

    return c;                               //Wasn't LC/UC/Digit so leave alone.
}
