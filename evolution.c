/*
 * evolution.c
 *
 *  Created on: Oct 3, 2015
 *      Author: thor
 */

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "inc/server.h"
#include "inc/client.h"

#include "inc/management.h"
#include "inc/action.h"
#include "inc/environment.h"



// main loop
int main( int argc, char **argv )
{
	printf("Hello world\n");

	Management manager = new_manager();

	// add actions to the manager
	action_add_to_manager( manager );

	Server server = new_server();
	if( start_server( server, 5555 ) < 0 ){
		return 1;
	}

	environment_loop( server, manager );

	close_server( server );
	destroy_server( server );


	destroy_manager( manager );

	return 0;
}

