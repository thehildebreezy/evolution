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
#include "../inc/room.h"


/**
 * Add the actions to the manager actions table
 * @param manager Management struct to build into
 */
void action_add_to_manager( Management manager ) {

	// add exit action to actions table
	Action quit = new_action( action_quit );
	hash_table_add( manager->actions, (void *)"quit", quit);

    /* --------------------
     * movement actions
     */
    // north
    Action north = new_action_linked( action_north, 2 );
    hash_table_add( manager->actions, (void *)"north", north);
    hash_table_add( manager->actions, (void *)"n", north);
    
    // south
    Action south = new_action_linked( action_south, 2 );
    hash_table_add( manager->actions, (void *)"south", south);
    hash_table_add( manager->actions, (void *)"s", south);
    
    // east
    Action east = new_action_linked( action_east, 2 );
    hash_table_add( manager->actions, (void *)"east", east);
    hash_table_add( manager->actions, (void *)"e", east);
    
    // west
    Action west = new_action_linked( action_west, 2 );
    hash_table_add( manager->actions, (void *)"west", west);
    hash_table_add( manager->actions, (void *)"w", west);
    
    /* --------------------
     * common actions
     */

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
        destroy_hash_table_func( actions, destroy_action );
    }
}

/**
 * Destroy the action when links run out
 * @param action the action to destroy
 */
void destroy_action( void *action )
{
    ((Action)action)->links--;
    if( ((Action)action)->links <= 0 ){
        free( action );
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
	
	action->links = 1;

	return action;
}


Action new_action_linked( void *(*action_func)( const char *, User, Management ), char links )
{
    Action action = new_action( action_func );
    action->links = links;
    return action;
}

/**
 * Quit from the game
 * @param response message following action command
 * @param user User making action request
 * @param manager Global resource manager
 */
void *action_quit( const char *response, User user, Management manager) 
{
    user_lock( user );
	user_exit( user );
	user_unlock( user );
	return NULL;
}

/**
 * Look at the current room
 * @param response message following action command
 * @param user User making action request
 * @param manager Global resource manager
 */
void *action_look( const char *response, User user, Management manager) {

    user_lock( user );
	
	Room current = char_get_room( user->parent->character );
	if( current == NULL ){
	    client_send( user->client, "You do not appear to be anywhere\n");
	}
	
	// get login
	int desc_length = 2048;
	char *desc = malloc( desc_length );
	
	// init
	memset( desc, 0, desc_length );
	
    int len = room_get_full_description( 
        current,
        user,
        &desc,
        &desc_length );

	// send the login screen
	client_send( user->client, desc );
	
	
	// loop over exits
	LinkedList link = room_get_exits( current );
	
	int shown = 0;
	
    while( link != NULL ){
        
        Exit exit = (Exit) link->data;
        
        // if not hidden
        if( exit_is_hidden( exit ) == 0 ) {
        
            const char *exit_name = exit_get_dir_text( exit );
            
            if( exit_name != NULL ){
                
                if( shown > 0 )
                    client_send( user->client, (char *)"," );
                
                shown = 1;
                client_send( user->client, (char *)exit_name );
            }
        }
        
        link = next_linked_item( link );
        
    }
	
	if( shown > 0 )
	    client_send( user->client, (char *)"\n" );

	// free up the buffer
	if( desc != NULL ){
		free( desc );
	}
	
	user_unlock( user );
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

        User current = (User) next->data;
        
        // lock this user
        user_lock( current );
        
		// if it is this user
		if( current == user ) {

			// just say its you
			client_send( current->client, "You shout: ");

		} else {

			// send name of sender
			client_send(
					current->client,
					current->parent->character->name
			);

			// what to do when the client is not the user
			client_send( current->client, " shouts: " );
		}

		// send message
		client_send( current->client, (char *)response);
		
		next = next_linked_item( next );
		
		// next
		user_unlock( current );
		
	}
	
	return NULL;
}


/* ------------------------------------------------
 * Actions to move around
 */

/**
 * Go to the specified room. Is a master action not directly
 * actionable
 * @param room Room to try and go to
 * @param user User trying to move
 * @param manager The manager we're moving in
 */
void *action_go_to_room( Room room, User user, Management manager )
{
    if( room == NULL ){
        client_send( user->client, "You can't seem to go that way.\n");
        return NULL;
    }
    
    char_set_room( user->parent->character, room );
    
    action_look( NULL, user, manager );
    
    return NULL;
}


/**
 * Go north
 * @param response message following action command
 * @param user User making action request
 * @param manager Global resource manager
 */
void *action_north( const char *response, User user, Management manager)
{
    Room room = char_get_room( user->parent->character );
    Room next = room_get_north( room, manager->rooms );
    action_go_to_room( next, user, manager );
}

/**
 * Go south
 * @param response message following action command
 * @param user User making action request
 * @param manager Global resource manager
 */
void *action_south( const char *response, User user, Management manager)
{
    Room room = char_get_room( user->parent->character );
    Room next = room_get_south( room, manager->rooms );
    action_go_to_room( next, user, manager );
}

/**
 * Go east
 * @param response message following action command
 * @param user User making action request
 * @param manager Global resource manager
 */
void *action_east( const char *response, User user, Management manager)
{
    Room room = char_get_room( user->parent->character );
    Room next = room_get_east( room, manager->rooms );
    action_go_to_room( next, user, manager );
}

/**
 * Go west
 * @param response message following action command
 * @param user User making action request
 * @param manager Global resource manager
 */
void *action_west( const char *response, User user, Management manager)
{
    Room room = char_get_room( user->parent->character );
    Room next = room_get_west( room, manager->rooms );
    action_go_to_room( next, user, manager );
}

