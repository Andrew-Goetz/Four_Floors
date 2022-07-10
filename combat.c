#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "constants.h"
#include "defs.h"

/** Special heavy attack used by golem which stuns player a turn */
void golem_slam(Character *m, Character *c) {
	assert(m->isMonster == GOLEM && !c->isMonster);
	printf("%s attacks %s with a devestating, overhead slam!\n", m->name, c->name);
	sleep_ms(SLEEP_DURATION);
	if(brand_check(m, c)) return;
	char effectiveDamage = m->attack+2 - c->defense;
	damage_check(c, effectiveDamage);
	c->effect = STUN;
	c->effectDuration = EFFECT_DURATIONS[STUN];
}

/** VAMPIRE_LORD attack variant (unparriable) */
/* Deals m->attack+1 damage */
void vampire_thrust(Character *m, Character *c) {
	assert(m->isMonster == VAMPIRE_LORD && !c->isMonster);
	printf("%s executes a lightning-quick thrust, blowing past %s's defenses!\n", m->name, c->name);
	sleep_ms(SLEEP_DURATION);
	if(brand_check(m, c)) return;
	char effectiveDamage = m->attack+1 - c->defense;
	damage_check(c, effectiveDamage);
}

/** VAMPIRE_LORD attack variant */
/* Deals m->attack+2 damage */
void vampire_slash(Character *m, Character *c) {
	assert(m->isMonster == VAMPIRE_LORD && !c->isMonster);
	printf("%s performs a broad, slashing attack on %s!\n", m->name, c->name);
	sleep_ms(SLEEP_DURATION);
	if(brand_check(m, c)) return;
	if(parry_check(m, c)) return;
	char effectiveDamage = m->attack+2 - c->defense;
	damage_check(c, effectiveDamage);
}

/** VAMPIRE_LORD attack variant (first attack unparriable) */
/* Deals 2 damage (ignores defense) on first hit, then m->attack damage on a second hit (doesn't ignore defense) */
void vampire_combo(Character *m, Character *c) {
	assert(m->isMonster == VAMPIRE_LORD && !c->isMonster);
	printf("%s jabs %s!\n", m->name, c->name);
	sleep_ms(SLEEP_DURATION);
	if(brand_check(m, c)) return;
	damage_check(c, 2);

	printf("But %s is not done yet! %s backsteps, then slides forward for another strike at %s!\n", m->name, m->name, c->name);
	sleep_ms(SLEEP_DURATION);
	if(parry_check(m, c)) return;
	char effectiveDamage = m->attack - c->defense;
	damage_check(c, effectiveDamage);
}

/** Special vampire attack which drains player of life energy, giving vampire back health */
void blood_reap(Character *m, Character *c) {
	assert(m->isMonster == VAMPIRE_LORD && !c->isMonster);
	printf("%s teleports behind %s bites into %s's neck!\n", m->name, c->name, c->name);
	sleep_ms(SLEEP_DURATION);
	if(brand_check(m, c)) return;
	char effectiveDamage = m->attack - c->defense;
	damage_check(c, effectiveDamage);
	if(effectiveDamage > 0) {
		sleep_ms(SLEEP_DURATION);
		if(m->totalHealth < m->health + effectiveDamage) {
			printf("%s heals for %d health!\n", m->name, effectiveDamage);
			m->health += effectiveDamage;
		}
		else {
			printf("%s heals to full health!\n", m->name);
			m->health = m->totalHealth;
		}
	}
}

/** When not the players turn, the monster performs an action.
 *  Default is simply attacking.
 *  Several monsters will have different abilities/spells, casted at random.
 */
void monsterAction(Character *m, Character *c) {
	assert(m->isMonster && !c->isMonster);
	if(c->isTurn)
		return;
	m->isTurn = false;
	c->isTurn = true;
	sleep_ms(SLEEP_DURATION);
	if(m->effect == STUN)
		return;
	int r = rand() % 100;
	printf("\nDEBUG: r\%14=%d\n\n", r%14);
	switch(m->isMonster) {
		case WRAITH:
			/* TODO: maybe some attack that adds DRAIN affliction? */
			meleeAttack(m, c);
			break;
		case MAD_WIZARD:
			switch(r % 4) { /* We fall through to default if not enough mana */
				case 0:
					if(m->mana >= SPELL_COSTS[FIREBALL]) {
						fireball(m, c);
						break;
					}
				case 1:
					if(m->mana >= SPELL_COSTS[LIGHTNING_STAKE]) {
						lightning_stake(m, c);
						break;
					}
				case 2:
					if(m->mana >= SPELL_COSTS[FROST_RESONANCE]) {
						frost_resonance(m, c);
						break;
					}
				default:
					meleeAttack(m, c);
					break;
			} /* end switch(r % 4) */
			break;
		case GOLEM:
			if(r % 5 == 0) {
				golem_slam(m, c);
			} else {
				meleeAttack(m, c);
			}
			break;
		case VAMPIRE_LORD:
			switch(r % 14) {
				case 0: case 1: case 2:
					vampire_thrust(m, c);
					break;
				case 3: case 4: case 5:
					vampire_combo(m, c);
					break;
				case 6: case 7:
					parry(m);
					break;
				case 12: case 13:
					if(m->totalHealth - m->health >= m->attack) {
						blood_reap(m, c);
						break;
					} /* Fall through to default if a heal isn't needed */
				default:
					vampire_slash(m, c);
					break;
			} /* end switch(r % 12) */
			break;
		default:
			meleeAttack(m, c);
			break;
	} /* end switch(m->isMonster) */
}

/** Ask user to equip item, potion, or spell found.
 *  Pass in player character, particular item that is found, and message to display.
 *  Whether "Item" is a Spell, potion, or Item will be determined in the function.
 *  If item, needs to go in itemSlot; if potion, needs to go in potionSlot, spells stored in knowSpell.
 *  message should have a trailing \n.
 */
void item_or_spell_found(Character *c, Item itemFound, char message[]) {
	assert(!c->isMonster);
	eprintf("%sPress enter to read its description:", message);
	eprintf("%s", ITEM_AND_SPELL_DESCRIPTIONS[itemFound]);
	bool isYes;
	switch(itemFound) {
		/* for spells, add to knowSpell */
		case FIREBALL: case LIGHTNING_STAKE: case SUMMON_SHEEP: case SACRIFICIAL_BRAND: case FROST_RESONANCE:
			/* Don't prompt to learn spell if c already knows spell */
			if(c->knowSpell[itemFound] == true) {
				printf("%s already knows how to cast %s!\n", c->name, ITEM_AND_SPELL_NAMES[itemFound]);
				sleep_ms(SLEEP_DURATION);
				return;
			}
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
				printf("%s is now in potion inventory.\n", ITEM_AND_SPELL_NAMES[c->potionSlot]);
			} else {
				if(c->potionSlot) {
					printf("%s remains in potion inventory.\n", ITEM_AND_SPELL_NAMES[c->potionSlot]);
				} else {
					printf("Ok then.\n");
				}
			}
			sleep_ms(SLEEP_DURATION);
			break;
		/* for items, offer to discard current item in inventory if one exists and pick up found one */
		case TEARS: case IRON_PELLET: case DEMON_FIRE: case LIGHT_VIAL: case HORN_OF_SAUL:
			printf("Add %s to item inventory?", ITEM_AND_SPELL_NAMES[itemFound]);
			if(!c->itemSlot) {
				isYes = yes_or_no("\n");
			} else {
				printf(" %s", ITEM_AND_SPELL_NAMES[c->itemSlot]);
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
void status_effect_check(Character *c) {
	if(c->effect == HEALTH_AND_MANA_UP || c->effect == DEFENSE_UP)
		return;
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
			/* TODO: Not yet implemented */
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
		default:
			break;
	} /* end switch(c->effect) */
}

/* Checks if monster has died during combat */
bool check_monster_health(Character *c, Character *m, unsigned char levelUpNumber) {
	if(m->health <= 0) {
		sleep_ms(SLEEP_DURATION);
		printf(C_PURPLE "VICTORY!\n" C_RESET);
		sleep_ms(SLEEP_DURATION);
		for(; levelUpNumber != 0; levelUpNumber--) {
			lvlUp(c);
		}
		buff_revert(c);
		c->isTurn = true;
		free(m);
		return true;
	}
	return false;
}

/* Checks if player has died during combat */
void check_player_health(Character *c, Character *m) {
	if(c->health <= 0) { /* status_effect_check can alter c->health */
		printf("%s has been defeated!\n", c->name);
		free(m); free(c); exit(0);
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
		check_player_health(c, m);
		actions(c, m);
		if(check_monster_health(c, m, levelUpNumber))
			break;
		isTurnChanged = c->isTurn;
		/* Monster turn */
		if(!c->isTurn) {
			status_effect_check(m);
			monsterAction(m, c);
			if(check_monster_health(c, m, levelUpNumber))
				break;
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
}
