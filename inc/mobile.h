/*
 * mobile.h
 *
 *  Created on: Jan 17, 2016
 *      Author: thor
 */

#ifndef INC_MOBILE_H_
#define INC_MOBILE_H_

#include "mob.h"
#include "stats.h"
#include "management.h"

typedef struct mobile_struct {

    Mob parent;

    unsigned short id;
    
    // max number of spawns
    unsigned short max_spawns;

    // current number of spawns
    unsigned short current_spawns;

} * Mobile;

/**
 * An instance of the mobile which is free moving
 */
typedef struct mobile_instance_struct {

    // its base mobile
    Mobile mobile;

    // same as mobiles parent
    Mob parent;

    // its own stats instance
    Stats stats;

} * MobileInstance;

// new mobile
Mobile new_mobile( );

// new mobile from source file
Mobile new_mobile_from_file( char *file );

// destory mobile
void destroy_mobile( void * );

// clean mobile table
void mobile_clean( HashTable mobiles );

// add mobile to the manager
void mobile_add_to_manager( Management );

// process category tag
void mobile_process_category( Management, xmlTextReaderPtr, const xmlChar* );


// --------------------------------------------------
// mobile instances


// new mobile instance
MobileInstance new_mobile_instance( Mobile );

// destroy mobile instance
void destroy_mobile_instance( MobileInstance );


#endif

