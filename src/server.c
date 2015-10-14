/**
 * server.c
 * Provides methods for server
 */

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <netinet/in.h>
#include <string.h>
#include <errno.h>
#include <sys/time.h>
#include <fcntl.h>

#include "../inc/server.h"
#include "../inc/client.h"

/**
 * Creates a new server to manage connections
 * @return a new allocated server structure
 */
Server new_server() {
	Server server;

	server = malloc( sizeof(struct server_struct) );
	if( server == NULL ){
		perror("Server malloc");
		return NULL;
	} else {

        // initialize
        memset( server, 0, sizeof( struct server_struct ) );

		server->status = SERVER_STARTING;
		return server;
	}
}

/**
 * Frees the server structure from mem allocation
 * @param server Server to free
 */
void destroy_server( Server server ) {
	free( server );
}

/**
 * Closes the server connection
 * @param server Server to close
 */
void close_server( Server server ) {
	if( server->status == SERVER_OK ){
		server->status = SERVER_STOPPED;
		close( server->socket );
	}
}

/**
 * Starts the server and starts listening on the port given
 * @param server Structure of server to start
 * @param port Port number to listen on
 * @return 0 on success, -1 on failure
 */
int start_server( Server server, int port ) {


	// open new server socket
	server->socket = socket( AF_INET, SOCK_STREAM, 0);
	if( server->socket < 0 ){
		perror("Error opening server socket");
		return -1;
	}

	if (setsockopt(server->socket, SOL_SOCKET,
			SO_REUSEADDR, &(int){ 1 }, sizeof(int)) < 0)
	    perror("setsockopt(SO_REUSEADDR) failed");


    // make the server non blocking
    int status = fcntl(
        server->socket, 
        F_SETFL, 
        fcntl(server->socket, F_GETFL, 0) | O_NONBLOCK // set non blocking
    );

    // probs won't'
    if (status == -1){
        perror("calling fcntl");
    }

	// clear the address struct
	bzero( (char *) &(server->address), sizeof(server->address) );

	// set up address
	server->port_number = port;

	server->address.sin_family = AF_INET;
	server->address.sin_addr.s_addr = INADDR_ANY;
	server->address.sin_port = htons(server->port_number);

	// bind the host to teha dress
	if( bind(server->socket,
			(struct sockaddr *) &(server->address),
			sizeof(server->address))
		< 0
	){
		perror("Error binding server socket");
		return -1;
	}

	// start listening
	listen( server->socket, 5 ); // 5 connections good for now

	server->status = SERVER_OK;

	return 0;
}

// stops the server
int stop_server( Server server ) {
	return 0;
}

// listens and accepts users from the server
Client server_accept( Server server ) {

	Client client = new_client();
	if( client == NULL ) return NULL;
	
	int clilen = sizeof(client->address);


    while( 1 ){
	    // accept a connection
	    client->socket = accept(server->socket,
			    (struct sockaddr *) &(client->address),
			    &clilen
		    );

	    if( errno != EWOULDBLOCK && client->socket < 0 ){
		    if( server->status != SERVER_STOPPED ){
		        perror("Error accepting Client");
		    } else {
		        printf("No longer accepting clients\n");
		    }
		    
		    destroy_client( client );
		    
		    return NULL;
	    } else if( client->socket >= 0 ) {
	        return client;
	    }
	}

	return client;
}

