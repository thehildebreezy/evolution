
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../inc/mob.h"
#include "../inc/character.h"

/**
 * Craete a new mob to parent the other mob types
 * @param type the type of mob for this
 * @param the data for the mob type (e.g. user)
 * @return new Mob structure
 */
Mob new_mob( MOB_TYPE type, void *mob_data )
{
    Mob mob = (Mob)malloc( sizeof( struct mob_struct ) );
    if( mob == NULL ){
        return NULL;
    }
    
    mob->mob = mob_data;
    mob->type = type;
    
    Character character = new_character( mob );
    if( character == NULL ){
        printf("error creating character for mob\n");
        free( mob );
        return NULL;
    }
    
    mob->character = character;
    
    return mob;
}

/**
 * free the mob data
 * @param mob The mob to free
 */
void destroy_mob( Mob mob )
{
    destroy_character( mob->character );
    free( mob );
}
