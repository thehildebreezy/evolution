/*
 * action.c
 *
 *  Created on: Oct 9, 2015
 *      Author: thor
 */

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../inc/action.h"
#include "../inc/hash.h"
#include "../inc/user.h"
#include "../inc/management.h"
#include "../inc/character.h"


/**
 * Add the actions to the manager actions table
 * @param manager Management struct to build into
 */
void action_add_to_manager( Management manager ) {

	// add look action to actions table
	Action look = new_action( action_look );
	hash_table_add( manager->actions, (void *)"look", look);

	// add look action to actions table
	Action shout = new_action( action_shout );
	hash_table_add( manager->actions, (void *)"shout", shout);
}

/**
 * Cleans action table from the manager
 * @param manager Management structure to clean
 */
void action_clean( HashTable actions ) {
    if( actions != NULL ){
        // clean everything, no special malloc so should be good
        // might have to add a func later or special data destruction
        destroy_hash_table_data( actions );
    }
}

/**
 * Parses an action message from the user
 * @param response The string response from the user
 * @param length The length of the response, for convenience
 * @param user The user who sent the response
 * @param manager Manager holding global information
 * @return 0 on success -1 on failure
 */
int action_parse_response(
		char *response,
		int length,
		User user,
		Management manager
){

    // parse first part of string
    // all strings are only so big
    char command[16];
    command[0] = '\0';
    int i=0;
    while( 
        i < 15 && 
        i < length &&
        !(  response[i] == ' '  || 
            response[i] == '\0' || 
            response[i] == '\n' ||
            response[i] == '\r'
        )
    ){
        command[i] = response[i];
        i++;
    }

    command[i] = '\0';
    
	// just testing
	Action action = (Action)hash_table_get_from(
			manager->actions,
			command );

	if( action != NULL ){
	    // check for extra response
	    if( i == length ){
	        action->action_func(NULL, user, manager);
	    } else {
	        action->action_func(response+i+1, user, manager);
	    }
		
	} else {
	    client_send( user->client, "I don't understand\n" );
	}
	

	return 0;
}


/**
 * Create a new action with action function
 * @param action_func pointer to action function for this action
 * @return a new Action structure
 */
Action new_action( void *(*action_func)( const char *, User, Management ) ) {
	Action action = (Action)malloc( sizeof(struct action_struct) );
	
	// check
	if( action == NULL ){
	    perror("malloc action");
	    return NULL;
	}
	
	// init
	memset( action, 0, sizeof(struct action_struct) );
	
	action->action_func = action_func;

	return action;
}

/**
 * Look at the current room
 * @param response message following action command
 * @param user User making action request
 * @param manager Global resource manager
 */
void *action_look( const char *response, User user, Management manager) {

	client_send( user->client, "You look around\n" );
	return NULL;
}



/**
 * Speak to all users
 * @param response message following action command
 * @param user User making action request
 * @param manager Global resource manager
 */
void *action_shout( const char *response, User user, Management manager) {

	
	// simple say
	LinkedList next = manager->users;
	while( next != NULL ){

		// if it is this user
		if( next->data == user ) {

			// just say its you
			client_send( ((User)(next->data))->client, "You shout: ");

		} else {

			// send name of sender
			client_send(
					((User)(next->data))->client,
					((User)(next->data))->character->name
			);

			// what to do when the client is not the user
			client_send( ((User)(next->data))->client, " shouts: " );
		}

		// send message
		client_send( ((User)(next->data))->client, (char *)response);
		next = next_linked_item( next );
	}
	
	return NULL;
}

