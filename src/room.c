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

#include <libxml/xmlreader.h>

#include "../inc/user.h"
#include "../inc/room.h"
#include "../inc/func.h"
#include "../inc/hash.h"

/**
 * Add the rooms to the manager
 * @param manager The manager to add rooms to 
 */
void room_add_to_manager( Management manager )
{
    Room room1 = room_from_file("res/world/core/login.room");
	hash_table_add( manager->rooms, &(room1->id), room1);
}

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
	
	// init values to nothing
	room->title = NULL;
	room->description = NULL;
	

    room->exits = NULL;
    
    
    // start text reader
    xmlTextReaderPtr reader;
    reader = xmlReaderForFile( file, NULL, 0 );
    
    
    // check for validity
    if( reader != NULL ) {
    
        // check reader status
        int reader_status = xmlTextReaderRead( reader );
        while( reader_status == 1 ) {
            
            // initialize values
            const xmlChar *name;
    
            name = xmlTextReaderConstName(reader);
            if( name == NULL ){
                // try next
                continue;
            }
            
            
            // parse the room tag
            room_parse_tag( room, name, reader );
            
            // update reader status
            reader_status = xmlTextReaderRead( reader );
        }
        
        // free the reader object
        xmlFreeTextReader( reader );
    
    } else {
    
        // no file data, abort
        free( room );
        return NULL;
    }
    
	// mutex
	pthread_mutex_t mutex;
	int status = pthread_mutex_init( &mutex, NULL );
	room->mutex = mutex;

	return room;
}

/**
 * Parse a tag in the room description 
 * @param room Room to parse xml tag for
 * @param tag the tag name to parse for the reader
 * @param reader The reader value to gather data for
 * @return -1 on failure, anything else on success
 */
int room_parse_tag( 
    Room room, 
    const xmlChar *tag, 
    xmlTextReaderPtr reader )
{
    
    if( tag == NULL || strlen(tag) <= 0 ) return -1;
    
    
    // reader stuff
    int depth = xmlTextReaderDepth(reader);
    int node_type = xmlTextReaderNodeType(reader);
    
    if( node_type == 15 ) return 0;
    
    int empty = xmlTextReaderIsEmptyElement(reader);
    int has_value = xmlTextReaderHasValue(reader);
    
    
    
    // important stuff here
    // compute tag numerical value
    unsigned int tag_value = 0;
    unsigned int tag_len = strlen( tag );
    int i = 0;
    for( ; i<tag_len; i++ ){
        tag_value += tag[i];
    }
    
    switch (tag_value) {
    case TAG_ROOM:
        
        if( depth == 0 && node_type == NODE_ELEMENT ) {
            // read room id, etc
            room_tag_room( room, reader );
        }
        break;
    default:
        break;
    }
    
    // value = xmlTextReaderConstValue( reader );

}

/**
 * Read in information from the room tag
 * @param room The room struct to update
 * @param reader the xml reader to read from
 * @return 0 on success, -1 on failure
 */
int room_tag_room( Room room, xmlTextReaderPtr reader )
{

    const xmlChar *attr = xmlTextReaderGetAttribute( reader, "id" );
    if( attr != NULL ){
        unsigned long id = (unsigned long) atoi( attr );
        if( id != INT_MAX && id != INT_MIN ){
            room->id = id;
        }
        xmlFree( (xmlChar *) attr );
        //free( (xmlChar *)attr );
    }
    
    attr = xmlTextReaderGetAttribute( reader, "title" );
    if( attr != NULL ){
        room->title = (char *)attr;
    }
}


/**
 * Cleans room table from the manager
 * @param manager Management structure to clean
 */
void room_clean( HashTable rooms ) {
    if( rooms != NULL ){
        // clean everything, no special malloc so should be good
        // might have to add a func later or special data destruction
        destroy_hash_table_func( rooms, room_free );
    }
}

/**
 * Special function for freeing the room
 * @param room The room to free
 */
void room_free( void *room )
{
    destroy_room( (Room) room );
}

/**
 * Destroy the room
 * @param room Room to destroy
 */
void destroy_room( Room room ) {
    
    if( room == NULL ) return;
    
	// remove in needed
	if( room->title != NULL )
        free( room->title );
    if( room->description != NULL )
        free( room->description );
        
    pthread_mutex_destroy( &(room->mutex) );
    
	free( room );
}


Room room_get( HashTable rooms, unsigned long id )
{
    unsigned long tmp = id;
    return (Room) hash_table_get_from( rooms, &tmp );
}


/**
 * Get just the title for the room
 * @param room Room to get title of
 * @return character string of room title
 */
char *room_get_title( Room room )
{
    return room->title;
}


/**
 * Get just the basic for the room
 * @param room Room to get description of
 * @return character string of room description
 */
char *room_get_description( Room room )
{
    return room->description;
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

    char *title = room_get_title( room );
    char *desc = room_get_description( room );

    if( title == NULL ) title = "";
    if( desc == NULL ) desc = "";
    
    int title_len = strlen( title );
    int descr_len = strlen( desc );

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
	strcat( *description, title );
	strcat( *description, "\n" );
	strcat( *description, desc );
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

