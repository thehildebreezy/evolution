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


/** constant array of stats for quick lookup */
extern const char *STAT_NAMES[];

// stats structure
struct stats_struct {

	// strength
	short power;

	// adaptability/intellect
	short adapt;

	// nimbleness/dexterity
	short nimble;

	// resiliance/constitution
	short res;

	// cunning/wisdom
	short cunning;

	// appeal/charisma
	short appeal;

	// health/ hp
	long health;

	// energy/ mana
	long energy;

	// standard moves
	long moves;

};

typedef struct stats_struct Stats;
typedef struct stats_struct *Status;

#endif /* INC_STATS_H_ */
