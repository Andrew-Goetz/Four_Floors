#define _CRT_SECURE_NO_WARNINGS /* Removes depreciated warnings for strcpy on windows, for linux compatibility */
#include <assert.h>
#include <stdbool.h> 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
	#include <windows.h>
#elif __unix__
	#include <time.h>
#else
	printf("Your operating system is not supported. The program will now exit.\n");
	exit(1);
#endif

#include "constants.h"
#include "defs.h"

///*************** Utility Functions  ***************/
///* https://code-examples.net/en/q/11a859 (slightly altered version) cross platform sleep */
//void sleep_ms(int milliseconds) {
//	#ifdef _WIN32
//	    Sleep(milliseconds);
//	#elif __unix__
//	    struct timespec ts;
//	    ts.tv_sec = milliseconds / 1000;
//	    ts.tv_nsec = (milliseconds % 1000) * 1000000;
//	    nanosleep(&ts, NULL);
//	#else
//		printf("An error has occurred, your operating system may not be supported.\n");
//		exit(1);
//	#endif
//}
//
///** Cross platform strcasecmp/_stricmp */
//int case_compare(const char *word1, const char *word2) {
//	#ifdef _WIN32
//		return _stricmp(word1, word2);
//	#elif __unix__
//		return strcasecmp(word1, word2);
//	#else
//		printf("An error has occurred, your operating system may not be supported.\n");
//		exit(1);
//	#endif
//}
//
//void getInput(char input[], char message[]) {
//	printf("%s", message);
//	if(fgets(input, MAX_INPUT_LENGTH, stdin) == NULL) {
//		printf("An error has occurred, fgets() returned NULL.\n");
//	}
//	assert(strlen(input) > 0);
//	if(input[strlen(input)-1] == '\n') { // For proper line spacing
//		input[strlen(input)-1] = '\0';
//	}
//	//@TODO better handling of input when > MAX_INPUT_LENGTH
//}
//
///** https://stackoverflow.com/questions/1406421/press-enter-to-continue-in-c 
// *  Also important: don't use \n at end of printf when pressEnter() is called immediately after,
// *  because the user pressing enter already goes to the next line.
// */
//void pressEnter(void) {
//	//printf("%s", message);
//	while(getchar() != '\n');
//}
//
///** Ask simple yes or no questions to user.
// *  If yes then return true, if no then return false.
// *  When used, returned value is stored in bool isYes (convention).
// */
//bool yes_or_no(char message[]) {
//	char input[MAX_INPUT_LENGTH];
//	printf("%s", message); /* assumes message will end with \n */
//	for(;;) {
//		getInput(input, "Yes(y) or No(n): ");
//		if(case_compare(input, "yes") == 0 || case_compare(input, "y") == 0) {
//			return true;
//		} else if(case_compare(input, "no") == 0 || case_compare(input, "n") == 0) {
//			return false;
//		} else {
//			printf("Invalid input.\n"); //repeats loop and asks again
//		}
//	}
//}

/* Notes from character struct (in defs.h):
 * character dead if(health <=0), health can be negative
 * Negative mana not allowed, if not enough mana cannot cast a spell
 * no negative defense, total_damage = attacker's attack - defender's defense
 * if char isMonster == 0, its the player character
 * bool isTurn is only used in player struct, is essentially pointless in monster structs
 * elements of knowSpell are true if character knows that spell, corresponds to order of spells in enum
 */

///** Creates a new character, some characters different than others. */
//Character* newCharacter(char message[], Enemy enemy) {
//	Character *c = malloc(sizeof(*c));
//
//	c->health = MONSTER_STATS[enemy][HEALTH];
//	c->totalHealth = MONSTER_STATS[enemy][HEALTH];
//	c->mana = MONSTER_STATS[enemy][MANA];
//	c->totalMana = MONSTER_STATS[enemy][MANA];
//	c->attack = MONSTER_STATS[enemy][ATTACK];
//	c->defense = MONSTER_STATS[enemy][DEFENSE];
//
//	c->itemSlot = NOTHING;
//	c->potionSlot = NOTHING;
//	for(unsigned char i = 0; i < SPELLS_IN_GAME; i++) c->knowSpell[i] = false;
//
//	c->effect = NONE;
//	c->effectDuration = 0;
//
//	c->isMonster = enemy;
//	/* Gets player character's name */
//	if(enemy == PLAYER) {
//		getInput(c->name, "Enter your name: ");
//		printf("Your name is " C_BLUE "\'%s\'" C_RESET ". ", c->name);
//		bool isYes = yes_or_no("Is this correct?\n");
//		while(!isYes) {
//			getInput(c->name, "Enter your name: ");
//			printf("Your name is " C_BLUE "\'%s\'" C_RESET ". ", c->name);
//			isYes = yes_or_no("Is this correct?\n");
//		}
//		c->isTurn = true; // make sure player character gets first turn
//	} else {
//		strcpy(c->name, MONSTER_NAMES[enemy]);
//		printf("%s appears!\n", c->name);
//	}
//	return c;
//}
//
///** Increases the stat of the player's choice, should be called whenever a monster is defeated */
//void lvlUp(Character *c) {
//	assert(!c->isMonster);
//	printf("Level up avaliable!\nLevel health(h), mana(m), attack(a), or defense(d)?\n");
//	char input[MAX_INPUT_LENGTH];
//	for(;;) {
//		getInput(input, ">> ");
//		if(case_compare(input, "health") == 0 || case_compare(input, "h") == 0) {
//			c->totalHealth++; c->health++;
//			printf("%s feels healthier.\n", c->name);
//			return;
//		} else if(case_compare(input, "mana") == 0 || case_compare(input, "m") == 0) {
//			c->totalMana++; c->mana++;
//			printf("%s feels more intelligent.\n", c->name);
//			return;
//		} else if(case_compare(input, "attack") == 0 || case_compare(input, "a") == 0) {
//			c->attack++;
//			printf("%s feels more powerful.\n", c->name);
//			return;
//		} else if(case_compare(input, "defense") == 0 || case_compare(input, "d") == 0) {
//			c->defense++;
//			printf("%s feels stronger.\n", c->name);
//			return;
//		} else {
//			printf("Invalid input.\n");
//		}
//	}
//}
///*************** Potion Functions  ***************/
//
//void red_potion(Character *c, bool isGreater) {
//	char healVal;
//	if(isGreater) {
//		healVal = 5;
//	} else {
//		healVal = 3;
//	}
//	printf("%s drinks the %s.\n", c->name, ITEM_AND_SPELL_NAMES[c->potionSlot]);
//	sleep_ms(SLEEP_DURATION);
//	if(c->health + healVal >= c->totalHealth) {
//		c->health = c->totalHealth;
//		printf("%s is now full health.\n", c->name);
//	} else {
//		c->health += healVal;
//		printf("%s has healed %d health.\n", c->name, healVal);
//	}
//}
//
//void blue_potion(Character *c, bool isGreater) {
//	char manaVal;
//	if(isGreater) {
//		manaVal = 5;
//	} else {
//		manaVal = 3;
//	}
//	printf("%s drinks the %s.\n", c->name, ITEM_AND_SPELL_NAMES[c->potionSlot]);
//	sleep_ms(SLEEP_DURATION);
//	if(c->mana + manaVal >= c->totalMana) {
//		c->mana = c->totalMana;
//		printf("%s is now full mana.\n", c->name);
//	} else {
//		c->mana += manaVal;
//		printf("%s has restored %d mana.\n", c->name, manaVal);
//	}
//}
//
//void panacea(Character *c) {
//	printf("%s drinks the %s. It tastes sweeter than honey.\n", c->name, ITEM_AND_SPELL_NAMES[c->potionSlot]);
//	sleep_ms(SLEEP_DURATION);
//	c->health = c->totalHealth;
//	c->mana = c->totalMana;
//	printf("All health and mana are restored.\n");
//}
//
///** Use potion in potionSlot */
//void usePotion(Character *c, bool isInCombat) {
//	assert(!c->isMonster);
//	switch(c->potionSlot) {
//		case NOTHING:
//			printf("There is no potion in inventory!\n");
//			return; //Don't end turn here
//		case RED_POTION:
//			red_potion(c, false);
//			break;
//		case GREATER_RED_POTION:
//			red_potion(c, true);
//			break;
//		case BLUE_POTION:
//			blue_potion(c, false);
//			break;
//		case GREATER_BLUE_POTION:
//			blue_potion(c, true);
//			break;
//		case PANACEA:
//			panacea(c);
//			break;
//		default:
//			printf("The item had no effect. Must have been a dud!\n");
//			//printf("\nDEBUG: %d\n\n\n", c->potionSlot);
//			break;
//	}
//	c->potionSlot = NOTHING;
//	if(isInCombat) c->isTurn = false;
//}
//
///** Asks if player wants to drink potion outside of combat. */
//void drink_potion(Character *c) {
//	printf("%s looks around and sees no enemies; safety, for the time being.", c->name); pressEnter();
//	if(!c->potionSlot) {
//		printf("It would be the perfect time to drink a potion; unfortunately, %s's potion slot is empty!", c->name); pressEnter();
//		return;
//	}
//	printf("%s currently has %d/%d health and %d/%d mana.\n", c->name, c->health, c->totalHealth, c->mana, c->totalMana);
//	sleep_ms(SLEEP_DURATION);
//	printf("Drink the %s in inventory while out of combat?", ITEM_AND_SPELL_NAMES[c->potionSlot]);
//	bool isYes = yes_or_no("\n");
//	if(isYes) {
//		usePotion(c, false);
//	} else {
//		printf("Ok then.\n");
//	}
//}

///*************** Item Functions  ***************/
//Effect tears(Character *c) {
//	printf("%s drinks the %s. %s glows warmly.\n", c->name, ITEM_AND_SPELL_NAMES[c->itemSlot], c->name);
//	return TEARS_ACTIVE;
//	//@TODO needs to toggle some status for the character that only lasts a turn
//}
//
//Effect iron_pellet(Character *c) {
//	printf("%s swallows the %s, hardening the skin.\n", c->name, ITEM_AND_SPELL_NAMES[c->itemSlot]);
//	const char IRON_PELLET_DEFENSE_INCREASE = 3;
//	return DEFENSE_UP;
//	//@TODO increase defense a decent amount for 3 turns (current turn counts as a turn
//}
//
//void demon_fire(Character *user, Character *c) {
//	printf("%s throws a %s at %s, making the room erupt in flames.\n", user->name, ITEM_AND_SPELL_NAMES[user->itemSlot], c->name);
//	sleep_ms(SLEEP_DURATION);
//	if(c->isMonster == KILLER_PLANT) {
//		const char DEMON_FIRE_DAMAGE = c->totalHealth; //gets oneshot
//		c->health -= DEMON_FIRE_DAMAGE;
//		printf("%s burns in the fires of hell, draining it of all life energy.\n", c->name);
//	} else {
//		const char DEMON_FIRE_DAMAGE = 5;
//		c->health -= DEMON_FIRE_DAMAGE;
//		printf("%s is severely burned, taking %d damage!\n", c->name, DEMON_FIRE_DAMAGE);
//	}
//	if(user->isMonster) {
//		user->itemSlot = NOTHING;
//	}
//}
//
///* Stuns enemy for one turn, dealing damage if vs wraith */
//Effect light_vial(Character *user, Character *c) {
//	assert(!user->isMonster);
//	if(c->isMonster == WRAITH) {
//		const char LIGHT_VIAL_DAMAGE = 5;
//	} else {
//		printf("%s throws a %s, blinding %s.\n", user->name, ITEM_AND_SPELL_NAMES[user->itemSlot], c->name);
//	}
//	return STUN;
//}
//
//void horn(Character *user, Character *c) {
//	assert(!user->isMonster);
//}
//
///** Use item in itemSlot */
//Effect useItem(Character *c, Character *m) {
//	assert(!c->isMonster);
//	Effect retEffect = NONE;
//	switch(c->itemSlot) {
//		case NOTHING:
//			printf("There is no item in inventory!\n");
//			return NONE; // Don't end turn here
//		case TEARS:
//			retEffect = tears(c);
//			break;
//		case IRON_PELLET:
//			retEffect = iron_pellet(c);
//			break;
//		case DEMON_FIRE:
//			demon_fire(c, m);
//			break;
//		case LIGHT_VIAL:
//			retEffect = light_vial(c, m);
//			break;
//		case HORN_OF_SAUL:
//			horn(c, m);
//			break;
//		default:
//			printf("The item had no effect. Must have been a dud!\n");
//			break;
//	}
//	c->itemSlot = NOTHING;
//	c->isTurn = false;
//	return retEffect;
//}

/*************** Spell Functions  ***************/
//void fireball(Character *caster, Character *c) {
//	printf("%s casts fireball!\n", caster->name);
//	sleep_ms(SLEEP_DURATION);
//	if(c->isMonster == KILLER_PLANT) {
//		const char FIREBALL_DAMAGE = 7;
//		c->health -= FIREBALL_DAMAGE;
//		printf("%s contorts in intense pain, taking %d damage!\n", c->name, FIREBALL_DAMAGE);
//	} else {
//		const char FIREBALL_DAMAGE = 3;
//		c->health -= FIREBALL_DAMAGE;
//		printf("%s burns from the flames, taking %d damage!\n", c->name, FIREBALL_DAMAGE);
//	}
//}
//
//void lightning_stake(Character *caster, Character *c) {
//	//@TODO
//	printf("PLACEHOLDER\n");
//}
//
///* One in a hundred chance of sheep exploading, dealing SHEEP_DAMAGE damage. */
//void summon_sheep(Character *caster, Character *c) {
//	char sheep_explosion = rand() % 70;
//	printf("%s summons a sheep!\n", caster->name);
//	sleep_ms(SLEEP_DURATION);
//	if(sheep_explosion == 0) { //sheep explodes
//		const char SHEEP_DAMAGE = 10;
//		c->health -= SHEEP_DAMAGE;
//		printf("The sheep walks towards %s and exploads, dealing %d damage!\n", c->name, SHEEP_DAMAGE);
//	} else if(sheep_explosion == 1) { //different dialogue options based on random number
//		printf("The sheep seems to smoke up a bit as if about to expload, but then walks away normally.\n");
//	} else if(sheep_explosion == 60) {
//		printf("The sheep sprouts wings and flies off into the distance, leaving the mortal world behind.\n");
//	} else if(sheep_explosion == 69) {
//		printf("The sheep opens it's mouth and says \"Nice\" in a heavy British accent before walking away.\n");
//	} else {
//		printf("It's just a regular sheep; it chews up a nearby painting or rug before walking away.\n");
//	}
//	sleep_ms(SLEEP_DURATION);
//}
//
//Effect sacrificial_brand(Character *caster, Character *c) {
//	//@TODO
//	
//	return BRAND_ACTIVE;
//}
//
//Effect frost_resonance(Character *caster, Character *c) {
//	//@TODO
//	printf("%s forms a frosty mist in the air, which surrounds %s.\n", caster->name, c->name);
//	sleep_ms(SLEEP_DURATION);
//	const char FROST_RESONANCE_DAMAGE = 2;
//	const char FROST_RESONANCE_MANA = 3;
//	printf("%s takes %d damages, and freezes for a turn from the cold!\n", c->name, FROST_RESONANCE_DAMAGE);
//	return STUN;
//}
//
///** Cast */
//Effect castSpell(Character *c, Character *m) {
//	assert(!c->isMonster);
//	char isMagicUser = 0;
//	char firstSpell = 0; /* used if(isMagicUser == 1) */
//	for(int i = 0; i <= SPELLS_IN_GAME; i++) { //@TODO verify recent change works
//		if(c->knowSpell[i]) {
//			//printf("IMPORTANT DEBUG: %d\n", i);
//			isMagicUser++;
//			firstSpell = i; /* corresponds to value of spell in the enum */
//		}
//	}
//	//printf("DEBUG: isMagicUser = %d", isMagicUser);
//	if(isMagicUser == 1) { // cast whatever 1 spell the player knows
//		printf("firstSpell: %d, FIREBALL: %d\n", firstSpell, FIREBALL);
//		switch(firstSpell) {
//			case FIREBALL:
//				fireball(c, m);
//				break;
//			case LIGHTNING_STAKE:
//				lightning_stake(c, m);
//				break;
//			case SUMMON_SHEEP:
//				summon_sheep(c, m);
//				break;
//			case SACRIFICIAL_BRAND:
//				sacrificial_brand(c, m);
//				break;
//			case FROST_RESONANCE:
//				frost_resonance(c, m);
//				break;
//			default:
//				printf("Something goes wrong and %s fails to cast the spell!", c->name);
//				break;
//		}
//	} else if(isMagicUser > 1) { // output spells the player knows and have them pick one
//		printf("What spell to cast?\n");
//		if(c->knowSpell[1]) { // FIREBALL
//			printf("Fireball(f)? ");
//		} if(c->knowSpell[2]) { // LIGHTNING_STAKE
//			printf("Lightning Stake(L)? ");
//		} if(c->knowSpell[3]) { // SUMMON_SHEEP
//			printf("Summon Sheep(s)? ");
//		} if(c->knowSpell[4]) { // SACRIFICIAL_BRAND
//			printf("Sacrificial Brand(b)? ");
//		} if(c->knowSpell[5]) { // FROST_RESONANCE
//			printf("Frost Resonance(r)?");
//		}
//		printf("\n");
//		char input[MAX_INPUT_LENGTH];
//		/* Need to check knowSpell during input too, make sure user doesn't guess a spell they don't know */
//		for(;;) {
//			getInput(input, ">> ");
//			if(c->knowSpell[1] && (case_compare(input, "Fireball") == 0 || case_compare(input, "f") == 0)) {
//				fireball(c, m);
//				break;
//			} else if(c->knowSpell[2] && (case_compare(input, "Lightning Stake") == 0 || case_compare(input, "L") == 0)) {
//				lightning_stake(c, m);
//				break;
//			} else if(c->knowSpell[3] && (case_compare(input, "Summon Sheep") == 0 || case_compare(input, "s") == 0)) {
//				summon_sheep(c, m);
//				break;
//			} else if(c->knowSpell[4] && (case_compare(input, "Sacrificial Brand") == 0 || case_compare(input, "b") == 0)) {
//				sacrificial_brand(c, m);
//				break;
//			} else if(c->knowSpell[5] && (case_compare(input, "Frost Resonance") == 0 || case_compare(input, "r") == 0)) {
//				frost_resonance(c, m);
//				break;
//			} else {
//				printf("Invalid input.\n");
//			}
//		}
//	} else {
//		printf("%s tries to cast magic, but doesn't know how. %s chuckles.\n", c->name, m->name);
//		sleep_ms(SLEEP_DURATION);
//	}
//	c->isTurn = false;
//}

/** Takes two character, and character attacker and character c.
 *  Checks to make sure arguments are in correct order,
 *  then subtracts c's health by attacker's attack value.
 *  First character (attacker) deals damage to second character (c).
 */
//void meleeAttack(Character *attacker, Character *c) {
//	printf("%s attacks %s!\n", attacker->name, c->name);
//	sleep_ms(SLEEP_DURATION);
//	char effectiveDamage = attacker->attack - c->defense;
//	if(effectiveDamage > 0) {
//		c->health -= effectiveDamage;
//		printf("%s took %u damage!\n", c->name, effectiveDamage);
//	} else {
//		printf("%s took no damage!\n", c->name);
//	}
//	if(!attacker->isMonster) attacker->isTurn = false;
//}

/** When not the players turn, the monster does something: as of right now, it will always attack */
//TODO: should this be where status effects for monster are checked?
//void monsterAction(Character *m, Character *c) {
//	if(!c->isTurn) {
//		sleep_ms(SLEEP_DURATION);
//		meleeAttack(m, c);
//	}
//	c->isTurn = true;
//}
//
///* Output status, use enemyStatus for non-player characters */
//void status(Character *c) {
//	printf("%s's stats:\n\tHealth:%d/%d\n\tMana:%d/%d\n\tAttack:%d\n\tDefense:%d\n",
//			c->name, c->health, c->totalHealth, c->mana, c->totalMana, c->attack, c->defense);
//	if(c->itemSlot) {
//		printf("%s in item slot.\n", ITEM_AND_SPELL_NAMES[c->itemSlot]);
//	}
//	if(c->potionSlot) {
//		printf("%s in potion slot.\n", ITEM_AND_SPELL_NAMES[c->potionSlot]);
//	}
//	bool anySpells = false;
//	for(int i = 0; i < SPELLS_IN_GAME; i++) {
//		if(c->knowSpell[i]) {
//			printf("%s knows the following spells:\n", c->name);
//			break;
//		}
//	}
//	if(c->knowSpell[0]) { // FIREBALL
//		printf("\tFireball(f)\n");
//	} if(c->knowSpell[1]) { // LIGHTNING_STAKE
//		printf("\tLightning Stake(L)\n");
//	} if(c->knowSpell[2]) { // SUMMON_SHEEP
//		printf("\tSummon Sheep(s)\n");
//	} if(c->knowSpell[3]) { // SACRIFICIAL_BRAND
//		printf("\tSacrificial Brand(b)\n");
//	} if(c->knowSpell[4]) { // FROST_RESONANCE
//		printf("\tFrost Resonance(r)\n");
//	}
//}
//
///** Output status of enemy character, calls status.
// *  Passes in player character as well to ensure player doesn't lose a turn.
// */
//void enemyStatus(Character *c, Character *m) {
//	assert(m->isMonster);
//	static const char *MONSTER_HINTS[MONSTERS_IN_GAME] = {
//		"ERROR\n", /* Character gets no hint */
//		"has a large scar across its hairy chest. It must be weak to physical attacks!\n", /* The Beast */
//		"seems vulnerable to fire.\n", /* The Killer Plant */
//		"is a creature of the night. The sun's rays would prove fatal.\n", /* The Wraith */
//		"is a powerful wizard, and magic would have no effect against him.\n", /* The Wizard */
//		"is an unthinking creature of destruction; attacking with massive physical blows, defense must be powerful against it!\n", /* The Wizard's Golem */
//		"is the ultimate foe. He has no obvious weakness weakness.\n" /* The Vampire Lord */
//	};
//	status(m);
//	printf("%s %s", MONSTER_NAMES[m->isMonster], MONSTER_HINTS[m->isMonster]); //prints out hints/descriptions here
//	c->isTurn = true;
//}
//
///** Output help info */
//void help(void) {
//	printf("Goal: defeat your foes and stay alive, reach the fourth floor:\n"
//			"\thelp(h)\t\tlists possible commands\n"
//			"\tstatus(s)\tlists out player stats\n"
//			"\tenemy(e)\tlists out enemy's stats\n"
//			"\tattack(a)\tattack enemy, ending turn\n"
//			"\tpotion(p)\tdrink potion, ending turn\n"
//			"\titem(i)\t\tuse item, ending turn\n"
//			"\tcast(c)\t\tcast spell, ending turn\n"
//			"\twait(w)\t\tdo nothing, ending turn\n"
//			"\tescape(exit)\tabandon quest and flee\n");
//	sleep_ms(SLEEP_DURATION);
//	bool isYes = yes_or_no("Output additional game information?\n");
//	if(isYes) {
//		printf("Additional information:\n"
//				"\tHealth is life energy. When health reaches 0, death occurs.\n"
//				"\tMana is magic energy, which is expended to cast spells.\n"
//				"\tHealth and mana can only be restored using potions.\n"
//				"\tDamage dealt is attacker's attack value minus defender's defense value.\n"
//				"\tIf a player with 3 attack attacks a monster with 1 defense, 2 damage is dealt.\n"
//				"\tSpells and items ignore defense.\n"
//				"\tPotions, items, and spells can be found after each battle.\n"
//				"\tItems and potions can only be used once, while spells can be used as long as there is enough mana to cast them.\n"
//				"\tCertain items, spells, and potions are very effective against certain enemies.\n"
//				"\tAt least 1 level up occurs after each battle; this will automatically save the game.\n");
//				//"\t\n"
//	} else {
//		printf("Ok then.\n");
//	}
//}
//
///** wait, expends player turn */
//void wait(Character *c) {
//	assert(!c->isMonster);
//	printf("%s does nothing.\n", c->name);
//	c->isTurn = false;
//}
//
///** Exit */
//void escape(Character *c, Character *m) {
//	assert(!c->isMonster);
//	printf("%s runs out of the mansion in shame.\n", c->name);
//	free(c); free(m);
//	exit(0);
//}
//
///** Call when input is required, c must be the player character, m the monster */
//
//Effect actions(Character *c, Character *m) {
//	assert(!c->isMonster);
//	char input[MAX_INPUT_LENGTH];
//	for(;;) {
//		getInput(input, ">> ");
//		/* help(h): lists out possible commands and then asks if user wants more in depth information */
//		if(case_compare(input, "help") == 0 || case_compare(input, "h") == 0) {
//			help();
//			break;
//		}
//		/* status(s): outputs current player status */
//		else if(case_compare(input, "status") == 0 || case_compare(input, "s") == 0) {
//			status(c);
//			break;
//		}
//		/* enemy(e): outputs non-player-character's status, a help fight him */
//		else if(case_compare(input, "enemy") == 0 || case_compare(input, "e") == 0) {
//			enemyStatus(c, m);
//			break;
//		}
//		/* attack(a): calls meleeAttack */
//		else if(case_compare(input, "attack") == 0 || case_compare(input, "a") == 0) {
//			meleeAttack(c, m);
//			break;
//		}
//		/* potion(p): use potion item currently in player's potionSlot */
//		else if(case_compare(input, "potion") == 0 || case_compare(input, "p") == 0) {
//			usePotion(c, true);
//			break;
//		}
//		/* item(i): use item currently in player's itemSlot */
//		else if(case_compare(input, "item") == 0 || case_compare(input, "i") == 0) {
//			return useItem(c, m);
//		}
//		/* cast(c): cast whatever magic is in player's magic slot */
//		else if(case_compare(input, "cast") == 0 || case_compare(input, "c") == 0) {
//			return castSpell(c, m);
//		}
//		/* wait(w): do nothing */
//		else if(case_compare(input, "wait") == 0 || case_compare(input, "w") == 0) {
//			wait(c);
//			break;
//		}
//		/* escape(exit): exits the program */
//		/* shortcut is "exit" instead of "e" to avoid accidental exits */
//		else if(case_compare(input, "escape") == 0 || case_compare(input, "exit") == 0) {
//			escape(c, m);
//			break;
//		}
//		else {
//			printf("Invalid input, type help(h) for possible commands.\n");
//		}
//	} 
//	return NONE; // default return value
//}

/** Ask user to equip item, buffitem, or spell found.
 *  Pass in player character, particular item that is found, and message.
 *  Whether "Item" is a Spell, potion, or Item will be determined in the function.
 *  If item, needs to go in itemSlot; if potion, needs to go in potionSlot, spells stored in knowSpell.
 *  message should have a trailing \n.
 */
//void item_or_spell_found(Character *c, Item itemFound, char message[]) {
//	assert(!c->isMonster);
//	// printf("\nTEST: %s: %s\n\n", ITEM_AND_SPELL_NAMES[itemFound], ITEM_AND_SPELL_DESCRIPTIONS[itemFound]);
//	printf("%sPress enter to read its description:", message); pressEnter();
//	printf("%s", ITEM_AND_SPELL_DESCRIPTIONS[itemFound]);
//	bool isYes;
//	switch(itemFound) {
//		/* for spells, add to knowSpell */
//		case FIREBALL: case LIGHTNING_STAKE: case SUMMON_SHEEP: case SACRIFICIAL_BRAND: case FROST_RESONANCE:
//			printf("Learn %s?", ITEM_AND_SPELL_NAMES[itemFound]);
//			isYes = yes_or_no("\n");
//			if(isYes) {
//				c->knowSpell[itemFound] = true;
//				//printf("DEBUG: c->knowSpell[itemFound - 1] = %d, itemFound = %d \n", c->knowSpell[itemFound-1], itemFound);
//				printf("After reading the scroll, %s learns how to cast %s.\n", c->name, ITEM_AND_SPELL_NAMES[itemFound]);
//				char isMagicUser = 0;
//				char firstSpell = 0;
//				for(int i = 1; i < SPELLS_IN_GAME; i++) {
//					if(c->knowSpell[i]) {
//						isMagicUser++;
//						firstSpell = i; /* corresponds to value of spell in the enum */
//					}
//				}
//				//printf("DEBUG: spells = %d, firstSpell = %d\n", isMagicUser, firstSpell);
//			} else {
//				printf("Ok then.\n");
//			}
//			break;
//		/* for potions, offer to use item in inventory and take new item */
//		case RED_POTION: case GREATER_RED_POTION: case BLUE_POTION:	case GREATER_BLUE_POTION: case PANACEA:
//			printf("Add %s to potion inventory?", ITEM_AND_SPELL_NAMES[itemFound]);
//			if(!c->potionSlot) {
//				isYes = yes_or_no("\n");
//			} else {
//				printf("%s", ITEM_AND_SPELL_NAMES[c->potionSlot]);
//				isYes = yes_or_no(" will be used before being discarded.\n");
//			}
//			if(isYes) {
//				if(c->potionSlot) {
//					usePotion(c, false);
//				}
//				c->potionSlot = itemFound;
//				printf("%s is now in potion inventory\n", ITEM_AND_SPELL_NAMES[c->potionSlot]);
//			} else {
//				if(c->potionSlot) {
//					printf("%s remains in potion inventory.\n", ITEM_AND_SPELL_NAMES[c->potionSlot]);
//				} else {
//					printf("Ok then.\n");
//				}
//			}
//			break;
//		/* for items, offer to discard current item in inventory if one exists and pick up found one */
//		case TEARS: case IRON_PELLET: case DEMON_FIRE: case LIGHT_VIAL: case HORN_OF_SAUL:
//			printf("Add %s to item inventory?", ITEM_AND_SPELL_NAMES[itemFound]);
//			if(!c->itemSlot) {
//				isYes = yes_or_no("\n");
//			} else {
//				printf("%s", ITEM_AND_SPELL_NAMES[c->itemSlot]);
//				isYes = yes_or_no(" will be discarded.\n");
//			}
//			if(isYes) {
//				if(c->itemSlot) {
//					printf("%s is discarded, and ", ITEM_AND_SPELL_NAMES[c->itemSlot]);
//				}
//				printf("%s is put into item inventory.\n", ITEM_AND_SPELL_NAMES[itemFound]);
//				c->itemSlot = itemFound;
//			} else {
//				if(c->itemSlot) {
//					printf("%s remains in item inventory.\n", ITEM_AND_SPELL_NAMES[c->itemSlot]);
//				} else {
//					printf("Ok then.\n");
//				}
//			}
//			break;
//		default:
//			printf("Something goes wrong. It must have been an illusion!\n");
//	}
//}
//
///* Handles status effects */
///* For now, applying a new status effect overwrite current status effect */
//void status_effect_check(Character *c, unsigned char turn_number) {
//	Effect curEffect = NONE;
//	Effect prevEffect = NONE;
//
//}
//
///** Function called once each level when combat is in progress.
// *  c is the player character, m is the monster, and levelUpNumber is the 
// *	number of times the lvlUp function will be called when m is defeated.
// *  Also keeps track of turn number so that status effects can be applied properly.
// *  MEMORY: monster is freed in this function.
// */
//void combat_sequence(Character *c, Character *m, unsigned char levelUpNumber) {
//	assert(!c->isMonster && m->isMonster);
//	unsigned char turn_number = 0;
//	unsigned char status_effect_count = 0;
//	bool isTurnChanged;
//	Effect curEffect;
//	for(;;) {
//		curEffect = actions(c, m);
//		if(m->health <= 0) {
//			sleep_ms(SLEEP_DURATION);
//			printf("VICTORY!\n");
//			for(; levelUpNumber != 0; levelUpNumber--) {
//				lvlUp(c);
//			}
//			c->isTurn = true;
//			break;
//		}
//		bool isTurnChanged = c->isTurn;
//		monsterAction(m, c);
//		if(c->health <= 0) {
//			printf("%s has been defeated!\n", c->name);
//			free(m); free(c); exit(0);
//		}
//		/* want to make sure turn_number not incremented on help or other non-isTurn-changing instructions */
//		if(isTurnChanged != c->isTurn) {
//			if(turn_number == 255) {
//				printf("%s after so long fighting, %s collapses in exhaustion. Defeat!\n", c->name, c->name);
//				free(m); free(c); exit(0);
//			} else if(turn_number == 252) {
//				printf("%s cannot go on much longer! End this fight before exhaustion overtakes %s!\n",
//						c->name, c->name);
//			}
//			turn_number++;
//			//printf("\n%d\n\n", turn_number);
//		}
//	}
//	free(m);
//}

///* Floor 1 start */
//void lvl0(Character *c) {
//	printf("Press enter to advance through dialogue."); pressEnter();
//	printf("A forest of trees surrounds a clearing; it is here that a massive,"
//			" four-floor mansion towers above the forest."); pressEnter();
//	printf(C_BLUE "%s " C_RESET "wonders: what might lie on the fourth floor?", c->name); pressEnter();
//	printf(C_BLUE "%s " C_RESET "reaches the massive front doors of the mansion.\n", c->name);
//	bool isYes = yes_or_no("Enter the mansion, beginning a perilous journey?\n");
//	if(!isYes) {
//		printf("Intimidated by the mansion," C_BLUE " %s " C_RESET "turns around and heads home. "
//				"Maybe it's for the best.\n", c->name);
//		free(c); exit(0);
//	}
//	printf("The doors creak open." C_BLUE " %s " C_RESET "enters the mansion.", c->name); pressEnter();
//	printf("Light streams into the mansion, revealing the dust floating in the air."); pressEnter();
//	printf("SLAM! The door closes behind %s!", c->name); pressEnter();
//	printf("%s turns and hears unnatural growls.", c->name); pressEnter();
//	Character *m = newCharacter(" appears!\nType help(h) for how to fight!", BEAST);
//	combat_sequence(c, m, 1);
//}
//
//void lvl1(Character *c) {
//	printf("Looking at %s on the ground, %s notices a strange brand near the beast's torso. "
//		   "Maybe it's important.", MONSTER_NAMES[BEAST], c->name); pressEnter();
//	printf("With the first foe defeated, it is time to move on! Press enter to continue deeper into the mansion."); pressEnter();
//	item_or_spell_found(c, RED_POTION, "After walking down a hallway, a potion can be seen sitting on a bookshelf.\n");
//	drink_potion(c);
//	printf("%s enters a massive room with another hallway branching off to the left.", c->name); pressEnter();
//	bool isYes = yes_or_no("Take this other path?\n");
//	if(isYes) {
//		printf("Something about this left hallway seems special. %s heads left and leaves the room.", c->name); pressEnter();
//		printf("The hallway continues for a time before making an abrupt right turn. Press enter to see what's around the corner."); pressEnter();
//		item_or_spell_found(c, FIREBALL, "On a small table to the side of the hall, there is a magical scroll!\n");
//		printf("At the end of the hallway there is a door. Press enter to open it."); pressEnter();
//	} else {
//		printf("%s decides to stay on the current path and leaves the room.", c->name); pressEnter();
//		printf("The next room is charred black; there must have been an intense fire here long ago."); pressEnter();
//		item_or_spell_found(c, DEMON_FIRE, "In a tattered robe on the floor is a mysterious vial of liquid.\n");
//		printf("The door is charred black; despite this, %s manages to force it open.", c->name); pressEnter();
//	}
//	printf("In the next room, %s sees a staircase. This must lead to the second floor!", c->name); pressEnter();
//	printf("The distant voice of an elderly man echoes mysteriously in the distance: "
//			"\"Begone from this place, stranger!\""); pressEnter();
//	printf("Suddenly, a magical glow fills the air, and a massive plant grows rapidly out of the ground."); pressEnter();
//	printf("It resembles a Venus flytrap, only 100 times the size!"); pressEnter();
//	Character *m = newCharacter(" appears!", KILLER_PLANT);
//	combat_sequence(c, m, 1);
//}
//
///* Floor 2 start */
//void lvl2(Character *c) {
//	printf("%s droops to the ground, then magically shrinks back down to normal size.", MONSTER_NAMES[KILLER_PLANT]); pressEnter();
//	printf("Whoever spoke earlier is gone; there is only silence."); pressEnter();
//	printf("It is time to move on. Press enter to proceed up the stairs to the second floor."); pressEnter();
//	printf("A faded red carpet lines the stairs and continues into the dark hallway beyond."); pressEnter();
//	item_or_spell_found(c, BLUE_POTION, "Right at the top of the stairs is a potion!\n");
//	printf("%s is now surrounded by darkness, but notices a faint light coming from the right.", c->name); pressEnter();
//	bool isYes = yes_or_no("Follow the source of light?");
//	if(isYes) {
//		printf("%s turns right in an attempt to escape the darkness.", c->name); pressEnter();
//		printf("The light grows brighter as %s continues down the corridor.", c->name); pressEnter();
//		printf("%s enters the next room and is blinded by the light radiating from its center.", c->name); pressEnter();
//		item_or_spell_found(c, LIGHT_VIAL, "The light is coming from an item!\n");
//		printf("There is a door on the right side of the room. Press enter to exit the room."); pressEnter();
//	} else {
//		printf("%s opts to continue on the main path.", c->name); pressEnter();
//		printf("Walking forward, %s almost falls into a massive hole in the floor. "
//				"It seems to have no bottom.", c->name); pressEnter();
//		isYes = yes_or_no("Jump into the hole?");
//		if(isYes) {
//			printf("%s jumps into the abyss but immediately lands on something soft... It's a sheep?", c->name); pressEnter();
//			printf("The sheep has wings, and majestically flies %s back to safety.", c->name); pressEnter();
//			item_or_spell_found(c, SUMMON_SHEEP, "The sheep has a magic scroll in its mouth!");
//			printf("The sheep disappears. %s is unsure if it was a hallucination or not.", c->name); pressEnter();
//		} else {
//			printf("%s makes the sensible decision not to jump into the hole.", c->name); pressEnter();
//		}
//		printf("Continuing on the path, %s notices a slight glimmer on the floor.", c->name); pressEnter();
//		printf("It's a huge claymore, unfortunately shattered and unusable."); pressEnter();
//		item_or_spell_found(c, TEARS, "Next to the sword is a vial of some liquid.\n");
//		printf("%s reaches a door at the end of the hallway. Press enter to open it.", c->name); pressEnter();
//	} //end else
//	drink_potion(c);
//	printf("Press enter to open the door and continue."); pressEnter();
//	printf("Immediately upon entering the room %s feels an unatural presence.", c->name); pressEnter();
//	printf("The room is ice cold, and a cold breeze blows the door shut!"); pressEnter();
//	printf("A mysterious figure appears floating above the ground. It wears a torn black cloak and wields a large scythe."); pressEnter();
//	Character *m = newCharacter(" appears!", WRAITH);
//	combat_sequence(c, m, 1);
//}
//
///* Floor 3 start */
//void lvl3(Character *c) {
//	printf("\"You will pay for this!\" This voice... it's the same voice as on the second floor!"); pressEnter();
//	printf("The old man who steps forward wears a wizard's hat, and has a crazy look in his eye."); pressEnter();
//	Character *m = newCharacter(" appears!", MAD_WIZARD);
//	combat_sequence(c, m, 2);
//
//}
//
//void lvl4(Character *c) {
//	printf("%s mutters a name in his dying breath: \"Elizabeth...\"", MONSTER_NAMES[MAD_WIZARD]); pressEnter();
//	printf("Behind him, %s sees a number of books, undoubtably written by the deceased man. Press enter to read a few.", c->name); pressEnter();
//	printf(" "); pressEnter();
//	bool isYes = yes_or_no("");
//	if(isYes) {
//
//	} else {
//
//	}
//	isYes = yes_or_no("Offer up all items and potions in inventory as tribute to the statue?");
//	if(isYes) {
//		item_or_spell_found(c, SACRIFICIAL_BRAND, "");
//	}
//	Character *m = newCharacter(" appears!", GOLEM);
//	combat_sequence(c, m, 2);
//}
//
///* Floor 4 start */
//void lvl5(Character *c) {
//	// printf("%s crumbles into pieces."); pressEnter();
//	item_or_spell_found(c, GREATER_BLUE_POTION, "Among the pieces is a potion!");
//	printf("Suddenly, a bookshelf on the far wall slides aside, revealing stairs to the fourth floor!");
//
//	printf("Press enter to climb the stairs to the fourth floor the mansion."); pressEnter();
//	item_or_spell_found(c, PANACEA, "On a pedastal in the center of the hall is a beautiful, rainbow-colored potion.");
//	printf("At the end of the hall lies a foreboding pair of black doors. Press enter to open them."); pressEnter();
//	printf("The next room is extravagantly decorated; stained glass windows adorn the walls, and a cloaked figure sits upon a magnificent throne."); pressEnter();
//	printf("He rises slowly from his throne, his crimson-red eyes striking fear into %s's heart.", c->name); pressEnter();
//	printf("His sharp teeth make it clear he is a vampire. %s thinks the red liquid in his cup may not be wine.", c->name); pressEnter();
//	printf("The vampire clears his throat and speaks: \"You have done well to come this far, and for that you deserve my respect.\""); pressEnter();
//	printf("\"Therefore, I will give you one last chance, %s. Leave this place, or I will kill you. Your death is guaranteed if you stay.\"", c->name); pressEnter();
//	bool isYes = yes_or_no("Take the vampire up on his offer and leave the mansion?");
//	if(isYes) {
//		printf("\"Hahaha, I always expected you would be a coward. Now, leave me be and vanish, before I change my mind\""); pressEnter();
//		printf("With that, %s leaves the vampire's throne room in shame, never knowing what treasure the vampire surely guards.\n", c->name); pressEnter();
//		printf("Press enter to admit defeat and leave the mansion."); pressEnter();
//		free(c); exit(0);
//	}
//	printf("\"You fool! You dare think you can challenge me, Lord of All Vampires? You boast, %s, and unduely so.\"", c->name); pressEnter();
//	Character *m = newCharacter(" tears off his cloak, unsheaths a rapier, and growls.", VAMPIRE_LORD);
//	printf("\"Your move, %s.\"", c->name);
//	combat_sequence(c, m, 0); // last fight so no level ups needed
//}
//
//void the_end(Character *c) {
//	printf(" ");
//}

int main() {
	/* Player created in main, monsters in the lvl functions */
	Character *c = newCharacter("", PLAYER);

	lvl0(c);
	lvl1(c);
	lvl2(c);
	//lvl3(c);
	//lvl4(c);
	//lvl5(c);
	the_end(c);

	free(c);
	return 0;
}
