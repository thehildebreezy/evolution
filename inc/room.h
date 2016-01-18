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

#define TAG_VALUE_OPEN 434
#define TAG_VALUE_NORTH 555



// forward declaration
typedef struct evolution_management_struct * Management;

/** Tag context for traversing xml */
typedef enum tag_context_enum {
    TAG_CONTEXT_ROOM,
    TAG_CONTEXT_TRIGGERS,
    TAG_CONTEXT_EXITS,
    TAG_CONTEXT_EXIT_VISIBLE,
    TAG_CONTEXT_EXIT_HIDDEN,
} TAG_CONTEXT;

/** Types of standard exits */
typedef enum exit_type_enum {
    EXIT_TYPE_SPECIAL = 0,
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
    EXIT_TYPE_OUT,
    EXIT_TYPE_MAX
} EXIT_TYPE;

/** constant array of directions for quick lookup */
extern const char *EXIT_TYPE_NAMES[];


#define EXIT_STATUS unsigned char
#define EXIT_STATUS_CLOSED  1
#define EXIT_STATUS_LOCKED  1 << 1
#define EXIT_STATUS_HIDDEN  1 << 2
#define EXIT_STATUS_SPECIAL 1 << 3

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
    
    // all the mobs in the room
    LinkedList mobs;
    
    // special list for users
    LinkedList users;

    // count number of ticks on current room
    unsigned short tick_count;

    // list of mobiles that spawn here
    LinkedList spawns;

    // mutex
    pthread_mutex_t mutex;

} * Room;

// exit structure
typedef struct exit_struct {
    EXIT_TYPE type;
    EXIT_STATUS status;
    //Room room;
    unsigned long roomid;
    char *cmd;
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

/* --------------------------------------
 * Room tag functions
 */

// read in room tag info
int room_tag_room( Room, xmlTextReaderPtr );

// int read in description tag
int room_tag_description( Room, xmlTextReaderPtr );

// int read in exits tag
int room_tag_exits( Room, xmlTextReaderPtr );

// int read in visible exits tag
int room_tag_exits_visible( Room, xmlTextReaderPtr );

// int read in hidden exits tag
int room_tag_exits_hidden( Room, xmlTextReaderPtr );

// int read in visible exit
int room_tag_visible_exit( Room, xmlTextReaderPtr );

// int read in hidden exit
int room_tag_hidden_exit( Room, xmlTextReaderPtr );

// get a value
const xmlChar *room_tag_get_value( xmlTextReaderPtr raeder );

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

// get room exits
LinkedList room_get_exits( Room );

// get the full description of the room
int room_get_full_description( Room, User, char **, int * );


/* -------------------------------------------------
 * Room directional getters
 */
Room room_get_dir( Room, HashTable, EXIT_TYPE );
Room room_get_north( Room, HashTable );
Room room_get_south( Room, HashTable );
Room room_get_east( Room, HashTable );
Room room_get_west( Room, HashTable );

/* -----------------------------------------
 * Exit funcs
 */
Exit new_exit( unsigned long );

void exit_open( Exit );

void exit_close( Exit );

void exit_lock( Exit );

void exit_unlock( Exit );

/* --------------------------------------------------------
 * Exit getters
 */
 
// get direction text
const char *exit_get_dir_text( Exit );

// check if is hidden
EXIT_STATUS exit_is_hidden( Exit );



void exit_clean( void * );

void destroy_exit( Exit );

#endif /* INC_ROOM_H_ */

