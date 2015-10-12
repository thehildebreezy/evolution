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

	user->client = client;

	// create character
	Character character = new_character();
	user->character = character;


	// create mutex
	pthread_mutex_t mutex;
	pthread_mutex_init( &mutex, NULL );
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
