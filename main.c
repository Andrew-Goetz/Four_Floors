#include <assert.h>
#include <stdbool.h> 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

/*********** Constants ************/

#define MAX_INPUT_LENGTH 30 /* No input greater than MAX_INPUT_LENGTH characters allowed */

/* Player's starting stats */
const unsigned char STARTING_PLAYER_HEALTH = 5;
const unsigned char STARTING_PLAYER_ATTACK = 1;

/* Monster's stats, will assign different ones for each monster type in future */
const unsigned char MONSTER_HEALTH = 3;
const unsigned char MONSTER_ATTACK = 1;
const char MONSTER_NAME[] = "The monster";


/** Defines a generic character */
typedef struct Characters {
	char health; /* character dead if(health <= 0) */
	//unsigned char mana;
	unsigned char attack;
	//char defense;
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

/** Creates a new player character, should only be called once until multiplayer is added */
Character* newPlayerCharacter() {
	/* Create character */
	Character *c = malloc(sizeof(*c));
	c->health = STARTING_PLAYER_HEALTH;
	c->attack = STARTING_PLAYER_ATTACK;
	c->isPlayerCharacter = true;
	c->isTurn = true; /* Controls turn for both characters, if false it's the monster's turn */
	getInput(c->name, "Enter your traveler's name: ");

	printf("%s has %u health.\n", c->name, c->health);
	printf("%s has %u attack power.\n", c->name, c->attack);
	printf("Type help(h) for how to play\n");

	printf("%s approaches the mansion. The moon shines brightly above.\n", c->name);
	sleep(1); /* Use these to wait a second before outputting next text */
	printf("What lies on the fourth floor?\n");
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
	printf("%s has %u health\n", c->name, c->health);
	printf("%s has %u attack power\n", c->name, c->attack);
	return c;
}

#if 0
/** Ends the turn, changing the state of isTurn for both characters.
 *  Also determines if one of the characters is dead, freeing memory if needed.
 *  Exits program if any character has died. //@TODO change this later to only player character
 */
bool nextTurn(Character *c1, Character *c2) {
	//@TODO if needed, need to rethink this so more than two characters can fight at once
	if(c1->health <= 0) {
		printf("%s has defeated %s!\n", c2->name, c1->name);
		if(c1->isPlayerCharacter) {
			printf("GAME OVER!\n");
			free(c1); free(c2);
			exit(0);
		} else {
			printf("VICTORY!\n");
			free(c1); free(c2);
			exit(0);
		}
	} else if(c2->health <= 0) {
		printf("%s has defeated %s!\n", c2->name, c1->name);
		if(c2->isPlayerCharacter) {
			printf("GAME OVER!\n");
			free(c1); free(c2);
			exit(0);
		} else {
			printf("VICTORY!\n");
			free(c1); free(c2);
			exit(0);
		}
	} 
	assert(c1->health > 0 && c2->health > 0);
	!c1->isTurn; !c2->isTurn;
	return true;
}
#endif
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
	sleep(1);
	printf("%s took %u damage!\n\n", c->name, attacker->attack);
//	nextTurn(attacker, c);
	if(attacker->isPlayerCharacter) attacker->isTurn = false;
}

/** Output stats of character pointed to by c */
void status(Character *c) {
	printf("%s's stats:\n\tHealth:%d/%d\n\tAttack:%d\n\n",
			c->name, c->health, STARTING_PLAYER_HEALTH, c->attack);
}

/** Output help info */
void help() {
	printf("Goal: defeat your foes and stay alive, reach the fourth floor:\n"
			"\thelp(h)\tlists possible commands\n"
			"\tattack(a)\tattack your foe\n"
			"\tstatus(s)\tlists out stats, i.e: health\n"
			"\twait(w)\tdaydream\n"
			"\tescape(exit)\tabandon your quest\n"
			"\n");
}

/** Nothing */
void wait(Character *c) {
	assert(c->isPlayerCharacter);
	printf("%s takes a break.\n\n", c->name);
	c->isTurn = false;
}

/** Exit */
void escape(Character *c, Character *m) {
	printf("%s runs away in shame.\n\n", c->name);
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
		//@TODO add argument for different types of attacks later, maybe this instead of dedicated magic command?
		else if(strcasecmp(input, "attack") == 0 || strcasecmp(input, "a") == 0) {
			meleeAttack(c, m);
			isValidInput = true;
		}

		/* status(a): check status of player character, c */
		else if(strcasecmp(input, "status") == 0 || strcasecmp(input, "s") == 0) {
			status(c);
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

/** When not the players turn, the monster does something: probably, it attacks */
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
	do {
		actions(player, monster);
		if(monster->health <= 0) {
			printf("VICTORY!\n");
			free(player); free(monster);
			return;
		}
		if(!player->isTurn) {
			monsterAction(monster, player);
		}
		if(player->health <= 0) {
			printf("You have been defeated!\n");
			free(player); free(monster);
			return;
		}
//		printf("state of isTurn: %d\n", player->isTurn);
	} while(player->health > 0 && monster->health);
	//printf("%c, then %c\n", player.name[5], player.name[6]);
}
//@TODO implement a save file in the main function, and perhaps an option to start a new game instead of that
int main() {
	Character *player = newPlayerCharacter(); /* Player created in main, monsters in the lvl functions */
	lvl0(player);
	return 0;
}
