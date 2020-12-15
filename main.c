#include <stdio.h>
#include <assert.h>

/*********** Constants ************/
/* PLayer's starting stats */
static const unsigned char STARTING_PLAYER_HEALTH = 5;
static const unsigned char STARTING_PLAYER_ATTACK = 1;

/* Monster's stats, will assign different ones for each monster type */
static const unsigned char MONSTER_HEALTH = 3;
static const unsigned char MONSTER_ATTACK = 1;
static const char MONSTER_NAME[] = "The monster";


/** Defines a generic character */
struct Character {
    unsigned char health;
//  unsigned char mana;
    unsigned char attack;
//  unsigned char defense;
    bool isTurn; /* True if character's turn, false if not */
    bool isPlayerCharacter
    char name[];
};

/** Creates a new player character, should only be called once until multiplayer is added */
*Character newPlayerCharacter() {
    struct Character *c = malloc(sizeof(*c));
    c->health = STARTING_PLAYER_HEALTH;
    c->attack = STARTING_PLAYER_ATTACK;
    c->isTurn = true; /* Player gets the first turn */
    c->isPlayerCharacter = true;
    scanf("Enter the character's name: %s\n", c->name); //@TODO should change to sscanf
    printf("%s has %u health\n", c->name, c->health);
    printf("%s has %u attack power\n", c->name, c->attack);
	printf("Type help(h) for how to play\n");
    return c;
}

/** Creates a new non-player character */
*Character newCharacter() {
    struct Character *c = malloc(sizeof(*c));
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
/** Free memory alloted for a charcter (in other words, character is dead) */
void freeCharacter(Character *c) {
    free(*c);
}

/** Takes two character, and character attacker and character c.
 *  Checks to make sure arguments are in correct order,
 *  then subtracts c's health by attacker's attack value.
 *  Called "melee" because there will be other attacks later.
 *  First argument inflicts damage upon second argument.
 *
 *  Calls nextTurn.
 */
void meleeAttack(Character *attacker, Character *c) {
    printf("%s attacks %s!\n", attacker->name, *c);
    c->health -= attacker->attack;
    printf("%s took %u damage!", c->name, attacker->attack);
	nextTurn(attacker, c);
}

/** Output stats of character pointed to by c */
void status(Character *c) {
	printf("%s's stats:\n\tHealth:%d/%d\n\tAttack:%d\n",
			c->name, c->health, STARTING_PLAYER_HEALTH, c->attack);
}

/** Output help info */
void help() {
	printf("Goal: defeat your foe and stay alive, reach the fourth floor:\n
			-help(h)\tlists commands\n
			-attack(a)\tattack your foe\n
			-status(s)\tlists out stats, i.e: health\n
			-wait(w)\tdaydream\n
			-escape(exit)\tabandon your quest\n
			\n");
}

/** Ends the turn, changing the state of isTurn for both characters.
  * Also determines if one of the characters is dead, calls freeCharacter and
  * exits program if the character has died, //@TODO change this later
  * NOTE: c1's turn has just ended, c2's turn is now beginning,
  * argument order matter
  *
  * Calls freeCharacter if health is <= 0
 */
void nextTurn(Character *c1, Character *c2) {
    //@TODO if needed, need to rework this so more than two characters can fight at once
    assert(c1->isTurn && !c2->isTurn); /* make sure arguments are in correct order */

    if(c1->health <= 0) {
        printf("%s has died!\n", c1->name);
        if(c1->isPlayerCharacter) {
            printf("GAME OVER!\n");
			freeCharacter(c1);
            exit(0);
        } else {
            printf("%s is victorious!\n", c2->name);
			freeCharacter(c1);
            exit(0);
        }
    } else if(c2->health <= 0) {
        printf("%s has died!\n", c2->name);
        if(c2->isPlayerCharacter) {
            printf("GAME OVER!\n");
			freeCharacter(c2);
            exit(0);
        } else {
            printf("%s is victorious!\n", c1->name);
			freeCharacter(c2);
            exit(0);
        }
    } else {
        assert(c1->health > 0 && c2->health > 0);
        c1->isTurn = false;
        c2->isTurn = true;
    }
}

/** Call when input from player is required, c must be the player character, m the monster */
//@TODO work needed for this function to support multiple monsters/players
//@TODO make case insensitive, should be attack or ATTACK or AtTacK or A instead of just lower case
void input(Character *c, Character *m) {
    assert(c->isPlayerCharacter);
    char input[];
	bool isValidInput = true;
	scanf("Enter command: %s\n", input);
	
	/***** Commands: *****/
	do {
		/* help(h): lists out possible commands and a little how to play */
			help();
			isValidInput = true;
		}

		/* attack(a): calls */
		else if(strcmp(input, "attack") || strcmp(input, "a")) {
			
			isValidInput = true;
		}

		/* status(a): check status of player character, c */
		//@TODO add argument to status to check status of monster as well, will support multiple characters
		else if(strcmp(input, "status") || strcmp(input, "s")) {
			status(Character(c));
			isValidInput = true;
		}

		/* wait(w): do nothing */	
		else if(strcmp(input, "wait") || strcmp(input, "w")) {
			printf("%s does absolutely nothing\n\n", c->name);
			isValidInput = true;
		}

		/* escape(exit): exits the program, maybe serve a practical purpose if adding ranged weapons/magic? */
		else if(strcmp(input, "wait") || strcmp(input, "w")) {
			printf("%s runs away in shame\n\n", c->name);
			freeCharacter(c); freeCharacter(m);
			exit(0);
		}

		/* invalid command */
		else {
			printf("Invalid input, type help for possible commands\n");
			isValidInput = false;
		}
	} while(!isValidInput);
}

/** Main function, calls the previous functions to make game */
//@TODO add levels in another file with their own function per level, have main call them
int main() {
	Character *player = newPlayerCharacter();
	Character *monster = newCharacter();
    return 0;
}
