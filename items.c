#include "constants.h"
#include "defs.h"

Effect tears(Character *c) {
	printf("%s drinks the %s. %s glows warmly.\n", c->name, ITEM_AND_SPELL_NAMES[c->itemSlot], c->name);
	return TEARS_ACTIVE;
	//@TODO needs to toggle some status for the character that only lasts a turn
}

Effect iron_pellet(Character *c) {
	printf("%s swallows the %s, hardening the skin.\n", c->name, ITEM_AND_SPELL_NAMES[c->itemSlot]);
	const char IRON_PELLET_DEFENSE_INCREASE = 3;
	return DEFENSE_UP;
	//@TODO increase defense a decent amount for 3 turns (current turn counts as a turn
}

void demon_fire(Character *user, Character *c) {
	printf("%s throws a %s at %s, making the room erupt in flames.\n", user->name, ITEM_AND_SPELL_NAMES[user->itemSlot], c->name);
	sleep_ms(SLEEP_DURATION);
	if(c->isMonster == KILLER_PLANT) {
		const char DEMON_FIRE_DAMAGE = c->totalHealth; //gets oneshot
		c->health -= DEMON_FIRE_DAMAGE;
		printf("%s burns in the fires of hell, draining it of all life energy.\n", c->name);
	} else {
		const char DEMON_FIRE_DAMAGE = 5;
		c->health -= DEMON_FIRE_DAMAGE;
		printf("%s is severely burned, taking %d damage!\n", c->name, DEMON_FIRE_DAMAGE);
	}
	if(user->isMonster) {
		user->itemSlot = NOTHING;
	}
}

/* Stuns enemy for one turn */
Effect light_vial(Character *user, Character *c) {
	assert(!user->isMonster);
	if(c->isMonster == WRAITH) {
		const char LIGHT_VIAL_DAMAGE = 5;
	} else {
		printf("%s throws a %s, blinding %s.\n", user->name, ITEM_AND_SPELL_NAMES[user->itemSlot], c->name);
	}
	return STUN;
}

void horn(Character *user, Character *c) {
	assert(!user->isMonster);
}

/** Use item in itemSlot */
Effect useItem(Character *c, Character *m) {
	assert(!c->isMonster);
	Effect retEffect = NONE;
	switch(c->itemSlot) {
		case NOTHING:
			printf("There is no item in inventory!\n");
			return NONE; // Don't end turn here
		case TEARS:
			retEffect = tears(c);
			break;
		case IRON_PELLET:
			retEffect = iron_pellet(c);
			break;
		case DEMON_FIRE:
			demon_fire(c, m);
			break;
		case LIGHT_VIAL:
			retEffect = light_vial(c, m);
			break;
		case HORN_OF_SAUL:
			horn(c, m);
			break;
		default:
			printf("The item had no effect. Must have been a dud!\n");
			break;
	}
	c->itemSlot = NOTHING;
	c->isTurn = false;
	return retEffect;
}
