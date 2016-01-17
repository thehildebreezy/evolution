/*
 * user.h
 *
 *  Created on: Jan 3, 2016
 *      Author: thor
 */

#ifndef INC_MOB_H_
#define INC_MOB_H_

/** defines type of mobs in the struct */
typedef enum mob_type_enum {
    MOB_TYPE_MOB,
    MOB_TYPE_USER,
    MOB_TYPE_NPC,
    MOB_TYPE_SHOP,
    MOB_TYPE_IMMORTAL
} MOB_TYPE;

// this is the parent for users and npcs and other mobs
typedef struct mob_struct {
    MOB_TYPE type;
    void *mob;
} * Mob;

Mob new_mob( MOB_TYPE, void * );

void destroy_mob( Mob );

#endif

