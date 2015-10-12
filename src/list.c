/*
 * list.c
 *
 *  Created on: Oct 3, 2015
 *      Author: thor
 */

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../inc/list.h"


// create new linked list item
LinkedList new_linked_list( void *data ) {
	LinkedList list = malloc(sizeof(struct linked_list_item));
	if( list == NULL ){
		perror("Malloc list item");
		return NULL;
	}
	
	// initialize
	memset( list, 0, sizeof( struct linked_list_item ) );

	list->data = data;

	list->next = NULL;
	list->prev = NULL;

	return list;
}

// free linked item
void destroy_linked_item( LinkedList item ) {
	free( item );
}

// add item to linked list
LinkedList add_linked_item( LinkedList list, void * item ) {

	LinkedList node = new_linked_list( item );

	node->next = list;
	return node;
}

// index at
unsigned long get_linked_index( LinkedList item ) {
	unsigned long i = 0;
	LinkedList current = item;
	while( current->prev != NULL ){
		i++;
		current = current->prev;
	}
	return i;
}

// next item in the list
LinkedList next_linked_item( LinkedList item ) {
	if( item == NULL ) return NULL;
	return item->next;
}

// remove this item from the list
LinkedList remove_linked_item( LinkedList *item ) {
	if( item == NULL ) return NULL;

	LinkedList current = *item;
	LinkedList prev = current->prev;
	LinkedList next = current->next;


	if( current->prev == NULL ){
		*item = next;
		return current;
	}

	prev->next = next;
	next->prev = prev;

	return current;
}


// remove this item with particular data from the list
LinkedList remove_linked_data( LinkedList *list, void *data ) {
	if( list == NULL ) return NULL;

	LinkedList current = *list;

	// move to data item
	while( current != NULL && current->data != data ){
		current = current->next;
	}

	if( current == NULL ) return NULL;

	if( current->prev == NULL ){
		*list = current->next;
		return current;
	}

	LinkedList prev = current->prev;
	LinkedList next = current->next;

	prev->next = next;
	next->prev = prev;

	return current;
}

// remove item at this index
LinkedList remove_linked_item_at(
		LinkedList *list,
		unsigned long index
) {

	if( list == NULL ) return NULL;

	LinkedList current = *list;

	if( index == 0 ){
		*list = current->next;
		return current;
	}


	current = current->next;

	int i=1;
	while( current != NULL && i < index ){
		i++;
		current = current->next;
	}

	if( current == NULL ) return NULL;

	LinkedList prev = current->prev;
	LinkedList next = current->next;

	prev->next = next;
	next->prev = prev;

	return current;

}

