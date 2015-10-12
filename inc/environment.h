/*
 * environment.h
 *
 *  Created on: Oct 3, 2015
 *      Author: thor
 */

#ifndef INC_ENVIRONMENT_H_
#define INC_ENVIRONMENT_H_

#include "server.h"
#include "client.h"
#include "management.h"

// thread arg struct
typedef struct environment_struct {

	Client client;

	Management manager;

} * Environment;

// connection management struct
typedef struct connection_struct {
	Server server;
	Management manager;
} * ConnectionThreadStruct;


// create new environment thread args
Environment new_env_arg( Client, Management );


void environment_loop( Server, Management );

void *environment_thread( void * );

void *user_thread( void * );

void *connection_thread( void * );

#endif /* INC_ENVIRONMENT_H_ */
