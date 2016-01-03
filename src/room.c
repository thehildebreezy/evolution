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
#include "../inc/list.h"
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

unsigned long tag_value( char *tag )
{
    unsigned int tagv = 0;
    unsigned int tag_len = strlen( tag );
    int i = 0;
    for( ; i<tag_len; i++ ){
        tagv += tag[i];
    }
    return tagv;
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
    unsigned int tagv = tag_value( (char *)tag );
    
    switch (tagv) {
    case TAG_ROOM:
        
        if( depth == 0 && node_type == NODE_ELEMENT ) {
            // read room id, etc
            room_tag_room( room, reader );
        }
        break;
    case TAG_DESCRIPTION:
    
        if( depth == 1 && node_type == NODE_ELEMENT ) {
            room_tag_description( room, reader );
        }
        break;
    case TAG_EXITS:
            
        if( depth == 1 && node_type == NODE_ELEMENT ) {
            // room_tag_exits( room, reader );
            // look for hidden / visible
            room_tag_exits( room, reader );
            
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
        xmlFree( (xmlChar *) attr );
        //free( (xmlChar *)attr );
    }
    
    attr = xmlTextReaderGetAttribute( reader, "title" );
    if( attr != NULL ){
        room->title = (char *)attr;
    }
    
    return 0;
}


/**
 * Load room description from file
 * @param room The room struct to update
 * @param reader the xml reader to read from
 * @return 0 on success, -1 on failure
 */
int room_tag_description( Room room, xmlTextReaderPtr reader )
{
    int reader_status = xmlTextReaderRead( reader );
    const xmlChar *value = NULL;
    
    while( reader_status == 1 && value == NULL ){
    
        int node_type = xmlTextReaderNodeType( reader );
        
        if( node_type == NODE_ENDELEMENT ) break;
        
        if( node_type == NODE_TEXT ){
            value = xmlTextReaderConstValue( reader );
        }
        
        // found value
        if( value != NULL ){
        
            // strlen
            int vlen = strlen( value );
            
            // description holder
            char *desc = (char *)malloc( vlen );
        
            int len = trimwhitespace( desc, vlen, value );
            if( len > 0 ){
                room->description = desc;
            }
            break;
        }
        
        reader_status = xmlTextReaderRead( reader );
    }
    
    return 0;
}

/**
 * Load room exits from file
 * @param room The room struct to update
 * @param reader the xml reader to read from
 * @return 0 on success, -1 on failure
 */
int room_tag_exits( Room room, xmlTextReaderPtr reader )
{
    int reader_status = xmlTextReaderRead( reader );
    
    while( reader_status == 1 ){
    
        // grab name
        const xmlChar *name = xmlTextReaderConstName( reader );
        unsigned long tagv = tag_value( (char *)name );
        
        int node_type = xmlTextReaderNodeType( reader );
        
        if( tagv == TAG_EXITS && 
            node_type == NODE_ENDELEMENT ) break;
        
        switch( tagv ){
        case TAG_VISIBLE:
            room_tag_exits_visible( room, reader );
            break;
        case TAG_HIDDEN:
            room_tag_exits_hidden( room, reader );
            break;
        default:
            break;
        }
        
        reader_status = xmlTextReaderRead( reader );
    }
    
    return 0;
}


/**
 * Load room exits from file
 * @param room The room struct to update
 * @param reader the xml reader to read from
 * @return 0 on success, -1 on failure
 */
int room_tag_exits_visible( Room room, xmlTextReaderPtr reader )
{
    
    int reader_status = xmlTextReaderRead( reader );
    
    while( reader_status == 1 ){
    
        // grab name
        const xmlChar *name = xmlTextReaderConstName( reader );
        unsigned long tagv = tag_value( (char *)name );
        
        int node_type = xmlTextReaderNodeType( reader );
        
        if( tagv == TAG_VISIBLE && 
            node_type == NODE_ENDELEMENT ) break;
        
        if( tagv == TAG_EXIT )
            room_tag_visible_exit( room, reader );
        
        reader_status = xmlTextReaderRead( reader );
    }
    
    return 0;
}

/**
 * Load room exits from file
 * @param room The room struct to update
 * @param reader the xml reader to read from
 * @return 0 on success, -1 on failure
 */
int room_tag_exits_hidden( Room room, xmlTextReaderPtr reader )
{
    
    int reader_status = xmlTextReaderRead( reader );
    
    while( reader_status == 1 ){
    
        // grab name
        const xmlChar *name = xmlTextReaderConstName( reader );
        unsigned long tagv = tag_value( (char *)name );
        
        int node_type = xmlTextReaderNodeType( reader );
        
        if( tagv == TAG_HIDDEN && 
            node_type == NODE_ENDELEMENT ) break;
        
        if( tagv == TAG_EXIT )
            room_tag_hidden_exit( room, reader );
        
        reader_status = xmlTextReaderRead( reader );
    }
    
    return 0;
}

/**
 * Load a visible exit
 * @param room The room struct to update
 * @param reader the xml reader to read from
 * @return 0 on success, -1 on failure
 */
int room_tag_visible_exit( Room room, xmlTextReaderPtr reader )
{

    // get the room id to load
    const xmlChar *roomAttr = xmlTextReaderGetAttribute( reader, "room" );
    
    // if no room drop it
    if( roomAttr == NULL ) return -1;
    
    // get room id
    int roomid = atoi(roomAttr);
    if( roomid == INT_MIN || roomid == INT_MAX ){
        xmlFree( (xmlChar *) roomAttr );
        return -1;
    }
    
    xmlFree( (xmlChar *) roomAttr );
    
    // new exit
    Exit exit = new_exit( (unsigned long)roomid );
    if( exit == NULL ){
    
        xmlFree( (xmlChar *) roomAttr );
        return -1;
    }
    
    // look for type
    const xmlChar *type = xmlTextReaderGetAttribute( reader, "type" );
    
    
    
    int reader_status = xmlTextReaderRead( reader );
    
    // get attributes
    
    
    while( reader_status == 1 ){
    
        // grab name
        const xmlChar *name = xmlTextReaderConstName( reader );
        unsigned long tagv = tag_value( (char *)name );
        
        int node_type = xmlTextReaderNodeType( reader );
        
        if( tagv == TAG_EXIT && 
            node_type == NODE_ENDELEMENT ) break;
        
        unsigned long valv = 0;
        
        switch( tagv ){
        case TAG_STATE: ;
            const xmlChar *state = room_tag_get_value( reader );
            valv = tag_value( (char *) state );
            
            switch( valv ){
            case TAG_VALUE_OPEN:
                exit_open( exit );
                break;
            default:
                exit_open( exit );
                break;
            }
            
            break;
        case TAG_DIRECTION: ;
            const xmlChar *dir = room_tag_get_value( reader );
            valv = tag_value( (char *) state );
            
            switch( valv ){
            case TAG_VALUE_NORTH:
                exit->type = EXIT_TYPE_N;
                break;
            default:
                exit->type = EXIT_TYPE_SPECIAL;
                break;
            }
            
            break;
        default:
            break;
        }
        
        reader_status = xmlTextReaderRead( reader );
    }
    
    if( type != NULL )
        xmlFree( (xmlChar *) type );    
    
    if( room->exits == NULL ){
        room->exits = new_linked_list( exit );
    } else {
        room->exits = add_linked_item( room->exits, exit );
    }
    
    return 0;
}

/**
 * Load room hidden exit
 * @param room The room struct to update
 * @param reader the xml reader to read from
 * @return 0 on success, -1 on failure
 */
int room_tag_hidden_exit( Room room, xmlTextReaderPtr reader )
{
    // return room_tag_visible
}

/**
 * Get the value of a tag
 * @param reader xml reader to get from
 * @return NULL on fail, value on success
 */
const xmlChar *room_tag_get_value( xmlTextReaderPtr reader )
{

    int reader_status = xmlTextReaderRead( reader );
    const xmlChar *value = NULL;
    
    while( reader_status == 1 && value == NULL ){
    
        int node_type = xmlTextReaderNodeType( reader );
        
        if( node_type == NODE_ENDELEMENT ) return NULL;
        
        if( node_type == NODE_TEXT ){
            value = xmlTextReaderConstValue( reader );
        }
        
        // found value
        if( value != NULL ){
        
            return value;
        }
        
        reader_status = xmlTextReaderRead( reader );
    }
    return NULL;
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
        
    if( room->exits != NULL ){
        destroy_linked_list_func( room->exits, (void (*)(void *))destroy_exit );
    }
        
        
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


/* ===========================================================
 * Exit functions
 */
 
/**
 * Create a new exit structure
 * @param type type of exit this is
 * @param status of the exit
 * @param roomid id of the room of exit
 * @param cmd any special command to use
 * @return the new exit struct or null on failure
 */
Exit new_exit( unsigned long roomid )
{
    Exit exit = (Exit)malloc( sizeof(struct exit_struct) );
    if( exit == NULL ){
        return NULL;
    }
    
    exit->type = 0;
    exit->status = 0;
    exit->roomid = roomid;
    exit->cmd = NULL;
    
    return exit;
}

/**
 * Opens an exit
 * @param exit exit to open
 */
void exit_open( Exit exit )
{
    // set status to open
    exit->status &= ~(EXIT_STATUS_CLOSED);
}

/**
 * Closes an exit
 * @param exit exit to close
 */
void exit_close( Exit exit )
{
    // set status to closed
    exit->status |= EXIT_STATUS_CLOSED;
}

/**
 * Locks an exit
 * @param exit exit to lock
 */
void exit_lock( Exit exit )
{
    exit->status |= EXIT_STATUS_LOCKED;
}

/**
 * Unlocks an exit
 * @param exit exit to unlock
 */
void exit_unlock( Exit exit )
{
    exit->status &= ~(EXIT_STATUS_LOCKED);
}



/**
 * Destroy the exit object
 * @param exit the exit structure to free
 */
void destroy_exit( Exit exit )
{
    if( exit != NULL && exit->cmd != NULL );
        free( exit->cmd );
    if( exit != NULL );
        free( exit );
}



