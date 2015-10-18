/*
 * room.h
 *
 *  Created on: Oct 9, 2015
 *      Author: thor
 */

#ifndef INC_ROOM_H_
#define INC_ROOM_H_

#include <pthread.h>

#include "user.h"
#include "list.h"

typedef enum exit_type_enum {
    EXIT_TYPE_SPECIAL,
    EXIT_TYPE_N,
    EXIT_TYPE_S,
    EXIT_TYPE_E,
    EXIT_TYPE_W,
    EXIT_TYPE_NE,
    EXIT_TYPE_NW,
    EXIT_TYPE_SE,
    EXIT_TYPE_SW,
    EXIT_TYPE_UP,
    EXIT_TYPE_DOWN,
    EXIT_TYPE_IN,
    EXIT_TYPE_OUT
} EXIT_TYPE;

typedef enum exit_status_enum {
    EXIT_STATUS_NONE,
    EXIT_STATUS_SPECIAL,
    EXIT_STATUS_OPEN,
    EXIT_STATUS_CLOSED,
    EXIT_STATUS_LOCKED,
    EXIT_STATUS_UNK
} EXIT_STATUS;

// room structure
typedef struct room_struct {

	// id of the room
	unsigned long id;

	// room title
	char *title;

	// room description
	char *description;

    LinkedList exits;

    // mutex
    pthread_mutex_t mutex;

} * Room;

// exit structure
typedef struct exit_struct {
    EXIT_TYPE type;
    EXIT_STATUS status;
    Room room;
} * Exit;

// parse a room file and return a room
Room room_from_file( char * );

// destroy a room
void destroy_room( Room );

void room_lock( Room );
void room_unlock( Room );

// get the full description of the room
int room_get_full_description( Room, User, char **, int * );

#endif /* INC_ROOM_H_ */

