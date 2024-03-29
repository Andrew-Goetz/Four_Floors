#include <assert.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
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
	c->isMonster = enemy;
	if(c->isMonster == PLAYER) {
		char buf[MAX_INPUT_LENGTH + sizeof(C_BLUE) + sizeof(C_RESET)];
		getInput(buf, "Enter your name: ");
		/* This sprintf is safe, I promise */
		sprintf(c->name, "%s%s%s", C_BLUE, buf, C_RESET);
		printf("Your name is \'%s\'. ", c->name);
		bool isYes = yes_or_no("Is this correct?\n");
		while(!isYes) {
			memset(buf, 0, MAX_INPUT_LENGTH + sizeof(C_BLUE) + sizeof(C_RESET));
			getInput(buf, "Enter your name: ");
			sprintf(c->name, "%s%s%s", C_BLUE, buf, C_RESET);
			printf("Your name is \'%s\'. ", c->name);
			isYes = yes_or_no("Is this correct?\n");
		}
		c->isTurn = true; /* make sure player character gets first turn */
		memset(buf, 0, MAX_INPUT_LENGTH + sizeof(C_BLUE) + sizeof(C_RESET));
		for(;;) {
			printf("Is %s a %sKnight%s (higher stats) or a %sDuelist%s (access to parry skill)?\n", c->name, C_CYAN, C_RESET, C_GREEN, C_RESET);
			getInput(buf, "Enter Knight(k) or Duelist(d): ");
			if(case_compare(buf, "knight") == 0 || case_compare(buf, "k") == 0) {
				printf("%s is now a %sKnight%s.\n", c->name, C_CYAN, C_RESET);
				c->isKnight = true;
				break;
			} else if(case_compare(buf, "duelist") == 0 || case_compare(buf, "d") == 0) {
				printf("%s is now a %sDuelist%s.\n", c->name, C_GREEN, C_RESET);
				c->isKnight = false;
				break;
			} else {
				printf("Invalid input.\n"); /* repeats loop and asks again */
			}
		}
	} else {
		/* This strcpy is safe since we're copying from a buffer of known size */
		strcpy(c->name, MONSTER_NAMES[enemy]);
		printf("%s%s\n", c->name, message);
	}
	c->health = MONSTER_STATS[enemy][HEALTH];
	c->totalHealth = MONSTER_STATS[enemy][HEALTH];
	c->mana = MONSTER_STATS[enemy][MANA];
	c->totalMana = MONSTER_STATS[enemy][MANA];
	c->attack = MONSTER_STATS[enemy][ATTACK];
	c->defense = MONSTER_STATS[enemy][DEFENSE];

	c->itemSlot = NOTHING;
	c->potionSlot = NOTHING;
	for(unsigned char i = 0; i < SPELLS_IN_GAME; i++) 
		c->knowSpell[i] = false;

	c->effect = NONE;
	c->buff = NONE;
	c->effectDuration = 0;
	if(!c->isMonster && !c->isKnight) {
		c->totalHealth--;
		c->health--;
		c->totalMana--;
		c->mana--;
	} else if(c->isMonster == MAD_WIZARD) {
		c->knowSpell[FIREBALL] = true;
		c->knowSpell[LIGHTNING_STAKE] = true;
		c->knowSpell[FROST_RESONANCE] = true;
		c->isKnight = true;
	}
	else {
		c->isKnight = true;
	}
	return c;
}

/** Increases the stat of the player's choice, should be called whenever a monster is defeated */
void lvlUp(Character *c) {
	assert(!c->isMonster);
	printf("Level up available!\nLevel health(h), mana(m), attack(a), or defense(d)?\n");
	char input[MAX_INPUT_LENGTH];
	for(;;) {
		getInput(input, ">> ");
		if(case_compare(input, "health") == 0 || case_compare(input, "h") == 0) {
			c->health++;
			c->totalHealth++;
			printf("%s feels healthier.\n", c->name);
			return;
		} else if(case_compare(input, "mana") == 0 || case_compare(input, "m") == 0) {
			c->mana++;
			c->totalMana++;
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
