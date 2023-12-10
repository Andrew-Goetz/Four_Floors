#include <assert.h>
#include <stdio.h>

#include "constants.h"
#include "defs.h"

#define DEFENSE_INCREASE 2
#define MANA_INCREASE 5
#define HEALTH_INCREASE 5

void tears(Character *c) {
	assert(!c->isMonster);
	printf("%s drinks the %s. %s glows warmly.\n", c->name, ITEM_AND_SPELL_NAMES[c->itemSlot], c->name);
	c->effect = TEARS_ACTIVE;
	c->effectDuration = EFFECT_DURATIONS[TEARS_ACTIVE];
	/* Rest of implemented in status_effect_check */
}

/* Increases defense for 3 turns */
void iron_pellet(Character *c) {
	printf("%s swallows the %s, hardening the skin.\n", c->name, ITEM_AND_SPELL_NAMES[c->itemSlot]);
	sleep_ms(SLEEP_DURATION);
	c->buff = DEFENSE_UP;
	c->defense += DEFENSE_INCREASE;
	printf("%s's defense has increased by %d!\n", c->name, DEFENSE_INCREASE);
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
	/* needed if monster becomes able to use this item
	if(user->isMonster)
		user->itemSlot = NOTHING;
	*/
}

/* Stuns enemy for one turn and deals some damage */
void light_vial(Character *user, Character *c) {
	const char LIGHT_VIAL_DAMAGE = (c->isMonster == WRAITH) ? 5 : 1;
	c->health -= LIGHT_VIAL_DAMAGE;
	c->effect = STUN;
	c->effectDuration = EFFECT_DURATIONS[STUN];
	printf("%s throws a %s, blinding and stunning %s for a turn, and dealing %d damage!\n", user->name, ITEM_AND_SPELL_NAMES[user->itemSlot], c->name, LIGHT_VIAL_DAMAGE);
}

/* Increased health and mana for a fight, past normal levels */
void horn(Character *c) {
	printf("%s blows the horn. The air trembles.\n", c->name);
	sleep_ms(SLEEP_DURATION);
	c->buff = HEALTH_AND_MANA_UP;
	c->health += HEALTH_INCREASE;
	c->totalHealth += HEALTH_INCREASE;
	c->mana += MANA_INCREASE;
	c->totalMana += MANA_INCREASE;
	printf("%s's health has increased by %d and %s's mana has increased by %d!\n", c->name, HEALTH_INCREASE, c->name, MANA_INCREASE);
}

/** Use item in itemSlot */
void useItem(Character *c, Character *m) {
	assert(!c->isMonster);
	switch(c->itemSlot) {
		case NOTHING:
			printf("There is no item in inventory!\n");
			return; /* Don't end turn here */
		case TEARS:
			tears(c);
			break;
		case IRON_PELLET:
			iron_pellet(c);
			break;
		case DEMON_FIRE:
			demon_fire(c, m);
			break;
		case LIGHT_VIAL:
			light_vial(c, m);
			break;
		case HORN_OF_SAUL:
			horn(c);
			break;
		default:
			printf("The item had no effect. Must have been a dud!\n");
			break;
	}
	c->itemSlot = NOTHING;
	c->isTurn = false;
}

/** Restores the effects of buffs at end of combat */
void buff_revert(Character *c) {
	switch(c->buff) {
		case DEFENSE_UP:
			c->defense -= DEFENSE_INCREASE;
			printf("%s's defense have reverted to normal.\n", c->name);
			break;

		case HEALTH_AND_MANA_UP:
			c->totalHealth -= HEALTH_INCREASE;
			c->totalMana -= MANA_INCREASE;
			if(c->health >= c->totalHealth)
				c->health = c->totalHealth;
			if(c->mana >= c->totalMana)
				c->mana = c->totalMana;
			printf("%s's health and mana have reverted to normal.\n", c->name);
			break;

		default:
			break;
	}
	c->buff = NONE;
}
