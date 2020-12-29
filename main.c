#include <assert.h>
#include <stdbool.h> 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*********** Constants ************/

#define MAX_INPUT_LENGTH 31 /* No input greater than MAX_INPUT_LENGTH-1 characters allowed */

/* Player's starting stats */
const unsigned char STARTING_PLAYER_HEALTH = 5;
const unsigned char STARTING_PLAYER_ATTACK = 1;

/* Monster's stats, will assign different ones for each monster type in future */
const unsigned char MONSTER_HEALTH = 3;
const unsigned char MONSTER_ATTACK = 1;
const char MONSTER_NAME[] = "The monster";


/** Defines a generic character */
typedef struct Characters {
	unsigned char health;
	//unsigned char mana;
	unsigned char attack;
	//unsigned char defense;
	bool isTurn; /* True if character's turn, false if not */
	bool isPlayerCharacter;
	char name[MAX_INPUT_LENGTH];
} Character;

/** Call to get input, formats to get rid of trailing \n */
void getInput(char input[], char message[]) {
	printf("%s", message);
	fgets(input, MAX_INPUT_LENGTH, stdin);
	assert(input[strlen(input)-1] == '\n'); /* Something weird if this fails */
	input[strlen(input)-1] = '\0';
}

/** Creates a new player character, should only be called once until multiplayer is added */
Character* newPlayerCharacter() {
	Character *c = malloc(sizeof(*c));
	c->health = STARTING_PLAYER_HEALTH;
	c->attack = STARTING_PLAYER_ATTACK;
	c->isTurn = true; /* Player gets the first turn */
	c->isPlayerCharacter = true;
	getInput(c->name, "Enter the character's name: ");
//	c.name[strlen(c.name)-1] = '\0';

	printf("%s has %u health\n", c->name, c->health);
	printf("%s has %u attack power\n", c->name, c->attack);
	printf("Type help(h) for how to play\n");
	return c;
}

/** Creates a new non-player character */
Character* newCharacter() {
	Character *c = malloc(sizeof(*c));
	c->health = MONSTER_HEALTH;
	c->attack = MONSTER_ATTACK;
	c->isTurn = false; /* Player gets the first turn */
	c->isPlayerCharacter = false;
	strcpy(c->name, MONSTER_NAME);
	printf("%s has appeared!\n", c->name);
	printf("%s has %u health\n", c->name, c->health);
	printf("%s has %u attack power\n", c->name, c->attack);
	return c;
}

/** Frees memopry alloted to a character upon death */
void freeCharacter(Character *c) {
	freeCharacter(c);
}


//#if 0
/** Ends the turn, changing the state of isTurn for both characters.
 *  Also determines if one of the characters is dead, freeing memory if needed.
 *  Exits program if any character has died. //@TODO change this later to only player character
 */
void nextTurn(Character *c1, Character *c2) {
	//@TODO if needed, need to rethink this so more than two characters can fight at once
	if(c1->health <= 0) {
		printf("%s has defeated %s!\n", c2->name, c1->name);
		if(c1->isPlayerCharacter) {
			printf("GAME OVER!\n");
			freeCharacter(c1); freeCharacter(c2);
			exit(0);
		} else {
			printf("VICTORY!\n");
			freeCharacter(c1); freeCharacter(c2);
			exit(0);
		}
	} else if(c2->health <= 0) {
		printf("%s has defeated %s!\n", c2->name, c1->name);
		if(c2->isPlayerCharacter) {
			printf("GAME OVER!\n");
			freeCharacter(c1); freeCharacter(c2);
			exit(0);
		} else {
			printf("VICTORY!\n");
			freeCharacter(c1); freeCharacter(c2);
			exit(0);
		}
	} 
	assert(c1->health > 0 && c2->health > 0);
	!c1->isTurn; !c2->isTurn;
}

/** Takes two character, and character attacker and character c.
 *  Checks to make sure arguments are in correct order,
 *  then subtracts c's health by attacker's attack value.
 *  Called "melee" because there will be other attacks later.
 *  First character (attacker) deals damage to second character (c).
 *
 *  Calls nextTurn.
 */
void meleeAttack(Character *attacker, Character *c) {
	printf("%s attacks %s!\n", attacker->name, c->name);
	c->health -= attacker->attack;
	printf("%s took %u damage!\n", c->name, attacker->attack);
	nextTurn(attacker, c);
}

/** Output stats of character pointed to by c */
void status(Character *c) {
	printf("%s's stats:\n\tHealth:%d/%d\n\tAttack:%d\n",
			c->name, c->health, STARTING_PLAYER_HEALTH, c->attack);
}

/** Output help info */
void help() {
	printf("Goal: defeat your foes and stay alive, reach the fourth floor:\n"
			" help(h)\tlists possible commands\n"
			" attack(a)\tattack your foe\n"
			" status(s)\tlists out stats, i.e: health\n"
			" wait(w)\tdaydream\n"
			" escape(exit)\tabandon your quest\n"
			"\n");
}

/** Calls next turn */
void wait(Character *c, Character *m) {
	printf("%s does nothing\n\n", c->name);
	nextTurn(c, m);
}

void escape(Character *c) {
	printf("%s runs away in shame\n\n", c->name);
	exit(0);
}

/** Call when input is required, c must be the player character, m the monster */
//@TODO work needed for this function to support multiple monsters/players
//@TODO make case insensitive, should be attack or ATTACK or AtTacK or A instead of just lower case
void actions(Character *c, Character *m) {
	assert(c->isPlayerCharacter);
	unsigned char input[MAX_INPUT_LENGTH];
	bool isValidInput = true;
	getInput(input, ">> ");
	/***** Commands: *****/
	do {
		/* help(h): lists out possible commands and a little how to play */
		if(strcmp(input, "help") == 0 || strcmp(input, "h") == 0) {
			help();
			isValidInput = true;
		}

		/* attack(a): calls meleeAttack */
		//@TODO add argument for different types of attacks later, maybe this instead of dedicated magic command?
		else if(strcmp(input, "attack") == 0 || strcmp(input, "a") == 0) {
			meleeAttack(c, m);
			isValidInput = true;
		}

		/* status(a): check status of player character, c */
		//@TODO add argument to status to check status of monster as well, will support multiple characters
		else if(strcmp(input, "status") == 0 || strcmp(input, "s") == 0) {
			status(c);
			isValidInput = true;
		}

		/* wait(w): do nothing */	
		else if(strcmp(input, "wait") == 0 || strcmp(input, "w") == 0) {
			wait(c, m);
			isValidInput = true;
		}

		/* escape(exit): exits the program, maybe serve a practical purpose if adding ranged weapons/magic? */
		/* shortcut is "exit" instead of "e" to avoid accidental exits */
		else if(strcmp(input, "escape") == 0 || strcmp(input, "exit") == 0) {
			printf("%s runs away in shame\n\n", c->name);
			exit(0); //@TODO this doesn't free stuff
		}

		/* invalid command */
		else {
			printf("Invalid input, type help for possible commands\n");
			isValidInput = false;
		}
	} while(!isValidInput); /* ask user for input again if the last input was invalid */
}
//#endif

void lvl0(Character *player) {
	Character *monster = newCharacter();
	actions(player, monster);
	//printf("%c, then %c\n", player.name[5], player.name[6]);
}

int main() {
	Character *player = newPlayerCharacter(); /* Player created in main, monsters in the lvl functions */
	lvl0(player);
	return 0;
}
