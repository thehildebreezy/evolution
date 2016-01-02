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
#include "../inc/management.h"



/**
 * Add the rooms to the manager
 * @param manager The manager to add rooms to 
 */
void room_add_to_manager( Management manager )
{

    // load the rooms list
    
    // start text reader
    xmlTextReaderPtr reader;
    reader = xmlReaderForFile( "res/world.xml", NULL, 0 );
    
    // check for validity
    if( reader != NULL ) {
    
        
        // check reader status
        int reader_status = xmlTextReaderRead( reader );
        while( reader_status == 1 ) {
            
            // initialize values
            const xmlChar *nameT;
            char *name;
    
            nameT = xmlTextReaderConstName(reader);
            name = quick_copy( (char *)nameT );
    
            if( name != NULL ){
            
                // process level
                // look for world level
                
                // look for continent level
                if( quick_comp( (char *)name, "continent" ) == 0 ){
                
                    const xmlChar *src = xmlTextReaderGetAttribute( reader, "src" );
                    //tmpPtr = attr;
                    if( src != NULL ){
                        // attribute check
                        
                        room_process_continent( manager, reader, src );
                        
                        // free attr
                        xmlFree( (xmlChar *)  src );
                    }
                    
                    
                }
                
                //if( name != NULL )
                    xmlFree( (xmlChar *)  name );
            }
            
            
            // update reader status
            reader_status = xmlTextReaderRead( reader );
        }
        
        
        // free the reader object
        xmlFreeTextReader( reader );
        
    } else {
        printf("error loading rooms\n");
        return;
    }
    

    //Room room1 = room_from_file("res/world/core/login.room");
	//hash_table_add( manager->rooms, &(room1->id), room1);
}

/**
 * Process a continent node of a world file
 * @param reader The reader to process from
 * @param src the source folder to draw continent information from
 */
void room_process_continent( Management manager, xmlTextReaderPtr reader, const xmlChar *src )
{
    // check reader status
    int reader_status = xmlTextReaderRead( reader );
    while( reader_status == 1 ) {
        // initialize values
        const xmlChar *nameT;
        char *name;

        nameT = xmlTextReaderConstName(reader);
        name = quick_copy( (char *) nameT );
        
        if( name != NULL ){
            
            // reader stuff
            int node_type = xmlTextReaderNodeType(reader);
            
            if( quick_comp( (char *)name, "continent" ) == 0 && 
                node_type == NODE_ENDELEMENT ) {
            
                free( name );    
                return;
            }
            
            // now look for areas
            if( quick_comp( (char *)name, "area" ) == 0 ){
            
                const xmlChar *attr = xmlTextReaderGetAttribute( reader, "area-id" );
                if( attr != NULL ){
                    // attribute check
                    int areaid = atoi( attr );
                    
                    room_process_area( manager, reader, src, (unsigned long)areaid );
                    
                    // free attr
                    xmlFree( (xmlChar *)  attr );
                }
            }
            
            free( name );
        }
        
        reader_status = xmlTextReaderRead( reader );
    }
}

/**
 * Process the area node of the world
 * @param reader the reader to process the area from
 * @param src the source folder for the continent
 * @param areaid the id of the area 
 */
void room_process_area( Management manager, xmlTextReaderPtr reader, const xmlChar *src, unsigned long areaid )
{
    
    // check reader status
    int reader_status = xmlTextReaderRead( reader );
    while( reader_status == 1 ) {
        // initialize values
        const xmlChar *nameT, *value;
        char *name;

        nameT = xmlTextReaderConstName(reader);
        name = quick_copy( (char *)nameT );
        
        
        if( name != NULL ){
            
            // reader stuff
            int node_type = xmlTextReaderNodeType(reader);
            
            
            if( quick_comp( (char *)name, "area" ) == 0 && 
                node_type == NODE_ENDELEMENT ) {
                
                free( name );
                return;
            }
            
            
            // now look for areas
            if( quick_comp( (char *)name, "room" ) == 0 ){
            
                // now we have to read until we find a value
                const xmlChar *value = NULL;
                
                reader_status = xmlTextReaderRead( reader );
                
                while( reader_status == 1 && value == NULL ){
                
                    int node_type = xmlTextReaderNodeType( reader );
                    
                    if( node_type == NODE_ENDELEMENT ) break;
                    
                    if( node_type == NODE_TEXT ){
                        value = xmlTextReaderConstValue( reader );
                    }
                    
                    if( value != NULL ){
                    
                    
	                    printf("%s\n",value);
                        
                        // length
                        unsigned long length = strlen(src) + strlen(value) + 17;
                        
                        // room file build
                        char *room_file = (char *)malloc( length );
                        strcpy( room_file, "res/world/" );
                        strcat( room_file, src );
                        strcat( room_file, "/" );
                        strcat( room_file, value );
                        strcat( room_file, ".room" );
                        
                        // close up room file
                        room_file[length-1] = 0;
                        
                        // load room
                        
                        Room room = room_from_file(room_file);
	                    hash_table_add( manager->rooms, &(room->id), room);
                        
                        // free the room file string
                        free( room_file );
                        
                    }
                    
                    reader_status = xmlTextReaderRead( reader );
                }
                
                
            }
            
            free( name );
        }
        
        reader_status = xmlTextReaderRead( reader );
    }
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
	
	room->area_id = 0;
	
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
            const xmlChar *nameT;
            char *name;
            
            nameT = xmlTextReaderConstName(reader);
            name = quick_copy( (char *) nameT );
            
            if( name != NULL ){
            
                // parse the room tag
                room_parse_tag( room, name, reader );
                
                free( name );
            }
            
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
    
    if( node_type == NODE_ENDELEMENT ) return 0;
    
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
        int id = atoi( attr );
        if( id != INT_MAX && id != INT_MIN ){
            room->id = (unsigned long) id;
        }
        xmlFree( (xmlChar *)  (xmlChar *) attr );
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

/**
 * Get the room structure for the room id
 * @param rooms the hashtable of rooms from the manager
 * @param id the id of the room to grab
 * @param Room structure of given room
 */
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
    if( room == NULL) return NULL;
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

    if( room == NULL ) return 0;
    
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

