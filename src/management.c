/*
 * management.c
 *
 *  Created on: Oct 4, 2015
 *      Author: thor
 */

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../inc/management.h"
#include "../inc/list.h"
#include "../inc/hash.h"

/**
 * Create a new server manager
 * @return new manager pointer
 */
Management new_manager() {
	Management manager = malloc(
			sizeof(struct evolution_management_struct)
		);

    if( manager == NULL ){
        perror("Malloc management");
        return NULL;
    }
    
    // initialize buffer
    memset( manager, 0, sizeof( struct evolution_management_struct ) );

	manager->users = NULL;

	// generate the hash table base
	manager->actions = create_hash_table(
			253,
			string_hash_djb2,
			string_comp_func
		);

	return manager;
}

/**
 * Destroys the manager
 * @param manager manager to destroy
 */
void destroy_manager( Management manager ) {
	if( manager->users != NULL ){

		LinkedList current = manager->users;
		while( current != NULL ){
			LinkedList last = remove_linked_item( &current );
			// destroy_user( data )
			destroy_linked_item( last );
		}
	}

	// destroy hash table with data
	if( manager->actions != NULL ){
		destroy_hash_table_data( manager->actions );
	}

	free( manager );
}

/**
 * Add a user to the list of users
 * @param manager Manager to adjust
 * @param user User to add
 */
void manager_add_user( Management manager, User user ){
	manager->users = add_linked_item( manager->users, user );
}

/**
 * Remove a user from the list of users
 * @param manager Manager to adjust
 * @param user User to remove
 */
void manager_remove_user( Management manager, User user ) {
	LinkedList dropped_user = remove_linked_data(
			&(manager->users),
			user
		);
}


