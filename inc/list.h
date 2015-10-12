/*
 * list.h
 *
 *  Created on: Oct 3, 2015
 *      Author: thor
 */

#ifndef INC_LIST_H_
#define INC_LIST_H_

// type definition
typedef struct linked_list_item {

	struct linked_list_item *next;

	struct linked_list_item *prev;

	void *data;

} * LinkedList;


// create new linked list item
LinkedList new_linked_list( void *data );

// free item
void destroy_linked_item( LinkedList );

// add item to linked list
LinkedList add_linked_item( LinkedList, void * );

// index at
unsigned long get_linked_index( LinkedList );

// next item in the list
LinkedList next_linked_item( LinkedList );

// remove this item from the list
LinkedList remove_linked_item( LinkedList * );

// remove this item with particular data from the list
LinkedList remove_linked_data( LinkedList *, void * );

// remove item at this index
LinkedList remove_linked_item_at( LinkedList *, unsigned long );



#endif /* INC_LIST_H_ */
