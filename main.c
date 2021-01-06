#include <assert.h>
#include <stdbool.h> 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
//@TODO unistd.h and string.h functions used are non-portable, maybe use preprocessor for windows compatability?

/*********** Constants ************/

#define MAX_INPUT_LENGTH 30 /* No input greater than MAX_INPUT_LENGTH characters allowed */

/* Player's starting stats */
const unsigned char STARTING_PLAYER_HEALTH = 5;
const unsigned char STARTING_PLAYER_MANA = 3;
const unsigned char STARTING_PLAYER_ATTACK = 2;
const unsigned char STARTING_PLAYER_DEFENSE = 1;

/* Monster's stats, will assign different ones for each monster type in future */
const unsigned char MONSTER_HEALTH = 3;
const unsigned char MONSTER_ATTACK = 2;
const char MONSTER_NAME[] = "The monster";

/** Lists out items and spells, BuffItems have their own separate inventory slot from regular Items */
typedef enum ITEMS_AND_SPELLS {
	/* Nothing is the default inventory and spell slot state */
	NOTHING,
	/* BuffItems */
	RED_POTION,
	GREATER_RED_POTION,
	BLUE_POTION,
	GREATER_BLUE_POTION,
	PANACEA,
	/* Items */
	TEARS,
	IRON_PELLET,
	DEMON_FIRE,
	LIGHT_VIAL,
	HORN_OF_SAUL,
	/* Spells */
	FIREBALL,
	LIGHTNING_STAKE,
	SUMMON_SHEEP,
	SACRIFICIAL_BRAND,
	FROST_RESONANCE
} BuffItem, Item, Spell;

/* Item descriptions, be precise about health and mana but not specific on attack/defense effects */
const char NOTHING_DESCRIPTION[] = "Nothing in this slot.\nFind powerful items and spells in the mansion.\n";
const char RED_POTION_DESCRIPTION[] = "A red, slimey liquid. Doesn't taste as good as it looks.\nRestores 3 health when used.\n";
const char GREATER_RED_POTION_DESCRIPTION[] = "A red, slimey liquid. Tastes stronger than the regular potion.\nRestores 4 health when used.\n";
const char BLUE_POTION_DESCRIPTION[] = "A blue, cold liquid. Is viscous, like syrup.\nRestores 2 mana when used.\n";
const char GREATER_BLUE_POTION_DESCRIPTION[] = "A blue, cold liquid. Is viscous, like syrup.\nRestores 3 mana when used.\n";
const char PANACEA_DESCRIPTION[] = "The cure-all spoken of in ancient legends.\nRemoves any negatives effects, such as poison.\n";
const char TEARS_DESCRIPTION[] = "The tears of a fallen hero and his broken promise.\nUsing immediately before a monster deals fatal damage restores health to full.\n";
const char IRON_PELLET_DESCRIPTION[] = "A foul tasting medicine of foreign origin.\nMakes the skin solid as iron for a single turn.\n";
const char DEMON_FIRE_DESCRIPTION[] = "The flames of ancient demons, captured in a vial by coastal wizards.\nSmashing this vial will make enemies erupt into flames.\n";
const char LIGHT_VIAL_DESCRIPTION[] = "A drop of pure sunlight, captured in a vial by coastal wizards.\nSmashing this vial will drown a room in the light of day, blinding enemies.\n";
const char HORN_OF_SAUL_DESCRIPTION[] = "A horn once sounded by Saul, a servant of the gods.\nBlowing this horn will destroy it, temporarily granting its user the strength of the gods.\n";
const char FIREBALL_DESCRIPTION[] = "The magic of the southern deserts, it radiates blue with magical energy.\nLaunches a fireball at enemies. Consumes 1 mana.\n";
const char LIGHTNING_STAKE_DESCRIPTION[] = "The magic of the eastern dragon slayers.\nSmash lightning into the earth, shocking surroundings.\n";
const char SUMMON_SHEEP_DESCRIPTION[] = "The magic of a madman.\nSummons a sheep, which might possibly explode.\n";
const char SACRIFICIAL_BRAND_DESCRIPTION[] = "The magic of the flagellants. Carve a brand into arm.\nKills the next enemy that touches you, but reduces health to 1 when attacked.\n";
const char FROST_RESONANCE_DESCRIPTION[] = "The magic of the northern icemen.\nEnvelops enemy in frost, dealing damage overtime.\n";


/** Defines a generic character */
typedef struct Characters {
	char totalHealth;
	char totalMana;
	Item itemSlot;
	BuffItem buffItemSlot;
	Spell spellSlot;
	char health; /* character dead if(health <= 0) */
	unsigned char mana; /* Negative mana not allowed, if not enough mana cannot cast a spell */
	unsigned char attack;
	char defense; //@TODO decide whether negative defense is possible or not
	bool isPlayerCharacter;
	bool isTurn;
	char name[MAX_INPUT_LENGTH];
} Character;

/** Call to get input, formats to get rid of trailing \n if it exists */
void getInput(char input[], char message[]) {
	printf("%s", message);
	fgets(input, MAX_INPUT_LENGTH, stdin);
	assert(strlen(input) > 0);
	if(input[strlen(input)-1] == '\n') {
		input[strlen(input)-1] = '\0';
	}
}

/** https://stackoverflow.com/questions/1406421/press-enter-to-continue-in-c */
void enterToContinue(char message[]) {
	printf("%s", message);
	while(getchar() != '\n');
	printf("\n");
}

/** Ask simple yes or no questions to user //@TODO maybe this instead of enterToContinue? 
 *  if yes then return true, if no then return false.
 */
bool yes_or_no(char message[]) {
	char input[MAX_INPUT_LENGTH];
	bool isValidInput = false;
	printf("%s", message);
	while(true) {
		getInput(input, ">> ");
		if(strcasecmp(input, "yes") == 0 || strcasecmp(input, "y")) {
			return true;
		} else if(strcasecmp(input, "no") == 0 || strcasecmp(input, "n")) {
			return false;
		} else {
			printf("Invalid input.\n");
		}
	}
}

/** Creates a new player character, should only be called once until multiplayer is added */
Character* newPlayerCharacter() {
	/* Create character */
	Character *c = malloc(sizeof(*c));
	c->totalHealth = STARTING_PLAYER_HEALTH;
	c->totalMana = STARTING_PLAYER_MANA;
	c->itemSlot = NOTHING;
	c->buffItemSlot = NOTHING;
	c->spellSlot = NOTHING;

	c->health = STARTING_PLAYER_HEALTH;
	c->mana = STARTING_PLAYER_MANA;
	c->attack = STARTING_PLAYER_ATTACK;
	c->defense = STARTING_PLAYER_DEFENSE;
	c->isPlayerCharacter = true;
	c->isTurn = true; /* Controls turn for both characters, if false it's the monster's turn */
	getInput(c->name, "Enter your traveler's name: ");

	printf("%s has %u health.\n", c->name, c->health);
	printf("%s has %u attack power.\n", c->name, c->attack);

	printf("%s approaches the mansion.\n", c->name);
	printf("What might lie on the fourth floor?\n");
	enterToContinue("Press enter to enter the mansion:");
	return c;
}

/** Creates a new non-player character */
Character* newCharacter(char message[]) {
	Character *c = malloc(sizeof(*c));
	c->health = MONSTER_HEALTH;
	c->attack = MONSTER_ATTACK;
	c->isPlayerCharacter = false;
	strcpy(c->name, MONSTER_NAME);
	printf("%s %s\n", c->name, message);
	return c;
}

/** Increases the stat of the player's choice, should be called whenever a monster is defeated */
void lvlUp(Character *c) {
	assert(c->isPlayerCharacter);
	printf("Level up avaliable!\nLevel health(h), mana(m), attack(a), or defense(d)?\n");
	char input[MAX_INPUT_LENGTH];
	bool isValidInput = true;
	do {
		getInput(input, ">> ");
		if(strcasecmp(input, "health") == 0 || strcasecmp(input, "h") == 0) {
			c->totalHealth++; c->health++;
			printf("%s's feels healthier.\n\n", c->name);
			isValidInput = true;
		}
		else if(strcasecmp(input, "mana") == 0 || strcasecmp(input, "m") == 0) {
			c->totalMana++; c->mana++;
			printf("%s's feels more intelligent.\n\n", c->name);
			isValidInput = true;
		}
		else if(strcasecmp(input, "attack") == 0 || strcasecmp(input, "a") == 0) {
			c->attack++;
			printf("%s's feels more powerful.\n\n", c->name);
			isValidInput = true;
		}
		else if(strcasecmp(input, "defense") == 0 || strcasecmp(input, "d") == 0) {
			c->defense++;
			printf("%s's feels stronger.\n\n", c->name);
			isValidInput = true;
		}
		else {
			printf("Invalid input, try again.\n");
			isValidInput = false;
		}
	} while(!isValidInput);
}

/** Ask user to equip item, buffitem, or spell found.
 *  Pass in player character, particular item that is found, and message.
 *  Whether "item" is a Spell, BuffItem, or Item will be determined in the function.
 *  If item, needs to go in itemSlot; if BuffItem, needs to go in buffItemSlot //@TODO decide what's up with spells
 */
void item_or_spell_found(Character *c, Item item, char message[]) {
	assert(c->isPlayerCharacter);
	//@TODO
}

/** Takes two character, and character attacker and character c.
 *  Checks to make sure arguments are in correct order,
 *  then subtracts c's health by attacker's attack value.
 *  Called "melee" because there "might" be other attacks later.
 *  First character (attacker) deals damage to second character (c).
 *  Ends player turn if attacker->isPlayerCharacter;
 */
void meleeAttack(Character *attacker, Character *c) {
	printf("%s attacks %s!\n", attacker->name, c->name);
	sleep(1);
	char effectiveDamage = attacker->attack - c->defense;
	if(effectiveDamage > 0) {
		c->health -= effectiveDamage;
		printf("%s took %u damage!\n\n", c->name, effectiveDamage);
	} else {
		printf("%s took no damage!\n\n", c->name);
	}
	if(attacker->isPlayerCharacter) attacker->isTurn = false;
}

/** Output stats of character pointed to by c */
void status(Character *c) {
	printf("%s's stats:\n\tHealth:%d/%d\n\tMana:%d/%d\n\tAttack:%d\n\tDefense:%d\n\n",
			c->name, c->health, c->totalHealth, c->mana, c->totalMana,
			c->attack, c->defense);
}

/** Output help info */
void help() {
	printf("Goal: defeat your foes and stay alive, reach the fourth floor:\n"
			"\thelp(h)\t\tlists possible commands\n"
			"\tstatus(s)\tlists out stats, i.e: health\n"
			"\tattack(a)\tattack enemy, ending turn\n"
			"\tuse(u)\t\tuse item, ending turn\n"
			"\tcast(c)\t\tcast spell, ending turn\n"
			"\twait(w)\t\tdo nothing, ending turn\n"
			"\tescape(exit)\tabandon quest and flee\n"
			"\n");
}

/** Use */
void useItem(Character *c, Character *m) {
	assert(c->isPlayerCharacter);
}

/** Cast */
void castSpell(Character *c, Character *m) {
	assert(c->isPlayerCharacter);

}

/** Nothing */
void wait(Character *c) {
	assert(c->isPlayerCharacter);
	printf("%s does nothing.\n\n", c->name);
	c->isTurn = false;
}

/** Exit */
void escape(Character *c, Character *m) {
	printf("%s runs out of the mansion in shame.\n\n", c->name);
	free(c); free(m);
	exit(0);
}


/** Call when input is required, c must be the player character, m the monster */
void actions(Character *c, Character *m) {
	assert(c->isPlayerCharacter);
	unsigned char input[MAX_INPUT_LENGTH];
	bool isValidInput = true;
	do {
		getInput(input, ">> ");
		/* help(h): lists out possible commands and a little how to play */
		if(strcasecmp(input, "help") == 0 || strcasecmp(input, "h") == 0) {
			help();
			isValidInput = true;
		}
		/* attack(a): calls meleeAttack */
		else if(strcasecmp(input, "attack") == 0 || strcasecmp(input, "a") == 0) {
			meleeAttack(c, m);
			isValidInput = true;
		}
		/* use(u): use item currently in player's inventory slot */
		else if(strcasecmp(input, "use") == 0 || strcasecmp(input, "u") == 0) {
			useItem(c, m);
			isValidInput = true;
		}
		/* cast(c): cast whatever magic is in player's magic slot */
		else if(strcasecmp(input, "cast") == 0 || strcasecmp(input, "c") == 0) {
			castSpell(c, m);
			isValidInput = true;
		}
		/* wait(w): do nothing */	
		else if(strcasecmp(input, "wait") == 0 || strcasecmp(input, "w") == 0) {
			wait(c);
			isValidInput = true;
		}
		/* escape(exit): exits the program, maybe serve a practical purpose if adding ranged weapons/magic? */
		/* shortcut is "exit" instead of "e" to avoid accidental exits */
		else if(strcasecmp(input, "escape") == 0 || strcasecmp(input, "exit") == 0) {
			escape(c, m);
			exit(0);
		} 
		else {
			printf("Invalid input, type help(h) for possible commands\n");
			isValidInput = false;
		}
	} while(!isValidInput); /* ask user for input again if the last input was invalid */
}

/** When not the players turn, the monster does something: as of right now, it will always attack */
void monsterAction(Character *m, Character *c) {
	if(!c->isTurn) {
		sleep(1);
		meleeAttack(m, c);
	}
	c->isTurn = true;
}

const unsigned LVL0_SAVE_CODE = 547283;
void lvl0(Character *player) {
	Character *monster = newCharacter("appears!");
	printf("Type help(h) for how to play\n");
	do {
		actions(player, monster);
		if(monster->health <= 0) {
			printf("VICTORY!\n");
			lvlUp(player);
			break;
		}
		monsterAction(monster, player);
		if(player->health <= 0) {
			printf("You have been defeated!\n");
			break;
		}
//		printf("state of isTurn: %d\n", player->isTurn);
	} while(player->health > 0);
	free(monster);
}
//@TODO implement a save file in the main function, and perhaps an option to start a new game instead of that
int main() {
	Character *player = newPlayerCharacter(); /* Player created in main, monsters in the lvl functions */
	lvl0(player);
	status(player);
	free(player);
	return 0;
}
