#include <stdio.h>

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

/** Creates a new player character */
*Character newPlayerCharacter(Character *c) {
    struct Character *c = malloc(sizeof(*c));
    c->health = STARTING_PLAYER_HEALTH;
    c->attack = STARTING_PLAYER_ATTACK;
    c->isTurn = true; /* Player gets the first turn */
    c->isPlayerCharacter = true;
    scanf("Enter the character's name: %s\n", c->name);
    printf("%s has %u health\n", c->name, c->health);
    printf("%s has %u attack power\n", c->name, c->attack);
    return c;
}

/** Creates a new monster character */
*Character newPlayerCharacter(Character *c) {
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
    printf("%s has been defeated\n", c->name);
    free(*c);
}

/** Takes two character, and character attacker and character c.
 *  Checks to make sure arguments are in correct order,
 *  then subtracts c's health by attacker's attack value.
 *  Called "melee" because there will be other attacks later.
 */
void meleeAttack(Character *attacker, Character *c) {
    printf("%s attacks\n", attacker->name);
    c->health -= attacker->attack;
    printf("%s took %u damage from %s!", c->name, attacker->attack, attacker->name);
}

/** Ends the turn, changing the state of isTurn for both characters.
  * Information displayed at the end of a player's turn is shown
  * if it is now the player's turn
  * NOTE: c's turn has just ended, m's turn is now beginning
 */
void nextTurn(Character *c, Character *m) {
    
}

/** Main function, calls the previous functions to make game */
int main() {
    return 0;
}
