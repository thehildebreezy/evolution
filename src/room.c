/*
 * room.c
 *
 *  Created on: Oct 9, 2015
 *      Author: thor
 */

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../inc/user.h"
#include "../inc/room.h"
#include "../inc/func.h"

/**
 * Parse the room file to create a room
 * @param file File to parse for the room
 * @return A new Room struct
 */
Room room_from_file( char *file ){

	// temp
	Room room = malloc( sizeof(struct room_struct) );
	if( room == NULL ){
		perror("Malloc room");
		return NULL;
	}
	
	// init
	memset( room, 0, sizeof( struct room_struct ) );

	room->id = 0;
	room->title = "\033[1mWelcome to Olypia\033[0m";
	room->description = "Who are you?";

    room->exits = NULL;
    
    
	// mutex
	pthread_mutex_t mutex;
	int status = pthread_mutex_init( &mutex, NULL );
	room->mutex = mutex;

	return room;
}

/**
 * Destroy the room
 * @param room Room to destroy
 */
void destroy_room( Room room ) {

	// re activiate these when update above
	//free( room->title );
	//free( room->description );
    pthread_mutex_destroy( &(room->mutex) );
    
	free( room );
}

/**
 * Get the full description of the room, must be freed by receiver
 * @param room Room to get desc of
 * @param user User requesting description
 * @param pointer to description buffer
 * @return length o description, -1 on fail
 */
int room_get_full_description( 
    Room room, 
    User user,
    char **description,
    int  *length
) {

    int title_len = strlen( room->title );
    int descr_len = strlen( room->description );

    if( title_len + descr_len >= *length ){
    
        // grow buffer
        int new_len = grow_buffer( 
            (void **)description, 
            title_len + descr_len + 2 );
        
        // error check
        if( new_len < 0 ){
            perror("Reallocating for room description");
            return -1;
        }
        
        // re assign
        *length = new_len;
    }

    // copy values
	strcat( *description, room->title );
	strcat( *description, "\n" );
	strcat( *description, room->description );
	strcat( *description, "\n" );


	return title_len + descr_len + 2;
}

/**
 * Locks the room struct for threaded use
 * @param room Room to lock
 */
void room_lock( Room room ) {
	pthread_mutex_lock( &(room->mutex) );
}

/**
 * Unlocks the room for threaded use
 * @param room Room struct to unlock
 */
void room_unlock( Room room ) {
	pthread_mutex_unlock( &(room->mutex) );
}

