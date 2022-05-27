#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "constants.h"
#include "defs.h"

void fireball(Character *caster, Character *c) {
	printf("%s casts fireball!\n", caster->name);
	assert(caster->mana != 0);
	caster->mana -= 1;
	sleep_ms(SLEEP_DURATION);
	if(c->isMonster == KILLER_PLANT) {
		const char FIREBALL_DAMAGE = 7;
		c->health -= FIREBALL_DAMAGE;
		printf("%s contorts in intense pain, taking %d damage!\n", c->name, FIREBALL_DAMAGE);
	} else {
		const char FIREBALL_DAMAGE = 3;
		c->health -= FIREBALL_DAMAGE;
		printf("%s burns from the flames, taking %d damage!\n", c->name, FIREBALL_DAMAGE);
	}
}

void lightning_stake(Character *caster, Character *c) {
	printf("%s casts lightning stake!\n", caster->name);
	caster->mana -= 1;
	c->effect = STUN;
	c->effectDuration = 1;
}

/* One in a hundred chance of sheep exploading, dealing SHEEP_DAMAGE damage. */
void summon_sheep(Character *caster, Character *c) {
	caster->mana -= 1;
	const unsigned char sheep_explosion = rand() % 100;
	printf("%s summons a sheep!\n", caster->name);
	sleep_ms(SLEEP_DURATION);
	if(sheep_explosion == 0) { //sheep explodes
		const char SHEEP_DAMAGE = 10;
		c->health -= SHEEP_DAMAGE;
		printf("The sheep walks towards %s and exploads, dealing %d damage!\n", c->name, SHEEP_DAMAGE);
	} else if(sheep_explosion == 1) { //different dialogue options based on random number
		printf("The sheep seems to smoke up a bit as if about to expload, but then walks away normally.\n");
	} else if(sheep_explosion == 69) {
		printf("The sheep opens it's mouth and says \"Nice\" in a heavy British accent before walking away.\n");
	} else if(sheep_explosion == 99) {
		printf("The sheep sprouts wings and flies off into the distance, leaving the mortal world behind.\n");
	} else {
		printf("It's just a regular sheep; it chews up a nearby rug before walking away.\n");
	}
}

void sacrificial_brand(Character *caster, Character *c) {
	/* Most logic for this function has to be in meleeAttack */
	printf("%s engraves a holy rune upon the skin.", caster->name);
	caster->health = 1;
	caster->effect = BRAND_ACTIVE;
	caster->effectDuration = EFFECT_DURATIONS[BRAND_ACTIVE];
	sleep_ms(SLEEP_DURATION);
	printf("%s bleeds out, reducing health to %d!", caster->name, caster->health);
}

void frost_resonance(Character *caster, Character *c) {
	printf("%s forms a frosty mist in the air, which surrounds %s.\n", caster->name, c->name);
	sleep_ms(SLEEP_DURATION);
	const char FROST_RESONANCE_DAMAGE = 2;
	const char FROST_RESONANCE_MANA = 3;
	caster->mana -= FROST_RESONANCE_MANA;
	c->health -= FROST_RESONANCE_DAMAGE;
	c->effect = STUN;
	c->effectDuration = 1;
	printf("%s takes %d damages, and freezes for a turn from the cold!\n", c->name, FROST_RESONANCE_DAMAGE);
}

void castSpell(Character *c, Character *m) {
	//@TODO check to ensure c has enough mana to cast requested spell
	assert(!c->isMonster);
	char isMagicUser = 0;
	char firstSpell = 0; /* used if(isMagicUser == 1) */
	for(int i = 1; i < SPELLS_IN_GAME; i++) {
		//printf("DEBUG: %d\n", c->knowSpell[i]);
		if(c->knowSpell[i]) {
			//printf("DEBUG: %d\n", i);
			isMagicUser++;
			firstSpell = i; /* corresponds to value of spell in the enum */
		}
	}
	//printf("DEBUG: isMagicUser = %d", isMagicUser);
	if(isMagicUser == 1) { // cast whatever 1 spell the player knows
		//printf("DEBUG: firstSpell: %d, FIREBALL: %d\n", firstSpell, FIREBALL);
		switch(firstSpell) {
			case FIREBALL:
				fireball(c, m);
				break;
			case LIGHTNING_STAKE:
				lightning_stake(c, m);
				break;
			case SUMMON_SHEEP:
				summon_sheep(c, m);
				break;
			case SACRIFICIAL_BRAND:
				sacrificial_brand(c, m);
				break;
			case FROST_RESONANCE:
				frost_resonance(c, m);
				break;
			default:
				printf("Something goes wrong and %s fails to cast the spell!", c->name);
				break;
		}
	} else if(isMagicUser > 1) { // output spells the player knows and have them pick one
		printf("What spell to cast? (type 'none(n)' to cancel)\n");
		if(c->knowSpell[1]) { // FIREBALL
			printf("Fireball(f)? ");
		} if(c->knowSpell[2]) { // LIGHTNING_STAKE
			printf("Lightning Stake(L)? ");
		} if(c->knowSpell[3]) { // SUMMON_SHEEP
			printf("Summon Sheep(s)? ");
		} if(c->knowSpell[4]) { // SACRIFICIAL_BRAND
			printf("Sacrificial Brand(b)? ");
		} if(c->knowSpell[5]) { // FROST_RESONANCE
			printf("Frost Resonance(r)?");
		}
		printf("\n");
		char input[MAX_INPUT_LENGTH];
		/* Need to check knowSpell during input too, make sure user doesn't guess a spell they don't know */
		for(;;) {
			getInput(input, ">> ");
			if(c->knowSpell[1] && (case_compare(input, ITEM_AND_SPELL_NAMES[FIREBALL]) == 0 || case_compare(input, "f") == 0)) {
				fireball(c, m);
				break;
			} else if(c->knowSpell[2] && (case_compare(input, ITEM_AND_SPELL_NAMES[LIGHTNING_STAKE]) == 0 || case_compare(input, "L") == 0)) {
				lightning_stake(c, m);
				break;
			} else if(c->knowSpell[3] && (case_compare(input, ITEM_AND_SPELL_NAMES[SUMMON_SHEEP]) == 0 || case_compare(input, "s") == 0)) {
				summon_sheep(c, m);
				break;
			} else if(c->knowSpell[4] && (case_compare(input, ITEM_AND_SPELL_NAMES[SACRIFICIAL_BRAND]) == 0 || case_compare(input, "b") == 0)) {
				sacrificial_brand(c, m);
				break;
			} else if(c->knowSpell[5] && (case_compare(input, ITEM_AND_SPELL_NAMES[FROST_RESONANCE]) == 0 || case_compare(input, "r") == 0)) {
				frost_resonance(c, m);
				break;
			}  else if(case_compare(input, "none") == 0 || case_compare(input, "n") == 0) {
				printf("Ok then.\n");
				return; // don't end turn here
			}
			else {
				printf("Invalid input.\n");
			}
		}
	} else {
		printf("%s tries to cast magic, but doesn't know how. %s chuckles.\n", c->name, m->name);
		sleep_ms(SLEEP_DURATION);
	}
	c->isTurn = false;
}
