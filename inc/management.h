/*
 * management.h
 *
 *  Created on: Oct 3, 2015
 *      Author: thor
 */

#ifndef INC_MANAGEMENT_H_
#define INC_MANAGEMENT_H_

#include <pthread.h>

#include "list.h"
#include "user.h"
#include "hash.h"

typedef struct evolution_management_struct {

	// cease action flag
	int cease;
	
	// count active threads
	unsigned long thread_count;
	pthread_t last_thread;

	// list of connected users
	LinkedList users;

    pthread_mutex_t users_mutex;

	// hash table of available commands here
	HashTable actions;
	
	// hashtable of available rooms
	HashTable rooms;
	
	// hashtable of the mobiles
	HashTable mobiles;

} * Management;

Management new_manager();
void destroy_manager( Management );

void manager_add_user( Management, User );
void manager_remove_user( Management, User );

void manager_set_ceased( Management );
int manager_is_ceased( Management );

unsigned long manager_thread_count( Management );
unsigned long manager_up_thread( Management, pthread_t );
unsigned long manager_down_thread( Management, pthread_t );

#endif /* INC_MANAGEMENT_H_ */
