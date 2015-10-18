/*
 * environment.c
 *
 *  Created on: Oct 3, 2015
 *      Author: thor
 */

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sched.h>
#include <signal.h>
#include <string.h>
#include <sys/time.h>
#include <fcntl.h>
#include <errno.h>

#include "../inc/environment.h"
#include "../inc/management.h"
#include "../inc/server.h"
#include "../inc/client.h"
#include "../inc/list.h"

#include "../inc/user.h"
#include "../inc/character.h"
#include "../inc/room.h"

#include "../inc/action.h"

/// tick interval in useconds - not used
#define EVOLUTION_TICK_INTERVAL_U 1000000

/// tick interval in seconds - used
#define EVOLUTION_TICK_INTERVAL_S 1

static volatile int cease = 0;



// interrupted
void interrupted( int );

/// TODO Client acceptance thread

/**
 * Creates the main mechanical loop.
 * @param server the server for this environment
 * @param manager where most of our variables come from
 */
void environment_loop( Server server, Management manager ) {

	signal( SIGINT, interrupted );

	// create a struct to pass to the connection accepting thread
	ConnectionThreadStruct conn = malloc(
			sizeof( struct connection_struct )
		);

	conn->server = server;
	conn->manager = manager;

	// start a thread to accept connections
	pthread_t thread;
	int err = pthread_create(
			&thread,
			NULL,
			connection_thread,
			conn);

	// check for error
	if (err != 0) {
		perror("Creating connection thread");
	}

	// TODO spawn real time activity thread


	// time values
	struct timeval last_time;
	gettimeofday(&last_time, NULL);

	// number of quarter ticks since last tick
	unsigned int quarter_ticks = 0;

	// environment loop actual
	while( cease != 1 ){


		/* --------------------------------------------------
		 * Check for ticks
		 */

		// get current time
		struct timeval current_time;
		gettimeofday(&current_time, NULL);

		// difference in usecs, used for quarter tick intervals, if needed
		long int diff = (last_time.tv_usec + 1000000 * last_time.tv_sec) -
				(current_time.tv_usec + 1000000 * current_time.tv_sec);

		int sec_diff = current_time.tv_sec - last_time.tv_sec;
		if( sec_diff >= EVOLUTION_TICK_INTERVAL_S ){
			// do something with the tick


			last_time = current_time;
		} else {

			// give up thread control
			sched_yield();
		}
	}

	// set manager cease flag
	manager->cease = 1;

	// TODO Disconnect all clients

	// wait for connection thread to end
	close_server( server );
	// pthread_kill( thread, SIGINT );
	pthread_join( thread, NULL );

    // free conn arg
    free( conn );

	printf("exiting\n");
}


/**
 * Environment thread responsible for handling other real time activity
 * @param arg Structure to use to process the thread
 */
void *environment_thread( void *arg ) {

	// TODO

	return NULL;
}

/**
 * User thread responsible for handling individual clients
 * @param arg Structure to use to process the thread
 */
void *user_thread( void *arg ) {

	User user = new_user( ((Environment)arg)->client );
	Management manager = ((Environment)arg)->manager;

    // increase thread count
    manager_up_thread( manager, pthread_self() );

    // lock user for now
    user_lock( user );

	    // add the user to the list of users
	    manager_add_user( manager, user );

	    // turn client on
	    client_ok( user->client );

    // ready user for commo
    user_unlock( user );



	/* ---------------------------------------------
	 * Login screen
	 */

	// first thing we'll do is push out the login screen
	Room login_room = room_from_file( "core/login" );

	// get login
	int desc_length = 2048;
	char *desc = malloc( desc_length );
	if( desc == NULL ){
	    perror("malloc login description");
	    destroy_room( login_room );
	    return NULL;
	}
	
	// init
	memset( desc, 0, desc_length );
	
    int len = room_get_full_description( 
        login_room,
        user,
        &desc,
        &desc_length );

	// send the login screen
	client_send( user->client, desc );

	// free up the buffer
	if( desc != NULL ){
		free( desc );
	}

    // client_prompt(  )

	// room only exists for this user
	destroy_room( login_room );





	// accept input
	
	// initialize
    int buff_len = 256;
	char *buff = (char *) malloc( buff_len );
	memset( buff, 0, 256 );
	if( buff == NULL ){
	    perror("client thread recv buffer malloc");
	    return NULL;
	}

	// user response loop
	while( manager->cease != 1 ){

		// receive response from client
		int length = client_recv( 
		    user->client, 
		    &buff, 
		    &buff_len );

		// handle response
		if( buff != NULL && length > 0 ){

			// parse the response for actionable text
			action_parse_response( 
			    buff,       // buffer with response
			    length,     // length of response
			    user,       // user making response
			    manager );  // manager

            // reset response
            memset(buff,0,length);

		} else {
		    // nothing going on, yield
		    sched_yield();
		}

	}
	
	free( buff );



	// remove from manager list
	manager_remove_user( manager, user );

    // lock user for this    
	user_lock( user );
	
	// close and destory all structs
	// manually manage this part
	close_client( user->client );
	destroy_client( user->client );
	
	// user gone from manager so safe to unlock a head
	user_unlock( user );
	
	// now destroy
	destroy_user( user );
	
	free( ((Environment)arg) );

    // decrease thread count
    manager_down_thread( manager, pthread_self() );
    pthread_exit( 0 );

	return NULL;
}


/**
 * Accepts incoming connections
 * @param arg Manager structure to pass into new clients
 */
void *connection_thread( void *arg ) {

	ConnectionThreadStruct conn = (ConnectionThreadStruct)arg;

	while( cease != 1 ){
		Client client = server_accept( conn->server );

        if( client == NULL ) continue;

        // make the client non blocking
        int status = fcntl(
            client->socket, 
            F_SETFL, 
            fcntl(client->socket, F_GETFL, 0) | O_NONBLOCK // set non blocking
        );

        // probs won't'
        if (status == -1){
            perror("calling fcntl");
        }


		Environment arg =  new_env_arg( client, conn->manager );

		pthread_t thread;
		// client thread
		int err = pthread_create(
				&thread,
				NULL,
				user_thread,
				arg);

		// check for error
		if (err != 0) {
			perror("Creating thread");
		}

		client->thread = thread;

	}
	
	// wait for threads to clean themselves
	while( manager_thread_count( conn->manager ) > 0 ){
	    // going to wait for threads to clean themselves up
	    sched_yield();
	}
	
	// wait for the last one
    if( conn->manager->last_thread != 0 ){
        printf("joining last thread\n");
        pthread_join( conn->manager->last_thread, NULL );
    }

}

/**
 * Creates a new environemnt thread argument
 * @param client Client information to add to argument
 * @param manager Manager pointer for the entire server
 * @return new Environment arg
 */
Environment new_env_arg( Client client, Management manager ) {
	Environment arg = malloc( sizeof(struct environment_struct) );

	if( arg == NULL ){
		perror("Malloc env arg");
		return NULL;
	}
	
	// initialize
	memset( arg, 0, sizeof( struct environment_struct ) );

	arg->client = client;
	arg->manager = manager;

	return arg;
}


void interrupted( int dummy ) {
	cease = 1;
}

