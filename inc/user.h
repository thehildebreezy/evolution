/*
 * user.h
 *
 *  Created on: Oct 4, 2015
 *      Author: thor
 */

#ifndef INC_USER_H_
#define INC_USER_H_



// forward declare Character
typedef struct character_struct *Character;

// include things I'll need
#include "character.h"
#include "client.h"
#include "pthread.h"


// need to pre define this before including character
typedef struct user_struct {

	Character character;
	Client client;
	pthread_mutex_t mutex;

} * User;

// create a new user
User new_user( Client );

void destroy_user( User );

// mutex funs
void user_lock( User );
void user_unlock( User );

#endif /* INC_USER_H_ */
