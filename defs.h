#ifndef DEFS_H
#define DEFS_H

#include <stdbool.h>

#include "constants.h"

// utility.c
void sleep_ms(int milliseconds);
int case_compare(const char *word1, const char *word2);
void getInput(char input[], char message[]);
void pressEnter(void);
bool yes_or_no(char message[]);

// character.c
typedef struct Character {
	char name[MAX_INPUT_LENGTH];
	char isMonster;
	bool isTurn;

	unsigned char totalHealth;
	unsigned char totalMana;
	char health;
	unsigned char mana;
	unsigned char attack;
	unsigned char defense;

	Item itemSlot;
	Item potionSlot;
	bool knowSpell[SPELLS_IN_GAME];

	Effect effect;
	unsigned char effectDuration;
} Character;
Character* newCharacter(char message[], Enemy enemy);
void lvlUp(Character *c);

// actions.c
void meleeAttack(Character *attacker, Character *c);
void status(Character *c);
void enemyStatus(Character *c, Character *m);
void help(void);
void wait(Character *c);
void escape(Character *c, Character *m);
Effect actions(Character *c, Character *m);

// potions.c
void red_potion(Character *c, bool isGreater);
void blue_potion(Character *c, bool isGreater);
void blue_potion(Character *c, bool isGreater);
void panacea(Character *c);
void usePotion(Character *c, bool isInCombat);
void drink_potion(Character *c);

// items.c
Effect tears(Character *c);
Effect iron_pellet(Character *c);
void demon_fire(Character *user, Character *c);
Effect light_vial(Character *user, Character *c);
void horn(Character *user, Character *c);
Effect useItem(Character *c, Character *m);

// spells.c
void fireball(Character *caster, Character *c);
void lightning_stake(Character *caster, Character *c);
void summon_sheep(Character *caster, Character *c);
Effect sacrificial_brand(Character *caster, Character *c);
Effect frost_resonance(Character *caster, Character *c);
Effect castSpell(Character *c, Character *m);

// combat.c
void monsterAction(Character *m, Character *c);
void item_or_spell_found(Character *c, Item itemFound, char message[]);
void status_effect_check(Character *c, unsigned char turn_number);
void combat_sequence(Character *c, Character *m, unsigned char levelUpNumber);

// levels.c
void lvl0(Character *c);
void lvl1(Character *c);
void lvl2(Character *c);
void lvl3(Character *c);
void lvl4(Character *c);
void lvl5(Character *c);
void the_end(Character *c);

// saves.c
//TODO

#endif
