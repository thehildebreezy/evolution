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

	free( room );
}

/**
 * Get the full description of the room, must be freed by receiver
 * @param room Room to get desc of
 * @param user User requesting description
 * @param pointer to description buffer
 * @return number of
 */
char *room_get_full_description( Room room, User user ) {

	// temp, fix later
	char *buffer = (char *)malloc( 2048 );
    memset( buffer, 0, 2048 );

	strcat( buffer, room->title );
	strcat( buffer, "\n" );
	strcat( buffer, room->description );
	strcat( buffer, "\n" );



	return buffer;
}
