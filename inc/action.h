/*
 * action.h
 *
 *  Created on: Oct 9, 2015
 *      Author: thor
 */

#ifndef INC_ACTION_H_
#define INC_ACTION_H_

#include "user.h"
#include "management.h"


// action structure
typedef struct action_struct {
	void *(*action_func)( const char *, User, Management );
} * Action;


// add actions to the manager
void action_add_to_manager( Management );

// parse the response
int action_parse_response(
		char *response,
		int length,
		User user,
		Management manager );

// create a new action
Action new_action( void *(*action_func)( const char *, User, Management ) );

// look action
void *action_look( const char *, User, Management);

#endif /* INC_ACTION_H_ */