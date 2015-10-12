#ifndef EV_SERVER_H
#define EV_SERVER_H

#include <netinet/in.h>
#include "../inc/client.h"

/**
 * server.h
 * Server header provides method declaration for server control
 * methods.
 * @author Th0r
 */


/* Enums
 * =====
 */

typedef enum server_status {
	SERVER_STARTING,
	SERVER_OK,
	SERVER_STOPPED,
	SERVER_ERROR,
	SERVER_UNK
} ServerStatus;



/* Structs
 * =======
 */

/**
 * Server structure provides a structure to manage the server
 */
typedef struct server_struct {
	/** socket file */
	int socket;

	/** port to listen on */
	int port_number;

	/** number of users connected */
	unsigned long users_connected;

	/** status of the server */
	ServerStatus status;

	/** server address for socket */
	struct sockaddr_in address;

} *Server;


/* Methods
 * =======
 */

// creates a new server structure
Server new_server();

// destroys the server after use
void destroy_server( Server );

// closes server connection
void close_server( Server server );

// starts the server listening for connections on port
int start_server( Server, int port );

// stops the server
int stop_server( Server );

// listens and accepts users from the server
Client accept_client( Server );

#endif
