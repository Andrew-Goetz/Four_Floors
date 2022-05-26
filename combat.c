#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "constants.h"
#include "defs.h"

/** When not the players turn, the monster does something: as of right now, it will always attack */
//TODO: should this be where status effects for monster are checked?
void monsterAction(Character *m, Character *c) {
	assert(m->isMonster && !c->isMonster);
	if(c->isTurn)
		return;
	m->isTurn = false;
	c->isTurn = true;
	sleep_ms(SLEEP_DURATION);
	if(m->effect == STUN)
		return;
	meleeAttack(m, c);
}

/** Ask user to equip item, potion, or spell found.
 *  Pass in player character, particular item that is found, and message to display.
 *  Whether "Item" is a Spell, potion, or Item will be determined in the function.
 *  If item, needs to go in itemSlot; if potion, needs to go in potionSlot, spells stored in knowSpell.
 *  message should have a trailing \n.
 */
void item_or_spell_found(Character *c, Item itemFound, char message[]) {
	assert(!c->isMonster);
	printf("%sPress enter to read its description:", message); pressEnter();
	printf("%s", ITEM_AND_SPELL_DESCRIPTIONS[itemFound]); pressEnter();
	bool isYes;
	switch(itemFound) {
		/* for spells, add to knowSpell, making sure index 0 (NOTHING) is still false */
		case FIREBALL: case LIGHTNING_STAKE: case SUMMON_SHEEP: case SACRIFICIAL_BRAND: case FROST_RESONANCE:
			printf("Learn %s?", ITEM_AND_SPELL_NAMES[itemFound]);
			isYes = yes_or_no("\n");
			if(isYes) {
				c->knowSpell[itemFound] = true;
				assert(c->knowSpell[0] == false);
				printf("After reading the scroll, %s learns how to cast %s.\n", c->name, ITEM_AND_SPELL_NAMES[itemFound]);
			} else {
				printf("Ok then.\n");
			}
			break;
		/* for potions, offer to use item in inventory and take new item */
		case RED_POTION: case GREATER_RED_POTION: case BLUE_POTION:	case GREATER_BLUE_POTION: case PANACEA:
			printf("Add %s to potion inventory?", ITEM_AND_SPELL_NAMES[itemFound]);
			if(!c->potionSlot) {
				isYes = yes_or_no("\n");
			} else {
				printf(" %s currently has %d/%d health and %u/%u mana.\n", c->name, c->health, c->totalHealth, c->mana, c->totalMana);
				printf("%s", ITEM_AND_SPELL_NAMES[c->potionSlot]);
				isYes = yes_or_no(" will be used before being discarded.\n");
			}
			if(isYes) {
				if(c->potionSlot) {
					usePotion(c, false);
				}
				c->potionSlot = itemFound;
				sleep_ms(SLEEP_DURATION);
				printf("%s is now in potion inventory.\n", ITEM_AND_SPELL_NAMES[c->potionSlot]);
			} else {
				if(c->potionSlot) {
					printf("%s remains in potion inventory.\n", ITEM_AND_SPELL_NAMES[c->potionSlot]);
				} else {
					printf("Ok then.\n");
				}
			}
			break;
		/* for items, offer to discard current item in inventory if one exists and pick up found one */
		case TEARS: case IRON_PELLET: case DEMON_FIRE: case LIGHT_VIAL: case HORN_OF_SAUL:
			printf("Add %s to item inventory?", ITEM_AND_SPELL_NAMES[itemFound]);
			if(!c->itemSlot) {
				isYes = yes_or_no("\n");
			} else {
				printf("%s", ITEM_AND_SPELL_NAMES[c->itemSlot]);
				isYes = yes_or_no(" will be discarded.\n");
			}
			if(isYes) {
				if(c->itemSlot) {
					printf("%s is discarded, and ", ITEM_AND_SPELL_NAMES[c->itemSlot]);
				}
				printf("%s is put into item inventory.\n", ITEM_AND_SPELL_NAMES[itemFound]);
				c->itemSlot = itemFound;
			} else {
				if(c->itemSlot) {
					printf("%s remains in item inventory.\n", ITEM_AND_SPELL_NAMES[c->itemSlot]);
				} else {
					printf("Ok then.\n");
				}
			}
			break;
		default:
			printf("Something goes wrong. It must have been an illusion!\n");
	}
}

/* Handles status effects */
/* For now, applying a new status effect overwrite current status effect */
//TODO maybe remove turn_number argument?? should be handled fine in combat_sequence
void status_effect_check(Character *c) {
	if(c->effectDuration == 0)
		c->effect = NONE;
	else
		c->effectDuration -= 1;
	switch(c->effect) {
		case NONE:
			return;
		case STUN:
			c->isTurn = false;
			printf("%s is stunned and loses a turn!\n", c->name);
			break;
		case POISON:
			if(c->health > 1) {
				c->health -= 1;
				printf("%s takes 1 damage from poison!\n", c->name);
			} else {
				printf("%s can take no more damage from poison!\n", c->name);
				c->effectDuration = 0;
			}
			break;
		case DRAIN:
			//@TODO
			break;
		case DEFENSE_UP:
			//@TODO
			break;
		case ATTACK_AND_HEALTH_UP:
			//@TODO
			break;
		case TEARS_ACTIVE:
			if(c->health <= 0) {
				c->health = c->totalHealth;
				printf("The tears of a forgotten hero grant your wish. %s takes fatal damage, but has health restored to full!\n", c->name);
			} else {
				printf("The warm glow fades away, the hero's wish unfulfilled.\n");
			}
			break;
		case BRAND_ACTIVE:
			//@TODO
			break;
	}
}

/** Function called once each level when combat is in progress.
 *  c is the player character, m is the monster, and levelUpNumber is the 
 *	number of times the lvlUp function will be called when m is defeated.
 *  Also keeps track of turn number so that status effects can be applied properly.
 */
void combat_sequence(Character *c, Character *m, unsigned char levelUpNumber) {
	assert(!c->isMonster && m->isMonster);
	unsigned char turn_number = 0;
	unsigned char status_effect_count = 0;
	bool isTurnChanged;
	for(;;) {
		/* Player turn */
		status_effect_check(c);
		if(c->health <= 0) { // status_effect_check can alter c->health
			printf("%s has been defeated!\n", c->name);
			free(m); free(c); exit(0);
		}
		actions(c, m);
		if(m->health <= 0) {
			sleep_ms(SLEEP_DURATION);
			printf("VICTORY!\n");
			sleep_ms(SLEEP_DURATION);
			for(; levelUpNumber != 0; levelUpNumber--) {
				lvlUp(c);
			}
			c->isTurn = true;
			break;
		}
		isTurnChanged = c->isTurn;
		/* Monster turn */
		if(!c->isTurn) {
			status_effect_check(m);
			monsterAction(m, c);
		}
		/* want to make sure turn_number not incremented on help or other non-isTurn-changing instructions */
		if(isTurnChanged != c->isTurn && c->health != 0) {
			if(turn_number == 255) {
				printf("After so long fighting, %s collapses in exhaustion. Defeat!\n", c->name);
				free(m); free(c); exit(0);
			} else if(turn_number == 252) {
				printf("%s cannot go on much longer! End this fight before exhaustion overtakes %s!\n",
						c->name, c->name);
			}
			turn_number++;
			//printf("\n%d\n\n", turn_number);
		}
	}
	free(m);
}
