#include "constants.h"
#include "defs.h"

void fireball(Character *caster, Character *c) {
	printf("%s casts fireball!\n", caster->name);
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
	//@TODO
}

/* One in a hundred chance of sheep exploading, dealing SHEEP_DAMAGE damage. */
void summon_sheep(Character *caster, Character *c) {
	char sheep_explosion = rand() % 100;
	//char sheep_explosion = 0;
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
		printf("It's just a regular sheep; it chews up a nearby painting or rug before walking away.\n");
	}
}

Effect sacrificial_brand(Character *caster, Character *c) {
	//@TODO
	
	return BRAND_ACTIVE;
}

Effect frost_resonance(Character *caster, Character *c) {
	//@TODO
	printf("%s forms a frosty mist in the air, which surrounds %s.\n", caster->name, c->name);
	sleep_ms(SLEEP_DURATION);
	const char FROST_RESONANCE_DAMAGE = 2;
	const char FROST_RESONANCE_MANA = 3;
	printf("%s takes %d damages, and freezes for a turn from the cold!\n", c->name, FROST_RESONANCE_DAMAGE);
	return STUN;
}

/** Cast */
Effect castSpell(Character *c, Character *m) {
	assert(!c->isMonster);
	char isMagicUser = 0;
	char firstSpell = 0; /* used if(isMagicUser == 1) */
	for(int i = 0; i < SPELLS_IN_GAME; i++) {
		if(c->knowSpell[i]) {
			isMagicUser++;
			firstSpell = i + 1; /* corresponds to value of spell in the enum */
		}
	}
	/* Need to check knowSpell during input too, make sure user doesn't guess a spell they don't know */
	if(isMagicUser == 1) { // cast whatever 1 spell the player knows
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
		printf("What spell to cast?\n");
		if(c->knowSpell[0]) { // FIREBALL
			printf("Fireball(f)? ");
		} if(c->knowSpell[1]) { // LIGHTNING_STAKE
			printf("Lightning Stake(L)? ");
		} if(c->knowSpell[2]) { // SUMMON_SHEEP
			printf("Summon Sheep(s)? ");
		} if(c->knowSpell[3]) { // SACRIFICIAL_BRAND
			printf("Sacrificial Brand(b)? ");
		} if(c->knowSpell[4]) { // FROST_RESONANCE
			printf("Frost Resonance(r)? ");
		}
		printf("\n");
		char input[MAX_INPUT_LENGTH];
		for(;;) {
			getInput(input, ">> ");
			if(c->knowSpell[0] && (case_compare(input, "Fireball") == 0 || case_compare(input, "f") == 0)) {
				fireball(c, m);
				break;
			} else if(c->knowSpell[1] && (case_compare(input, "Lightning Stake") == 0 || case_compare(input, "L") == 0)) {
				lightning_stake(c, m);
				break;
			} else if(c->knowSpell[2] && (case_compare(input, "Summon Sheep") == 0 || case_compare(input, "s") == 0)) {
				summon_sheep(c, m);
				break;
			} else if(c->knowSpell[3] && (case_compare(input, "Sacrificial Brand") == 0 || case_compare(input, "b") == 0)) {
				sacrificial_brand(c, m);
				break;
			} else if(c->knowSpell[4] && (case_compare(input, "Frost Resonance") == 0 || case_compare(input, "r") == 0)) {
				frost_resonance(c, m);
				break;
			} else {
				printf("Invalid input.\n");
			}
		}
	} else {
		printf("%s tries to cast magic, but doesn't know how. %s chuckles.\n", c->name, m->name);
		sleep_ms(SLEEP_DURATION);
	}
	c->isTurn = false;
}
