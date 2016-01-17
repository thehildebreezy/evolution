/*
 * character.h
 *
 *  Created on: Oct 3, 2015
 *      Author: thor
 */

#ifndef INC_CHARACTER_H_
#define INC_CHARACTER_H_

#include <pthread.h>


// forward declare user
typedef struct user_struct *User;

// local includes
#include "client.h"
#include "user.h"
#include "stats.h"
#include "room.h"

// Structure
typedef struct character_struct {

	// character name
	char name[32];

	// character location
	// unsigned long location;
	Room room;

	// character stats
	Stats stats;
	

	// if it has a user it will be associated here
	//User user;
	void *parent;

	// mutex
	pthread_mutex_t mutex;

} * Character;


/* -------------------
 * Methods
 */

Character new_character( void * );
void destroy_character( Character );

// get character room
Room char_get_room( Character );


// set character room
void char_set_room( Character, Room );


// mutex funcs
void char_lock( Character );
void char_unlock( Character );


#endif /* INC_CHARACTER_H_ */
