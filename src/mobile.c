/*
 * Author: thor
 * Date:   18 Jan 2016
 */

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <libxml/xmlreader.h>

#include "../inc/mobile.h"
#include "../inc/mob.h"
#include "../inc/func.h"

/**
 * Craete a new mobile creature for the world
 * @return new Mobile structure
 */
Mobile new_mobile()
{
    Mobile mobile = (Mobile)malloc( sizeof( struct mobile_struct ) );
    if( mobile == NULL ){
        return NULL;
    }
    
    // mob instance
    Mob mob = new_mob( MOB_TYPE_MOBILE, mobile );
    if( mob == NULL ){
        free( mobile );
        return NULL;
    }
    
    mobile->parent = mob;
    
    mobile->max_spawns = 0;
    
    return mobile;
}


/** 
 * Creates a new mobile from a source file
 * @param the file to load from
 * return a new mobile struct pointer
 */
Mobile new_mobile_from_file( char *file )
{
    Mobile mobile = new_mobile();
    
    
    return mobile;
}

/**
 * free the mobile data
 * @param mob The mob to free
 */
void destroy_mobile( void *mobile )
{
    if( ((Mobile)mobile)->parent );
        free( ((Mobile)mobile)->parent );
    free( mobile );
}

/**
 * Cleans mobile table from the manager
 * @param manager Management structure to clean
 */
void mobile_clean( HashTable mobiles ) {
    if( mobiles != NULL ){
        destroy_hash_table_func( mobiles, destroy_mobile );
    }
}


/**
 * Add mobiles from the directory to the mobile source table
 * @param manager the manager to add the mobile to
 */
void mobile_add_to_manager( Management manager )
{
    // load the mobile list
    
    // start text reader
    xmlTextReaderPtr reader;
    reader = xmlReaderForFile( "res/mobs/directory.xml", NULL, 0 );
    
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
                if( quick_comp( (char *)name, "category" ) == 0 ){
                
                    const xmlChar *src = xmlTextReaderGetAttribute( reader, "src" );
                    //tmpPtr = attr;
                    if( src != NULL ){
                        // attribute check
                        
                        mobile_process_category( manager, reader, src );
                        
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
}


/**
 * Process a category node of the mobile directory file
 * @param reader The reader to process from
 * @param src the source folder to draw continent information from
 */
void mobile_process_category( Management manager, 
    xmlTextReaderPtr reader, 
    const xmlChar *src )
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
            
            if( quick_comp( (char *)name, "category" ) == 0 && 
                node_type == NODE_ENDELEMENT ) {
            
                free( name );    
                return;
            }
            
            // now look for areas
            if( quick_comp( (char *)name, "mob" ) == 0 ){
            
                // mob from file
                const xmlChar *attr = xmlTextReaderGetAttribute( reader, "file" );
                
                if( attr != NULL ){
                
                    // build string
                    int str_len = strlen( attr ) + strlen( src ) + 22;
                    
                    char *file = malloc( str_len + 1 );
                    
                    strcpy( file, "res/mobs/mobiles/" );
                    strcat( file, src );
                    strcat( file, "/" );
                    strcat( file, attr );
                    strcat( file, ".mob" );
                    
                    file[str_len] = 0;
                
                    // 
                    Mobile mobile = new_mobile_from_file( file );
                    if( mobile != NULL )
                        hash_table_add( manager->mobiles, &(mobile->id), mobile );
                    
                    free( file );
                    
                    
                    // free attr
                    xmlFree( (xmlChar *)  attr );
                }
            }
            
            free( name );
        }
        
        reader_status = xmlTextReaderRead( reader );
    }
}


/* ====================================================================
 * Mobiles Instance functions
 */

/**
 * Create a new mobile instance 
 * @param mobile the mobile this instance is sourced from
 * @return a new MobileInstance
 */
MobileInstance new_mobile_instance( Mobile mobile )
{
    MobileInstance instance = (MobileInstance)malloc( sizeof(struct mobile_instance_struct) );
    if( instance == NULL ){
        return NULL;
    }
    
    instance->mobile = mobile;
    instance->parent = mobile->parent;
}

/**
 * Destroy this mobile instance
 * @param instance the mobile instance to destroy
 */
void destroy_mobile_instance( MobileInstance instance )
{
    free( instance );
}

