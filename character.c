#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "constants.h"
#include "defs.h"

/* Notes from character struct (in defs.h):
 * character dead if(health <=0), health can be negative
 * Negative mana not allowed, if not enough mana cannot cast a spell
 * no negative defense, total_damage = attacker's attack - defender's defense
 * if char isMonster == 0, its the player character
 * bool isTurn is only used in player struct, is essentially pointless in monster structs
 * elements of knowSpell are true if character knows that spell, corresponds to order of spells in enum
 */

/** Creates a new character, some characters different than others. */
Character* newCharacter(char message[], Enemy enemy) {
	Character *c = malloc(sizeof(*c));

	c->health = MONSTER_STATS[enemy][HEALTH];
	c->totalHealth = MONSTER_STATS[enemy][HEALTH];
	c->mana = MONSTER_STATS[enemy][MANA];
	c->totalMana = MONSTER_STATS[enemy][MANA];
	c->attack = MONSTER_STATS[enemy][ATTACK];
	c->defense = MONSTER_STATS[enemy][DEFENSE];

	c->itemSlot = NOTHING;
	c->potionSlot = NOTHING;
	for(unsigned char i = 0; i < SPELLS_IN_GAME; i++) c->knowSpell[i] = false;

	c->effect = NONE;
	c->effectDuration = 0;

	c->isMonster = enemy;
	/* Gets player character's name */
	if(enemy == PLAYER) {
		getInput(c->name, "Enter your name: ");
		printf("Your name is " C_BLUE "\'%s\'" C_RESET ". ", c->name);
		bool isYes = yes_or_no("Is this correct?\n");
		while(!isYes) {
			getInput(c->name, "Enter your name: ");
			printf("Your name is " C_BLUE "\'%s\'" C_RESET ". ", c->name);
			isYes = yes_or_no("Is this correct?\n");
		}
		c->isTurn = true; // make sure player character gets first turn
	} else {
		strcpy(c->name, MONSTER_NAMES[enemy]);
		printf("%s appears!\n", c->name);
	}
	return c;
}

/** Increases the stat of the player's choice, should be called whenever a monster is defeated */
void lvlUp(Character *c) {
	assert(!c->isMonster);
	printf("Level up avaliable!\nLevel health(h), mana(m), attack(a), or defense(d)?\n");
	char input[MAX_INPUT_LENGTH];
	for(;;) {
		getInput(input, ">> ");
		if(case_compare(input, "health") == 0 || case_compare(input, "h") == 0) {
			c->totalHealth++; c->health++;
			printf("%s feels healthier.\n", c->name);
			return;
		} else if(case_compare(input, "mana") == 0 || case_compare(input, "m") == 0) {
			c->totalMana++; c->mana++;
			printf("%s feels more intelligent.\n", c->name);
			return;
		} else if(case_compare(input, "attack") == 0 || case_compare(input, "a") == 0) {
			c->attack++;
			printf("%s feels more powerful.\n", c->name);
			return;
		} else if(case_compare(input, "defense") == 0 || case_compare(input, "d") == 0) {
			c->defense++;
			printf("%s feels stronger.\n", c->name);
			return;
		} else {
			printf("Invalid input.\n");
		}
	}
}
