/*
 * stats.h
 *
 *  Created on: Oct 9, 2015
 *      Author: thor
 */

#ifndef INC_STATS_H_
#define INC_STATS_H_

typedef enum stats_enum {
	STAT_POWER = 0,
	STAT_ADAPT,
	STAT_NIMBLE,
	STAT_RES,
	STAT_CUNNING,
	STAT_APPEAL,
	STAT_HEALTH,
	STAT_ENERGY,
	STAT_MOVES

} EVO_STAT;


// stats structure
struct stats_struct {

	// strength
	int power;

	// adaptability/intellect
	int adapt;

	// nimbleness/dexterity
	int nimble;

	// resiliance/constitution
	int res;

	// cunning/wisdom
	int cunning;

	// appeal/charisma
	int appeal;

	// health/ hp
	int health;

	// energy/ mana
	int energy;

	// standard moves
	int moves;

};

typedef struct stats_struct Stats;
typedef struct stats_struct *Status;

#endif /* INC_STATS_H_ */
