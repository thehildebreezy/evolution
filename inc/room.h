/*
 * room.h
 *
 *  Created on: Oct 9, 2015
 *      Author: thor
 */

#ifndef INC_ROOM_H_
#define INC_ROOM_H_

#include "user.h"

// room structure
typedef struct room_struct {

	// id of the room
	unsigned long id;

	// room title
	char *title;

	// room description
	char *description;

} * Room;

// parse a room file and return a room
Room room_from_file( char * );

// destroy a room
void destroy_room( Room );

// get the full description of the room
char *room_get_full_description( Room, User );

#endif /* INC_ROOM_H_ */
