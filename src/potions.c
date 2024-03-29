#include <assert.h>
#include <stdio.h>

#include "constants.h"
#include "defs.h"

void red_potion(Character *c, bool isGreater) {
	const char HEAL_VAL = (isGreater) ? 5 : 3;
	printf("%s drinks the %s.\n", c->name, ITEM_AND_SPELL_NAMES[c->potionSlot]);
	sleep_ms(SLEEP_DURATION);
	if(c->health + HEAL_VAL >= c->totalHealth) {
		c->health = c->totalHealth;
		printf("%s is now full health.\n", c->name);
	} else {
		c->health += HEAL_VAL;
		printf("%s has healed %d health.\n", c->name, HEAL_VAL);
	}
}

void blue_potion(Character *c, bool isGreater) {
	const char MANA_VAL = (isGreater) ? 5 : 3;
	printf("%s drinks the %s.\n", c->name, ITEM_AND_SPELL_NAMES[c->potionSlot]);
	sleep_ms(SLEEP_DURATION);
	if(c->mana + MANA_VAL >= c->totalMana) {
		c->mana = c->totalMana;
		printf("%s is now full mana.\n", c->name);
	} else {
		c->mana += MANA_VAL;
		printf("%s has restored %d mana.\n", c->name, MANA_VAL);
	}
}

void panacea(Character *c) {
	printf("%s drinks the %s. It tastes incredible.\n", c->name, ITEM_AND_SPELL_NAMES[c->potionSlot]);
	sleep_ms(SLEEP_DURATION);
	c->health = c->totalHealth;
	c->mana = c->totalMana;
	printf("Health and mana restored to full.\n");
}

/** Use potion in potionSlot */
void usePotion(Character *c, bool isInCombat) {
	assert(!c->isMonster);
	switch(c->potionSlot) {
		case NOTHING:
			printf("There is no potion in inventory!\n");
			return; //Don't end turn here
		case RED_POTION:
			red_potion(c, false);
			break;
		case GREATER_RED_POTION:
			red_potion(c, true);
			break;
		case BLUE_POTION:
			blue_potion(c, false);
			break;
		case GREATER_BLUE_POTION:
			blue_potion(c, true);
			break;
		case PANACEA:
			panacea(c);
			break;
		default:
			printf("The item had no effect. Must have been a dud!\n");
			//printf("\n\n%d\n\n\n", c->potionSlot);
			break;
	}
	c->potionSlot = NOTHING;
	if(isInCombat) c->isTurn = false;
}

/** Asks if player wants to drink potion outside of combat. */
void drink_potion(Character *c) {
	eprintf("%s looks around and sees no enemies; safety, for the time being.", c->name);
	if(!c->potionSlot) {
		eprintf("It would be the perfect time to drink a potion; unfortunately, %s's potion slot is empty!", c->name);
		return;
	}
	printf("%s currently has %d/%d health and %d/%d mana.\n", c->name, c->health, c->totalHealth, c->mana, c->totalMana);
	printf("Drink the %s in inventory while out of combat?", ITEM_AND_SPELL_NAMES[c->potionSlot]);
	bool isYes = yes_or_no("\n");
	if(isYes) {
		usePotion(c, false);
	} else {
		printf("Ok then.\n");
	}
}
