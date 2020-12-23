#include <assert.h>
#include <stdbool.h> 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*********** Constants ************/
/* Player's starting stats */
const unsigned char STARTING_PLAYER_HEALTH = 5;
const unsigned char STARTING_PLAYER_ATTACK = 1;

/* Monster's stats, will assign different ones for each monster type in future */
const unsigned char MONSTER_HEALTH = 3;
const unsigned char MONSTER_ATTACK = 1;
const char MONSTER_NAME[] = "The monster";

#define MAX_INPUT_LENGTH 9

/** Defines a generic character */
typedef struct Characters {
	unsigned char health;
	//  unsigned char mana;
	unsigned char attack;
	//  unsigned char defense;
	bool isTurn; /* True if character's turn, false if not */
	bool isPlayerCharacter;
	char name[MAX_INPUT_LENGTH];
} Character;

/** Creates a new player character, should only be called once until multiplayer is added */
Character newPlayerCharacter() {
	Character c;
	c.health = STARTING_PLAYER_HEALTH;
	c.attack = STARTING_PLAYER_ATTACK;
	c.isTurn = true; /* Player gets the first turn */
	c.isPlayerCharacter = true;
	sscanf(stdin, "Enter the character's name: %s\n", c.name);
	printf("%s has %u health\n", c.name, c.health);
	printf("%s has %u attack power\n", c.name, c.attack);
	printf("Type help(h) for how to play\n");
	return c;
}

/** Creates a new non-player character */
Character newCharacter() {
	Character c;
	c.health = MONSTER_HEALTH;
	c.attack = MONSTER_ATTACK;
	c.isTurn = false; /* Player gets the first turn */
	c.isPlayerCharacter = false;
	strcpy(c.name, MONSTER_NAME);
	printf("%s has appeared!\n", c.name);
	printf("%s has %u health\n", c.name, c.health);
	printf("%s has %u attack power\n", c.name, c.attack);
	return c;
}

/** Ends the turn, changing the state of isTurn for both characters.
 * Also determines if one of the characters is dead,
 * exits program if the character has died, //@TODO change this later
 * NOTE: c1's turn has just ended, c2's turn is now beginning
 */
bool nextTurn(Character c1, Character c2) {
	//@TODO if needed, need to rework this so more than two characters can fight at once
	assert(c1.isTurn && !c2.isTurn); /* make sure arguments are in correct order */

	if(c1.health <= 0) {
		printf("%s has died!\n", c1.name);
		if(c1.isPlayerCharacter) {
			printf("GAME OVER!\n");
			exit(0);
		} else {
			printf("%s is victorious!\n", c2.name);
			exit(0);
		}
	} else if(c2.health <= 0) {
		printf("%s has died!\n", c2.name);
		if(c2.isPlayerCharacter) {
			printf("GAME OVER!\n");
			exit(0);
		} else {
			printf("%s is victorious!\n", c1.name);
			exit(0);
		}
	} 
	assert(c1.health > 0 && c2.health > 0);
	c1.isTurn = false;
	c2.isTurn = true;
	return true; /* Characters still alive, game continues */
}

/** Takes two character, and character attacker and character c.
 *  Checks to make sure arguments are in correct order,
 *  then subtracts c's health by attacker's attack value.
 *  Called "melee" because there will be other attacks later.
 *  First argument inflicts damage upon second argument.
 *
 *  Calls nextTurn.
 */
void meleeAttack(Character attacker, Character c) {
	printf("%s attacks %s!\n", attacker.name, c.name);
	c.health -= attacker.attack;
	printf("%s took %u damage!", c.name, attacker.attack);
	nextTurn(attacker, c);
}

/** Output stats of character pointed to by c */
void status(Character c) {
	printf("%s's stats:\n\tHealth:%d/%d\n\tAttack:%d\n",
			c.name, c.health, STARTING_PLAYER_HEALTH, c.attack);
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

/** Call when input from player is required, c must be the player character, m the monster */
//@TODO work needed for this function to support multiple monsters/players
//@TODO make case insensitive, should be attack or ATTACK or AtTacK or A instead of just lower case
void input(Character c, Character m) {
	assert(c.isPlayerCharacter);
	char input[MAX_INPUT_LENGTH];
	bool isValidInput = true;
	scanf("Enter command: %s\n", input);

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
			printf("%s does absolutely nothing\n\n", c.name);
			nextTurn(c, m);
			isValidInput = true;
		}

		/* escape(exit): exits the program, maybe serve a practical purpose if adding ranged weapons/magic? */
		/* shortcut is "exit" instead of "e" to avoid accidental exit */
		else if(strcmp(input, "escape") == 0 || strcmp(input, "exit") == 0) {
			printf("%s runs away in shame\n\n", c.name);
			exit(0);
		}

		/* invalid command */
		else {
			printf("Invalid input, type help for possible commands\n");
			isValidInput = false;
		}
	} while(!isValidInput); /* ask user for input again if the last input was invalid */
}

/** Main function, calls the previous functions to make game */
//@TODO add levels in another file with their own function per level, have main call them
int main() {
	Character player = newPlayerCharacter();
	Character monster = newCharacter();
	while(player.health > 0 && monster.health > 0) {
		while(player.isTurn) {
			input(player, monster);
		}
		meleeAttack(monster, player);
	}
	return 0;
}
