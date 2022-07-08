#ifndef DEFS_H
#define DEFS_H

#include <stdarg.h>
#include <stdbool.h>

#include "constants.h"

/* utility.c */
void sleep_ms(int milliseconds);
int case_compare(const char *word1, const char *word2);
void getInput(char input[], char message[]);
bool yes_or_no(char message[]);
int eprintf(const char *format, ...);

/* character.c */
typedef struct Character {
	char name[MAX_INPUT_LENGTH + sizeof(C_BLUE) + sizeof(C_RESET)];
	char isMonster;
	bool isTurn;

	bool isKnight;
	unsigned char totalHealth;
	unsigned char totalMana;
	char health;
	unsigned char mana;
	unsigned char attack;
	unsigned char defense;

	Item itemSlot;
	Item potionSlot;
	bool knowSpell[SPELLS_IN_GAME];

	Effect effect; /* For stuff like poison or parry */
	Effect buff; /* For buffs (DEFENSE_UP or ATTACK_AND_HEALTH_UP */
	unsigned char effectDuration;
} Character;
Character* newCharacter(char message[], Enemy enemy);
void lvlUp(Character *c);

/* actions.c */
bool brand_check(Character *attacker, Character *c);
bool parry_check(Character *attacker, Character *c);
void damage_check(Character *c, char effectiveDamage);
void meleeAttack(Character *attacker, Character *c);
void parry(Character *c);
void status(Character *c);
void enemyStatus(Character *c, Character *m);
void help(Character *c);
void wait(Character *c);
void escape(Character *c, Character *m);
void actions(Character *c, Character *m);

/* potions.c */
void red_potion(Character *c, bool isGreater);
void blue_potion(Character *c, bool isGreater);
void panacea(Character *c);
void usePotion(Character *c, bool isInCombat);
void drink_potion(Character *c);

/* items.c */
void tears(Character *c);
void iron_pellet(Character *c);
void demon_fire(Character *user, Character *c);
void light_vial(Character *user, Character *c);
void horn(Character *c);
void useItem(Character *c, Character *m);
void buff_revert(Character *c);

/* spells.c */
void fireball(Character *caster, Character *c);
void lightning_stake(Character *caster, Character *c);
void summon_sheep(Character *caster, Character *c);
void sacrificial_brand(Character *caster, Character *c);
void frost_resonance(Character *caster, Character *c);
void castSpell(Character *c, Character *m);

/* combat.c */
void golem_slam(Character *m, Character *c);
void vampire_thrust(Character *m, Character *c);
void vampire_slash(Character *m, Character *c);
void bloop_reap(Character *m, Character *c);
void monsterAction(Character *m, Character *c);
void item_or_spell_found(Character *c, Item itemFound, char message[]);
void status_effect_check(Character *c);
bool check_monster_health(Character *c, Character *m, unsigned char levelUpNumber);
void check_player_health(Character *c, Character *m);
void combat_sequence(Character *c, Character *m, unsigned char levelUpNumber);

/* levels.c */
void lvl0(Character *c);
void lvl1(Character *c);
void lvl2(Character *c);
void lvl3(Character *c);
void lvl4(Character *c);
void lvl5(Character *c);
void the_end(Character *c);

/* saves.c */
int init_save(char *name);
int load_save(void);
int save(void);

#endif
