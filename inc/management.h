/*
 * management.h
 *
 *  Created on: Oct 3, 2015
 *      Author: thor
 */

#ifndef INC_MANAGEMENT_H_
#define INC_MANAGEMENT_H_

#include "list.h"
#include "user.h"
#include "hash.h"

typedef struct evolution_management_struct {

	// cease action flag
	int cease;

	// list of connected users
	LinkedList users;

	// hash table of available commands here
	HashTable actions;

} * Management;

Management new_manager();
void destroy_manager( Management );

void manager_add_user( Management, User );
void manager_remove_user( Management, User );


#endif /* INC_MANAGEMENT_H_ */
