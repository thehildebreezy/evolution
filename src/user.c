/*
 * user.c
 *
 *  Created on: Oct 4, 2015
 *      Author: thor
 */

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>

#include "../inc/user.h"
#include "../inc/client.h"
#include "../inc/character.h"
#include "../inc/mob.h"

/**
 * Create a new user
 * @param client to associate with user
 * @return new user pointer struct
 */
User new_user( Client client ) {


	User user = malloc( sizeof(struct user_struct ) );
	if( user == NULL ){
		perror("Malloc user");
		return NULL;
	}
	
    // init
    memset( user, 0, sizeof( struct user_struct ) );
    
    // generate the parent, if this fails everything does
    Mob mob = new_mob( MOB_TYPE_USER, user );
    if( mob == NULL ){
        perror("malloc mob");
        free( user );
        return NULL;
    }
    
    user->parent = mob;

	user->client = client;

    user->flags = 0;
    
	// create character
	Character character = new_character( user );
	user->character = character;


	// create mutex
	pthread_mutex_t mutex;
	int status = pthread_mutex_init( &mutex, NULL );
    user->mutex = mutex;

	return user;
}

/**
 * Destroy the user
 * @param user User to destroy
 */
void destroy_user( User user ) {

	pthread_mutex_destroy(&(user->mutex));
	destroy_character( user->character );
	free( user );

}


/**
 * Exit the user by setting a flag
 * @param user the user to let exit
 * @return 0 on success
 */
int user_exit( User user )
{
    user->flags |= USER_FLAG_EXIT;
    return 0;
}

/**
 * Save the user data
 * @param user User to save
 * @return 0 on failure
 */
int user_save( User user ) {

    return 0;
}

/**
 * Get save data for a user which is used to load the user
 * and character
 * @param username Name of the user to get
 * @return Save struct pointer
 */
Save user_get_save( char * username ) {

    return NULL;
}

/**
 * Load user and character from a set of save data
 * @param save Save data to load up
 */
User user_load_saved( Save save ) {

    return NULL;
}


/**
 * Locks the user's mutex
 * @param user User to lock
 */
void user_lock( User user ) {
	pthread_mutex_lock( &(user->mutex) );
}

/**
 * Unlocks the user
 * @param user User to unlock
 */
void user_unlock( User user ){
	pthread_mutex_unlock( &(user->mutex) );
}

