#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "defs.h"
#include "constants.h"

/** Takes two character, and character attacker and character c.
 *  Checks to make sure arguments are in correct order,
 *  then subtracts c's health by attacker's attack value.
 *  First character (attacker) deals damage to second character (c).
 */
void meleeAttack(Character *attacker, Character *c) {
	printf("%s attacks %s!\n", attacker->name, c->name);
	sleep_ms(SLEEP_DURATION);
	//@TODO check status for BRAND_ACTIVE here
	char effectiveDamage = attacker->attack - c->defense;
	if(effectiveDamage > 0) {
		c->health -= effectiveDamage;
		printf("%s took %u damage!\n", c->name, effectiveDamage);
	} else {
		printf("%s took no damage!\n", c->name);
	}
	if(!attacker->isMonster) attacker->isTurn = false;
}

/* Output status, use enemyStatus for non-player characters */
void status(Character *c) {
	printf("%s's stats:\n\tHealth:%d/%d\n\tMana:%d/%d\n\tAttack:%d\n\tDefense:%d\n",
			c->name, c->health, c->totalHealth, c->mana, c->totalMana, c->attack, c->defense);
	if(c->itemSlot) {
		printf("%s in item slot.\n", ITEM_AND_SPELL_NAMES[c->itemSlot]);
	}
	if(c->potionSlot) {
		printf("%s in potion slot.\n", ITEM_AND_SPELL_NAMES[c->potionSlot]);
	}
	bool anySpells = false;
	for(int i = 1; i < SPELLS_IN_GAME; i++) {
		if(c->knowSpell[i]) {
			printf("%s knows the following spells:\n", c->name);
			break;
		}
	}
	//@TODO check that this works properly, it almost certainly does
	if(c->knowSpell[1]) { // FIREBALL
		printf("\tFireball(f)\n");
	} if(c->knowSpell[2]) { // LIGHTNING_STAKE
		printf("\tLightning Stake(L)\n");
	} if(c->knowSpell[3]) { // SUMMON_SHEEP
		printf("\tSummon Sheep(s)\n");
	} if(c->knowSpell[4]) { // SACRIFICIAL_BRAND
		printf("\tSacrificial Brand(b)\n");
	} if(c->knowSpell[5]) { // FROST_RESONANCE
		printf("\tFrost Resonance(r)\n");
	}
}

/** Output status of enemy character, calls status.
 *  Passes in player character as well to ensure player doesn't lose a turn.
 */
void enemyStatus(Character *c, Character *m) {
	assert(m->isMonster);
	static const char *MONSTER_HINTS[MONSTERS_IN_GAME] = {
		"ERROR\n", /* Character gets no hint */
		"has a large scar across its hairy chest. It must be weak to physical attacks!\n", /* The Beast */
		"seems vulnerable to fire.\n", /* The Killer Plant */
		"is a creature of the night. The sun's rays would prove fatal.\n", /* The Wraith */
		"is a powerful wizard, and magic would have no effect against him.\n", /* The Wizard */
		"is an unthinking creature of destruction; attacking with massive physical blows, defense must be powerful against it!\n", /* The Wizard's Golem */
		"is the ultimate foe. He has no obvious weakness weakness.\n" /* The Vampire Lord */
	};
	status(m);
	printf("%s %s", MONSTER_NAMES[m->isMonster], MONSTER_HINTS[m->isMonster]); //prints out hints/descriptions here
	c->isTurn = true;
}

/** Output help info */
void help(void) {
	printf("Goal: defeat your foes and stay alive, reach the fourth floor:\n"
			"\thelp(h)\t\tlists possible commands\n"
			"\tstatus(s)\tlists out player stats\n"
			"\tenemy(e)\tlists out enemy's stats\n"
			"\tattack(a)\tattack enemy, ending turn\n"
			"\tpotion(p)\tdrink potion, ending turn\n"
			"\titem(i)\t\tuse item, ending turn\n"
			"\tcast(c)\t\tcast spell, ending turn\n"
			"\twait(w)\t\tdo nothing, ending turn\n"
			"\tescape(exit)\tabandon quest and flee\n");
	sleep_ms(SLEEP_DURATION);
	bool isYes = yes_or_no("Output additional game information?\n");
	if(isYes) {
		printf("Additional information:\n"
				"\tHealth is life energy. When health reaches 0, death occurs.\n"
				"\tMana is magic energy, which is expended to cast spells.\n"
				"\tHealth and mana can only be restored using potions.\n"
				"\tDamage dealt is attacker's attack value minus defender's defense value.\n"
				"\tIf a player with 3 attack attacks a monster with 1 defense, 2 damage is dealt.\n"
				"\tSpells and items ignore defense.\n"
				"\tPotions, items, and spells can be found after each battle.\n"
				"\tItems and potions can only be used once, while spells can be used as long as there is enough mana to cast them.\n"
				"\tCertain items, spells, and potions are very effective against certain enemies.\n"
				"\tAt least 1 level up occurs after each battle; this will automatically save the game.\n");
				//"\t\n"
	} else {
		printf("Ok then.\n");
	}
}

/** wait, expends player turn */
void wait(Character *c) {
	assert(!c->isMonster);
	printf("%s does nothing.\n", c->name);
	c->isTurn = false;
}

/** Exit */
void escape(Character *c, Character *m) {
	assert(!c->isMonster);
	printf("%s runs out of the mansion in shame.\n", c->name);
	free(c); free(m);
	exit(0);
}

/** Call when input is required, c must be the player character, m the monster */

void actions(Character *c, Character *m) {
	assert(!c->isMonster);
	char input[MAX_INPUT_LENGTH];
	for(;;) {
		getInput(input, ">> ");
		/* help(h): lists out possible commands and then asks if user wants more in depth information */
		if(case_compare(input, "help") == 0 || case_compare(input, "h") == 0) {
			help();
			break;
		}
		/* status(s): outputs current player status */
		else if(case_compare(input, "status") == 0 || case_compare(input, "s") == 0) {
			status(c);
			break;
		}
		/* enemy(e): outputs non-player-character's status, a help fight him */
		else if(case_compare(input, "enemy") == 0 || case_compare(input, "e") == 0) {
			enemyStatus(c, m);
			break;
		}
		/* attack(a): calls meleeAttack */
		else if(case_compare(input, "attack") == 0 || case_compare(input, "a") == 0) {
			meleeAttack(c, m);
			break;
		}
		/* potion(p): use potion item currently in player's potionSlot */
		else if(case_compare(input, "potion") == 0 || case_compare(input, "p") == 0) {
			usePotion(c, true);
			break;
		}
		/* item(i): use item currently in player's itemSlot */
		else if(case_compare(input, "item") == 0 || case_compare(input, "i") == 0) {
			useItem(c, m);
			break;
		}
		/* cast(c): cast whatever magic is in player's magic slot */
		else if(case_compare(input, "cast") == 0 || case_compare(input, "c") == 0) {
			castSpell(c, m);
			break;
		}
		/* wait(w): do nothing */
		else if(case_compare(input, "wait") == 0 || case_compare(input, "w") == 0) {
			wait(c);
			break;
		}
		/* escape(exit): exits the program */
		/* shortcut is "exit" instead of "e" to avoid accidental exits */
		else if(case_compare(input, "escape") == 0 || case_compare(input, "exit") == 0) {
			escape(c, m);
			break;
		}
		else {
			printf("Invalid input, type help(h) for possible commands.\n");
		}
	} 
}
