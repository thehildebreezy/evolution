/*
 * room.h
 *
 *  Created on: Oct 9, 2015
 *      Author: thor
 */

#ifndef INC_ROOM_H_
#define INC_ROOM_H_

#include <pthread.h>
#include <libxml/xmlreader.h>

#include "user.h"
#include "list.h"
#include "hash.h"
#include "management.h"

// define some tag identifiers
#define TAG_ROOM 445
#define TAG_DESCRIPTION 1188
#define TAG_TRIGGERS 871
#define TAG_TRIGGER 756
#define TAG_EXITS 557
#define TAG_EXIT 442
#define TAG_VISIBLE 750
#define TAG_HIDDEN 620
#define TAG_STATE 545
#define TAG_DIRECTION 961 
#define TAG_COMMAND 735

#define NODE_ELEMENT 1
#define NODE_ENDELEMENT 15
#define NODE_TEXT 3

// forward declaration
typedef struct evolution_management_struct * Management;


typedef enum tag_context_enum {
    TAG_CONTEXT_ROOM,
    TAG_CONTEXT_TRIGGERS,
    TAG_CONTEXT_EXITS,
    TAG_CONTEXT_EXIT_VISIBLE,
    TAG_CONTEXT_EXIT_HIDDEN,
} TAG_CONTEXT;

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

    // room area id
    unsigned long area_id;

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

// add rooms to the manager
void room_add_to_manager( Management );

// process a continent
void room_process_continent( Management, xmlTextReaderPtr, const xmlChar * );

// process area
void room_process_area( Management, xmlTextReaderPtr, const xmlChar *, unsigned long  );

// parse a room file and return a room
Room room_from_file( char * );

// update a room based on a node
int room_parse_tag( Room, const xmlChar *, xmlTextReaderPtr );

// read in room tag info
int room_tag_room( Room, xmlTextReaderPtr );

// destroy a room
void destroy_room( Room );

void room_clean( HashTable );
void room_free( void * );

// get a room by id
Room room_get( HashTable, unsigned long );


void room_lock( Room );
void room_unlock( Room );

// get individual descriptions
char *room_get_title( Room );
char *room_get_description( Room );

// get the full description of the room
int room_get_full_description( Room, User, char **, int * );

#endif /* INC_ROOM_H_ */

