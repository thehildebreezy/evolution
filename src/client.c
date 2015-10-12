/*
 * client.c
 *
 *  Created on: Oct 3, 2015
 *      Author: thor
 */

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

#include "../inc/client.h"


/**
 * Creates a new client for connections
 * @return a new allocated client structure
 */
Client new_client() {
	Client client;

	client = malloc( sizeof(struct client_struct) );

	if( client == NULL ){
		perror("Client malloc");
		return NULL;
	} else {
	
	    memset( client, 0, sizeof( struct client_struct ) );
	
		client->status = CLIENT_STARTING;

		return client;
	}
}

/**
 * Frees the client structure from mem allocation
 * @param client Client to free
 */
void destroy_client( Client client ) {
	free( client );
}


/**
 * Sets client status to ok
 * @param client Client to set ok
 */
void client_ok( Client client ) {
	client->status = CLIENT_OK;
}

/**
 * Closes the client connection
 * @param client Client to close
 */
void close_client( Client client) {
	if( client != NULL && client->status == CLIENT_OK ){
		client->status = CLIENT_STOPPED;
		close( client->socket );
	}
}


/**
 * Sends a message tot he client
 * @param client Client struct to send to
 * @param message Message to send
 * @return number of bytes transmitted or -1 on fail
 */
int client_send( Client client, char *message ) {


	if( client->status != CLIENT_OK ) return -1;


	int n = write( client->socket, message, strlen(message) );

	if( n < 0 ){
		perror("Error sending to client");
		return -1;
	}

	return n;
}

/**
 * Receives a message from the client
 * @param client Client struct to receive from
 * @param message Pointer to unallocated message buffer to receive into
 * @param number of bytes received  or -1 on fail
 */
int client_recv( Client client, char **message ) {

	if( client->status != CLIENT_OK ) return -1;

	unsigned long message_size = 1024;

	*message = (char *) malloc( message_size );

	int n = read( client->socket, *message, message_size );
	if( errno != EWOULDBLOCK && errno != EAGAIN &&  n < 0 ){
		perror("Error recv from client");
		return -1;
	}

	return n;
}

