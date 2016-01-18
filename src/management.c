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
#include "../inc/action.h"
#include "../inc/list.h"
#include "../inc/hash.h"
#include "../inc/room.h"
#include "../inc/mobile.h"

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
	
	pthread_mutex_t mutex;
	pthread_mutex_init( &mutex, NULL );
	manager->users_mutex = mutex;
	
	manager->cease = 0;
    manager->thread_count = 0;
    manager->last_thread = (pthread_t)0;

	// generate the hash table base
	manager->actions = create_hash_table(
			253,
			string_hash_djb2,
			string_comp_func
		);
		
		
	// generate the hash table for rooms
	// sourced by id, integer style
	manager->rooms = create_hash_table(
			253,
			int_hash,
			int_comp_func
		);
		
	// generate the hash table for mobiles
	// sourced by mobile id, integer style
	manager->mobiles = create_hash_table(
			253,
			int_hash,
			int_comp_func
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
		action_clean( manager->actions );
	}
	
	if( manager->rooms != NULL ){
	    room_clean( manager->rooms );
	}
	
	if( manager->mobiles != NULL ){
	    mobile_clean( manager->mobiles );
	}
	
	pthread_mutex_destroy( &(manager->users_mutex ));

	free( manager );
}

/**
 * Add a user to the list of users
 * @param manager Manager to adjust
 * @param user User to add
 */
void manager_add_user( Management manager, User user ){
    pthread_mutex_lock( &(manager->users_mutex) );
	manager->users = add_linked_item( manager->users, user );
    pthread_mutex_unlock( &(manager->users_mutex) );
}

/**
 * Remove a user from the list of users
 * @param manager Manager to adjust
 * @param user User to remove
 */
void manager_remove_user( Management manager, User user ) {

    pthread_mutex_lock( &(manager->users_mutex) );
	LinkedList dropped_user = remove_linked_data(
			&(manager->users),
			user
		);
		
	// and destroy the linked item
	destroy_linked_item( dropped_user );
    pthread_mutex_unlock( &(manager->users_mutex) );
}


/**
 * Set manager to ceased 
 * @param manager Management struct
 */
void manager_set_ceased( Management manager ) {
    manager->cease = 1;
}


/** 
 * Reports if the manager is ceased yet
 * @param manager Manager to check
 * @return 1 if true, 0 otherwise
 */
int manager_is_ceased( Management manager ) {
    return manager->cease == 1;
}


/**
 * Retrieve thread count
 * @param manager management struct
 * @return number of threads still working
 */
unsigned long manager_thread_count( Management manager ) {
    return manager->thread_count;
}

/**
 * Increase thread count
 * @param manager management struct
 * @return number of threads still working
 */
unsigned long manager_up_thread( 
    Management manager, 
    pthread_t thread 
) {
    manager->thread_count += 1;
    return manager->thread_count;
}

/**
 * Decrease thread count, setes last thread if this the last thread
 * @param manager management struct
 * @return number of threads still working
 */
unsigned long manager_down_thread( 
    Management manager,
    pthread_t thread
){
    if( manager->thread_count == 1 ){
        manager->last_thread = thread;
    }
    manager->thread_count -= 1;
    return manager->thread_count;
}

