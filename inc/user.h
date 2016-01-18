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
#include "mob.h"

#define USER_FLAG_EXIT 1 << 0


// need to pre define this before including character
typedef struct user_struct {

    // parent is the mob
    Mob parent;

    // children clients
	Client client;
	
	pthread_mutex_t mutex;
    unsigned char flags;

} * User;

// save structure to store character information
typedef struct save_struct {
    char  user_name[32];
    short character_version;
    short password_version;
    unsigned long  flags;
} * Save;

// create a new user
User new_user( Client );

void destroy_user( User );

// mutex funs
void user_lock( User );
void user_unlock( User );

// save user
int  user_save( User );

// set user to exit
int user_exit( User );

// user loading functions
Save user_get_save( char * );
User user_load_saved( Save );


#endif /* INC_USER_H_ */
