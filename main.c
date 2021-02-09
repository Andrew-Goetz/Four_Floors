#define _CRT_SECURE_NO_WARNINGS /* Removes depreciated warnings for strcpy, I want it for linux compatibility in future */
#include <assert.h>
#include <stdbool.h> 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
//#include <unistd.h>
#include <windows.h>
/* sleep() from unistd.h and _stricmp() from string.h are non-portable for other OSes */

//#ifdef _WINDOWS
//#include <windows.h>
//#define _stricmp _stricmp
//#define sleep Sleep
//#else
//#include <unistd.h>
//#endif
/*********** Constants ************/

#define MAX_INPUT_LENGTH 30 /* No input greater than MAX_INPUT_LENGTH characters allowed */
#define SPELLS_IN_GAME 5
#define MONSTERS_IN_GAME 7 /* Includes player character */
#define SLEEP_DURATION 750 /* Amount of time that passes, in ms, whenever Sleep is called */

/* Below goes in order: health, mana, attack, defense */
static const int MONSTER_STATS[MONSTERS_IN_GAME][4] = {
	/* Player Character */	5, 3, 2, 1,
	/* Beast */ 			5, 0, 2, 0,
	/* Killer Plant */ 		9, 0, 2, 1, /* Almost 1 shot by fireball */
	/* Wraith */ 			8, 2, 3, 1, /* Almost 1 shot by light vial */
	/* Mad Wizard */ 		7, 5, 1, 0, /* Immune to magic, casts a lot of spells */
	/* Wizard's Golem */ 	10, 0, 4, 3, /* All physical damage so iron pellet good against him */
	/* Vampire Lord */		9, 3, 4, 2
};

static const char *MONSTER_NAMES[MONSTERS_IN_GAME] = {
	"ERROR", "The Beast", "The Killer Plant", "The Wraith", "The Mad Wizard", "The Wizard's Golem", "The Vampire Lord"
};

/* To make accessing above array less annoying */
typedef enum STATS {
	HEALTH,
	MANA,
	ATTACK,
	DEFENSE
} Stat;

typedef enum ENEMY_TYPES {
	PLAYER,
	BEAST,
	KILLER_PLANT,
	WRAITH,
	MAD_WIZARD,
	GOLEM,
	VAMPIRE_LORD
} Enemy;

typedef enum STATUS_EFFECTS { /* Many relate to spells/items below */
	/* None(0) is the default status effect */
	NONE,
	/*  */
	STUN,
	POISON,
	DRAINED,
	DEFENSE_UP,
	ATTACK_UP
} Effect;

typedef enum ITEMS_AND_SPELLS {
	/* Nothing(0) is the default inventory and spell slot value */
	NOTHING,
	/* Spells(1-5) */
	FIREBALL,
	LIGHTNING_STAKE,
	SUMMON_SHEEP,
	SACRIFICIAL_BRAND,
	FROST_RESONANCE,
	/* BuffItems(6-10) */
	RED_POTION,
	GREATER_RED_POTION,
	BLUE_POTION,
	GREATER_BLUE_POTION,
	PANACEA,
	/* Items(11-15) */
	TEARS,
	IRON_PELLET,
	DEMON_FIRE,
	LIGHT_VIAL,
	HORN_OF_SAUL
} Item;

/* Item/spell descriptions and names, be precise about health and mana info but not specific on attack/defense effects */
static const char *ITEM_AND_SPELL_NAMES[16] = {
	"Nothing",
	"Fireball", "Lightning Stake", "Summon Sheep", "Sacrificial Brand", "Frost Resonance",/* Spells */
	"Red Potion", "Greater Red Potion", "Blue Potion", "Greater Blue Potion", "Panacea", /* PotionItems */
	"Vial of Tears", "Iron Pellet", "Vial of Demon Fire", "Light Vial", "Horn of Saul" /* Items */
};
static const char *ITEM_AND_SPELL_DESCRIPTIONS[16] = {
	"Find powerful items and potions in the mansion.\n",

	"The magic of the southern deserts. The fire radiates with a blue, magical energy.\nLaunches a fireball at enemies. Consumes 1 mana.\n",
	"The magic of the dragon slayers of old.\nSmash lightning into the earth, shocking surroundings.\n",
	"The magic of the mad wizard Pizel.\nSummons a sheep. The magic is unstable, so the sheep may explode.\n",
	"The magic of the flagellants.\n When used, the enemy is instantly defeated, but the health of the caster is reduced to 1.\n",
	"The magic of the northern icemen.\nEnvelops enemy in frost, dealing damage overtime.\n",

	"A red, slimey liquid. Doesn't taste as good as it looks.\nRestores 3 health when used.\n",
	"A red, slimey liquid. Tastes stronger than the regular potion.\nRestores 5 health when used.\n",
	"A blue, cold liquid. Is viscous, like syrup.\nRestores 3 mana when used.\n",
	"A blue, cold liquid. Even thicker than the regular potion.\nRestores 5 mana when used.\n",
	"The cure-all spoken of in ancient legends. The potion glimmers a brilliant gold.\nRemoves any negative status effects, such as poison.\n",

	"The tears of a fallen hero and his broken promise.\nUsing immediately before a monster deals fatal damage restores health to full.\n",
	"A foul tasting, chewy medicine. Used by warriors from the east.\nUsing makes one's skin solid as iron for a short time, increasing defense.\n",
	"The flames of ancient demons, captured in a vial by coastal wizards.\nSmashing this vial will make enemies erupt into flames.\n",
	"A drop of pure sunlight, captured in a vial by coastal wizards.\nSmashing this vial will drown a room in the light of day, blinding enemies.\n",
	"A horn once sounded by Saul, a servant of the gods.\nBlowing this horn will shatter it, temporarily granting its user the strength of the gods.\n"
};

/** Call to get input, formats to get rid of trailing \n if it exists */
void getInput(char input[], char message[]) {
	printf("%s", message);
	fgets(input, MAX_INPUT_LENGTH, stdin);
	assert(strlen(input) > 0);
	if(input[strlen(input)-1] == '\n') {
		input[strlen(input)-1] = '\0';
	}
}

/** https://stackoverflow.com/questions/1406421/press-enter-to-continue-in-c 
 *  Also important: don't use \n at end of printf when pressEnter() is called immediately after,
 *  because the user pressing enter already goes to the next line.
 */
void pressEnter() {
	//printf("%s", message);
	while(getchar() != '\n');
}

/** Ask simple yes or no questions to user.
 *  If yes then return true, if no then return false.
 */
bool yes_or_no(char message[]) {
	char input[MAX_INPUT_LENGTH];
	printf("%s", message); /* assumes message will end with \n */
	while(true) {
		getInput(input, "Yes(y) or No(n): ");
		if(_stricmp(input, "yes") == 0 || _stricmp(input, "y") == 0) { //|| _stricmp(input, "\n") == 0) { /* \n works too for yes */
			return true;
		} else if(_stricmp(input, "no") == 0 || _stricmp(input, "n") == 0) {
			return false;
		} else {
			printf("Invalid input.\n");
		}
	}
}

/** Defines a generic character */
typedef struct Characters {
	char totalHealth;
	char totalMana;
	Item itemSlot;
	Item potionSlot;
	Effect effect;
	unsigned char effectDuration;

	char health; /* character dead if(health <= 0) */
	unsigned char mana; /* Negative mana not allowed, if not enough mana cannot cast a spell */
	unsigned char attack;
	unsigned char defense; /* no negative defense, total_damage = attacker's_attack - defender's_defense */
	char isMonster; /* 0 == is the player character, any other value corresponds to enemy enum */
	bool isTurn;
	bool knowSpell[SPELLS_IN_GAME]; /* If true, character knows that spell, corresponds to the order of the spells in the enum */
	char name[MAX_INPUT_LENGTH];
} Character;

/** Creates a new character, some characters different than others. */
Character* newCharacter(char message[], Enemy enemy) {
	Character *c = malloc(sizeof(*c));

	c->health = MONSTER_STATS[enemy][HEALTH];
	c->totalHealth = MONSTER_STATS[enemy][HEALTH];
	c->mana = MONSTER_STATS[enemy][MANA];
	c->totalMana = MONSTER_STATS[enemy][MANA];
	c->attack = MONSTER_STATS[enemy][ATTACK];
	c->defense = MONSTER_STATS[enemy][DEFENSE];

	c->itemSlot = NOTHING;
	c->potionSlot = NOTHING;
	for(unsigned char i = 0; i < SPELLS_IN_GAME; i++) c->knowSpell[i] = false;

	c->effect = NONE;
	c->effectDuration = 0;

	c->isMonster = enemy;
	if(enemy == PLAYER) {
		getInput(c->name, "Enter your traveler's name: ");
	} else {
		strcpy(c->name, MONSTER_NAMES[enemy]);
		printf("%s appears!\n", c->name);
	}
	return c;
}

/** Increases the stat of the player's choice, should be called whenever a monster is defeated */
void lvlUp(Character *c) {
	assert(!c->isMonster);
	printf("Level up avaliable!\nLevel health(h), mana(m), attack(a), or defense(d)?\n");
	char input[MAX_INPUT_LENGTH];
	while(true) {
		getInput(input, ">> ");
		if(_stricmp(input, "health") == 0 || _stricmp(input, "h") == 0) {
			c->totalHealth++; c->health++;
			printf("%s feels healthier.\n", c->name);
			return;
		} else if(_stricmp(input, "mana") == 0 || _stricmp(input, "m") == 0) {
			c->totalMana++; c->mana++;
			printf("%s feels more intelligent.\n", c->name);
			return;
		} else if(_stricmp(input, "attack") == 0 || _stricmp(input, "a") == 0) {
			c->attack++;
			printf("%s feels more powerful.\n", c->name);
			return;
		} else if(_stricmp(input, "defense") == 0 || _stricmp(input, "d") == 0) {
			c->defense++;
			printf("%s feels stronger.\n", c->name);
			return;
		} else {
			printf("Invalid input.\n");
		}
	}
}

/** Takes two character, and character attacker and character c.
 *  Checks to make sure arguments are in correct order,
 *  then subtracts c's health by attacker's attack value.
 *  First character (attacker) deals damage to second character (c).
 */
void meleeAttack(Character *attacker, Character *c) {
	printf("%s attacks %s!\n", attacker->name, c->name);
	Sleep(SLEEP_DURATION);
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
	for(int i = 0; i < SPELLS_IN_GAME; i++) {
		if(c->knowSpell[i]) {
			anySpells = true;
			break;
		}
	}
	if(anySpells) {
		printf("%s knows the following spells:\n", c->name);
		if(c->knowSpell[0]) { // FIREBALL
			printf("\tFireball(f)\n");
		} if(c->knowSpell[1]) { // LIGHTNING_STAKE
			printf("\tLightning Stake(L)\n");
		} if(c->knowSpell[2]) { // SUMMON_SHEEP
			printf("\tSummon Sheep(s)\n");
		} if(c->knowSpell[3]) { // SACRIFICIAL_BRAND
			printf("\tSacrificial Brand(b)\n");
		} if(c->knowSpell[4]) { // FROST_RESONANCE
			printf("\tFrost Resonance(r)\n");
		}
	}
}

/** Output status of enemy character, calls status */
void enemyStatus(Character *m) {
	assert(m->isMonster);
	static const char *MONSTER_HINTS[MONSTERS_IN_GAME] = {
		"ERROR\n", /* Character gets no hint */
		"has a large scar across its hairy chest. It must be weak to physical attacks!\n", /* The Beast */
		"seems vulnerable to fire.\n", /* The Killer Plant */
		"is a creature of the night. The sun's rays would prove fatal.\n", /* The Wraith */
		"is a powerful wizard, yet also an old man. melee attacks would be powerful against him.\n", /* The Wizard */
		"is an unthinking creature of destruction; attacking with massive physical blows, defense must be powerful against it!\n", /* The Wizard's Golem */
		"is the ultimate foe. He has no obvious weakness weakness.\n" /* The Vampire Lord */
	};
	status(m);
	printf("%s %s", MONSTER_NAMES[m->isMonster], MONSTER_HINTS[m->isMonster]); //prints out hints/descriptions here
}

/** Output help info */
void help() {
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
	Sleep(SLEEP_DURATION);
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
				"\tAt least 1 level up occurs after each battle; the game automatically saves when this occurs.\n");
				//"\t\n"
	} else {
		printf("Ok then.\n");
	}
}

/***** Potion Functions *****/
void red_potion(Character *c, bool isGreater) {
	char healVal;
	if(isGreater) {
		healVal = 5;
	} else {
		healVal = 3;
	}
	printf("%s drinks the %s.\n", c->name, ITEM_AND_SPELL_NAMES[c->potionSlot]);
	Sleep(SLEEP_DURATION);
	if(c->health + healVal >= c->totalHealth) {
		c->health = c->totalHealth;
		printf("%s is now full health.\n", c->name);
	} else {
		c->health += healVal;
		printf("%s has healed %d health.\n", c->name, healVal);
	}
}

void blue_potion(Character *c, bool isGreater) {
	char manaVal;
	if(isGreater) {
		manaVal = 5;
	} else {
		manaVal = 3;
	}
	printf("%s drinks the %s.\n", c->name, ITEM_AND_SPELL_NAMES[c->potionSlot]);
	Sleep(SLEEP_DURATION);
	if(c->mana + manaVal >= c->totalMana) {
		c->mana = c->totalMana;
		printf("%s is now full mana.\n", c->name);
	} else {
		c->mana += manaVal;
		printf("%s has restored %d mana.\n", c->name, manaVal);
	}
}

void panacea(Character *c) {
	printf("%s drinks the %s. It tastes incredible.\n", c->name, ITEM_AND_SPELL_NAMES[c->potionSlot]);
	Sleep(SLEEP_DURATION);
	//@TODO whenever status afflictions are implemented, remove only harmful afflictions
}

/** Use potion in potionSlot */
void usePotion(Character *c, bool isInCombat) {
	assert(!c->isMonster);
	switch(c->potionSlot) {
		case NOTHING:
			printf("There is no potion in inventory!\n");
			return; //Don't end turn here
		case RED_POTION:
			red_potion(c, false);
			break;
		case GREATER_RED_POTION:
			red_potion(c, true);
			break;
		case BLUE_POTION:
			blue_potion(c, false);
			break;
		case GREATER_BLUE_POTION:
			blue_potion(c, true);
			break;
		case PANACEA:
			panacea(c);
			break;
		default:
			printf("The item had no effect. Must have been a dud!\n");
			printf("\n\n%d\n\n\n", c->potionSlot);
			break;
	}
	c->potionSlot = NOTHING;
	if(isInCombat) c->isTurn = false;
}

/***** Item Functions *****/
void tears(Character *c) {
	printf("%s drinks the %s. %s glows warmly.\n", c->name, ITEM_AND_SPELL_NAMES[c->itemSlot], c->name);
	//@TODO needs to toggle some status for the character that only lasts a turn
}

void iron_pellet(Character *c) {
	printf("%s swallows the %s, hardening the skin.\n", c->name, ITEM_AND_SPELL_NAMES[c->itemSlot]);
	//@TODO increase defense a decent amount for 3 turns
}

void demon_fire(Character *user, Character *c) {
	printf("%s throws a %s at %s, making the room erupt in flames.\n", user->name, ITEM_AND_SPELL_NAMES[user->itemSlot], c->name);
	Sleep(SLEEP_DURATION);
	if(c->isMonster == KILLER_PLANT) {
		const char DEMON_FIRE_DAMAGE = c->totalHealth; //gets oneshot
		c->health -= DEMON_FIRE_DAMAGE;
		printf("%s burns in the fires of hell, draining it of all life energy.\n", c->name);
	} else {
		const char DEMON_FIRE_DAMAGE = 5;
		c->health -= DEMON_FIRE_DAMAGE;
		printf("%s is severely burned, taking %d damage!\n", c->name, DEMON_FIRE_DAMAGE);
	}
	if(user->isMonster) {
		user->itemSlot = NOTHING;
	}
}
/* Stuns enemy for two turns */
void light_vial(Character *user, Character *c) {
	assert(!user->isMonster);
	if(c->isMonster == WRAITH) {
		const char LIGHT_VIAL_DAMAGE = 5;
	} else {
		printf("%s throws a %s at %s, blinding %s.\n", user->name, ITEM_AND_SPELL_NAMES[user->itemSlot], c->name, c->name);
	}
}

void horn(Character *user, Character *c) {
	assert(!user->isMonster);
}

/** Use item in itemSlot */
void useItem(Character *c, Character *m) {
	assert(!c->isMonster);
	switch(c->itemSlot) {
		case NOTHING:
			printf("There is no item in inventory!\n");
			return; // Don't end turn here
		case TEARS:
			tears(c);
			break;
		case IRON_PELLET:
			iron_pellet(c);
			break;
		case DEMON_FIRE:
			demon_fire(c, m);
			break;
		case LIGHT_VIAL:
			light_vial(c, m);
			break;
		case HORN_OF_SAUL:
			horn(c, m);
			break;
		default:
			printf("The item had no effect. Must have been a dud!\n");
			break;
	}
	c->itemSlot = NOTHING;
	c->isTurn = false;
}

/***** Spell Functions *****/
void fireball(Character *caster, Character *c) {
	printf("%s casts fireball!\n", caster->name);
	Sleep(SLEEP_DURATION);
	if(c->isMonster == KILLER_PLANT) {
		const char FIREBALL_DAMAGE = 7;
		c->health -= FIREBALL_DAMAGE;
		printf("%s contorts in intense pain, taking %d damage!\n", c->name, FIREBALL_DAMAGE);
	} else {
		const char FIREBALL_DAMAGE = 3;
		c->health -= FIREBALL_DAMAGE;
		printf("%s burns from the flames, taking %d damage!\n", c->name, FIREBALL_DAMAGE);
	}
}
void lightning_stake(Character *caster, Character *c) {
	//@TODO
}
void summon_sheep(Character *caster, Character *c) {
	//@TODO
}
void sacrificial_brand(Character *caster, Character *c) {
	//@TODO
}
void frost_resonance(Character *caster, Character *c) {
	//@TODO
}

/** Cast */
void castSpell(Character *c, Character *m) {
	assert(!c->isMonster);
	char isMagicUser = 0;
	char firstSpell = 0; /* used if(isMagicUser == 1) */
	for(int i = 0; i < SPELLS_IN_GAME; i++) {
		if(c->knowSpell[i]) {
			isMagicUser++;
			firstSpell = i + 1; /* corresponds to value of spell in the enum */
		}
	}
	/* Need to check knowSpell during input too, make sure user doesn't guess a spell they don't know */
	if(isMagicUser == 1) { // cast whatever 1 spell the player knows
		switch(firstSpell) {
			case FIREBALL:
				fireball(c, m);
				break;
			case LIGHTNING_STAKE:
				lightning_stake(c, m);
				break;
			case SUMMON_SHEEP:
				summon_sheep(c, m);
				break;
			case SACRIFICIAL_BRAND:
				sacrificial_brand(c, m);
				break;
			case FROST_RESONANCE:
				frost_resonance(c, m);
				break;
			default:
				printf("Something goes wrong and %s fails to cast the spell!", c->name);
				break;
		}
	} else if(isMagicUser > 1) {
		printf("What spell to cast?\n");
		if(c->knowSpell[0]) { // FIREBALL
			printf("Fireball(f)? ");
		} if(c->knowSpell[1]) { // LIGHTNING_STAKE
			printf("Lightning Stake(L)? ");
		} if(c->knowSpell[2]) { // SUMMON_SHEEP
			printf("Summon Sheep(s)? ");
		} if(c->knowSpell[3]) { // SACRIFICIAL_BRAND
			printf("Sacrificial Brand(b)? ");
		} if(c->knowSpell[4]) { // FROST_RESONANCE
			printf("Frost Resonance(r)? ");
		}
		printf("\n");
		char input[MAX_INPUT_LENGTH];
		while(true) {
			getInput(input, ">> ");
			if(c->knowSpell[0] && (_stricmp(input, "Fireball") == 0 || _stricmp(input, "f") == 0)) {
				fireball(c, m);
				break;
			} else if(c->knowSpell[1] && (_stricmp(input, "Lightning Stake") == 0 || _stricmp(input, "L") == 0)) {
				lightning_stake(c, m);
				break;
			} else if(c->knowSpell[2] && (_stricmp(input, "Summon Sheep") == 0 || _stricmp(input, "s") == 0)) {
				summon_sheep(c, m);
				break;
			} else if(c->knowSpell[3] && (_stricmp(input, "Sacrificial Brand") == 0 || _stricmp(input, "b") == 0)) {
				sacrificial_brand(c, m);
				break;
			} else if(c->knowSpell[4] && (_stricmp(input, "Frost Resonance") == 0 || _stricmp(input, "r") == 0)) {
				frost_resonance(c, m);
				break;
			} else {
				printf("Invalid input.\n");
			}
		}
	} else {
		printf("%s tries to cast magic, but doesn't know how. %s chuckles.\n", c->name, m->name);
		Sleep(SLEEP_DURATION);
	}
	c->isTurn = false;
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
	while(true) {
		getInput(input, ">> ");
		/* help(h): lists out possible commands and then asks if user wants more in depth information */
		if(_stricmp(input, "help") == 0 || _stricmp(input, "h") == 0) {
			help();
			return;
		}
		/* status(s): outputs current player status */
		if(_stricmp(input, "status") == 0 || _stricmp(input, "s") == 0) {
			status(c);
			return;
		}
		/* enemy(e): outputs non-player-character's status, a help fight him */
		if(_stricmp(input, "enemy") == 0 || _stricmp(input, "e") == 0) {
			enemyStatus(m);
			return;
		}
		/* attack(a): calls meleeAttack */
		else if(_stricmp(input, "attack") == 0 || _stricmp(input, "a") == 0) {
			meleeAttack(c, m);
			return;
		}
		/* potion(p): use potion item currently in player's potionSlot */
		else if(_stricmp(input, "potion") == 0 || _stricmp(input, "p") == 0) {
			usePotion(c, true);
		}
		/* item(i): use item currently in player's itemSlot */
		else if(_stricmp(input, "item") == 0 || _stricmp(input, "i") == 0) {
			useItem(c, m);
			return;
		}
		/* cast(c): cast whatever magic is in player's magic slot */
		else if(_stricmp(input, "cast") == 0 || _stricmp(input, "c") == 0) {
			castSpell(c, m);
			return;
		}
		/* wait(w): do nothing */
		else if(_stricmp(input, "wait") == 0 || _stricmp(input, "w") == 0) {
			wait(c);
			return;
		}
		/* escape(exit): exits the program */
		/* shortcut is "exit" instead of "e" to avoid accidental exits */
		else if(_stricmp(input, "escape") == 0 || _stricmp(input, "exit") == 0) {
			escape(c, m);
			exit(0);
		}
		/* ask user for input again if the last input was invalid */
		else {
			printf("Invalid input, type help(h) for possible commands.\n");
		}
	} 
}

/** When not the players turn, the monster does something: as of right now, it will always attack */
void monsterAction(Character *m, Character *c) {
	if(!c->isTurn) {
		Sleep(SLEEP_DURATION);
		meleeAttack(m, c);
	}
	c->isTurn = true;
}

/** Ask user to equip item, buffitem, or spell found.
 *  Pass in player character, particular item that is found, and message.
 *  Whether "Item" is a Spell, potion, or Item will be determined in the function.
 *  If item, needs to go in itemSlot; if potion, needs to go in potionSlot, spells stored in knowSpell.
 *  message should have a trailing \n.
 */
void item_or_spell_found(Character *c, Item itemFound, char message[]) {
	assert(!c->isMonster);
	// printf("\nTEST: %s: %s\n\n", ITEM_AND_SPELL_NAMES[itemFound], ITEM_AND_SPELL_DESCRIPTIONS[itemFound]);
	printf("%sPress enter to read its description:", message); pressEnter();
	printf("%s", ITEM_AND_SPELL_DESCRIPTIONS[itemFound]);
	bool isYes;
	switch(itemFound) { /* Never going to find nothing so skip 0 */
		/* for spells, add to knowSpell */
		case FIREBALL: case LIGHTNING_STAKE: case SUMMON_SHEEP: case SACRIFICIAL_BRAND: case FROST_RESONANCE:
			printf("Learn %s?", ITEM_AND_SPELL_NAMES[itemFound]);
			isYes = yes_or_no("\n");
			if(isYes) {
				c->knowSpell[itemFound - 1] = true; //-1 needed since 0 is NOTHING, bad but w/e
				printf("After reading the scroll, %s figures out how to cast %s.\n", c->name, ITEM_AND_SPELL_NAMES[itemFound]);
			} else {
				printf("Ok then.\n");
			}
			break;
		/* for potions, offer to use item in inventory and take new item */
		case RED_POTION: case GREATER_RED_POTION: case BLUE_POTION:	case GREATER_BLUE_POTION: case PANACEA:
			printf("Add %s to potion inventory?", ITEM_AND_SPELL_NAMES[itemFound]);
			if(!c->potionSlot) {
				isYes = yes_or_no("\n");
			} else {
				printf("%s", ITEM_AND_SPELL_NAMES[c->potionSlot]);
				isYes = yes_or_no(" will be used before being discarded.\n");
			}
			if(isYes) {
				if(c->potionSlot) {
					usePotion(c, false);
				}
				c->potionSlot = itemFound;
				printf("%s is now in potion inventory\n", ITEM_AND_SPELL_NAMES[c->potionSlot]);
			} else {
				if(c->potionSlot) {
					printf("%s remains in potion inventory.\n", ITEM_AND_SPELL_NAMES[c->potionSlot]);
				} else {
					printf("Ok then.\n");
				}
			}
			break;
		/* for items, offer to discard current item in inventory if one exists and pick up found one */
		case TEARS: case IRON_PELLET: case DEMON_FIRE: case LIGHT_VIAL: case HORN_OF_SAUL:
			printf("Add %s to item inventory?", ITEM_AND_SPELL_NAMES[itemFound]);
			if(!c->itemSlot) {
				isYes = yes_or_no("\n");
			} else {
				printf("%s", ITEM_AND_SPELL_NAMES[c->itemSlot]);
				isYes = yes_or_no(" will be discarded.\n");
			}
			if(isYes) {
				if(c->itemSlot) {
					printf("%s is discarded, and ", ITEM_AND_SPELL_NAMES[c->itemSlot]);
				}
				printf("%s is put into item inventory.\n", ITEM_AND_SPELL_NAMES[itemFound]);
				c->itemSlot = itemFound;
			} else {
				if(c->itemSlot) {
					printf("%s remains in item inventory.\n", ITEM_AND_SPELL_NAMES[c->itemSlot]);
				} else {
					printf("Ok then.\n");
				}
			}
			break;
		default: //in case something weird happens
			printf("Something goes wrong. It must have been an illusion!\n");
	}
}
/** Quick function that asks if player wants to drink potion inbetween battles. */
void drink_potion(Character *c) {
	printf("%s looks around and sees no enemies; safety, for the time being.", c->name); pressEnter();
	if(!c->potionSlot) {
		printf("It would be the perfect time to drink a potion; unfortunately, %s's potion slot is empty!", c->name); pressEnter();
		return;
	}
	printf("%s currently has %d/%d health and %d/%d mana.\n", c->name, c->health, c->totalHealth, c->mana, c->totalMana);
	printf("Drink the %s in inventory while out of combat?", ITEM_AND_SPELL_NAMES[c->potionSlot]);
	bool isYes = yes_or_no("\n");
	if(isYes) {
		usePotion(c, false);
	} else {
		printf("Ok then.\n");
	}
}

void status_effect_check(Character *c, Character *m, unsigned char turn_number) {
	printf("");
}

/** Function called once each level when combat is in progress.
 *  c is the player character, m is the monster, and levelUpNumber is the 
 *	number of times the lvlUp function will be called when m is defeated.
 *  Also keeps track of turn number so that status effects can be applied properly.
 */
void combat_sequence(Character *c, Character *m, unsigned char levelUpNumber) {
	assert(!c->isMonster && m->isMonster);
	unsigned char turn_number = 0;
	bool isTurnChanged;
	while(true) {
		actions(c, m);
		if(m->health <= 0) {
			printf("VICTORY!\n");
			for(; levelUpNumber != 0; levelUpNumber--) {
				lvlUp(c);
			}
			c->isTurn = true;
			break;
		}
		bool isTurnChanged = c->isTurn;
		monsterAction(m, c);
		if(c->health <= 0) {
			printf("%s has been defeated!\n", c->name);
			free(m); free(c); exit(0);
		}
		/* want to make sure turn_number not incremented on help or other non-isTurn-changing instructions */
		if(isTurnChanged != c->isTurn) {
			turn_number++;
			// printf("\n%d\n\n", turn_number);
		}
	}
	free(m);
}
void lvltest(Character *c) {

}
/* Floor 1 start */
void lvl0(Character *c) {
	printf("Press enter to advance through dialogue."); pressEnter();
	printf("A forest of trees surrounds a clearing; it is here that a massive, four-floor mansion, stands."); pressEnter();
	printf("%s wonders: what might lie on the fourth floor?", c->name); pressEnter();
	printf("%s reaches the massive front doors of the mansion.\n", c->name);
	bool isYes = yes_or_no("Enter the mansion, beginning a perilous journey?\n");
	if(!isYes) {
		printf("Intimidated by the mansion, %s turns around and heads home. Maybe it's for the best.\n", c->name);
		free(c); exit(0);
	}
	printf("The doors creak open. %s enters the mansion.", c->name); pressEnter();
	printf("Light streams into the mansion, revealing the dust floating in the air."); pressEnter();
	printf("SLAM! The door closes behind %s!", c->name); pressEnter();
	printf("%s turns and hears unnatural growls.", c->name); pressEnter();
	Character *m = newCharacter(" appears!\nType help(h) for how to fight!", BEAST);
	combat_sequence(c, m, 1);
}
void lvl1(Character *c) {
	printf("Looking at %s on the ground, %s notices a strange brand near the beast's torso. "
		   "Maybe it's important.", MONSTER_NAMES[BEAST], c->name); pressEnter();
	printf("With the first foe defeated, it is time to move on! Press enter to continue deeper into the mansion."); pressEnter();
	item_or_spell_found(c, RED_POTION, "After walking down a hallway, a potion can be seen sitting on a bookshelf.\n");
	drink_potion(c);
	printf("%s enters a massive room with another hallway branching off to the left.", c->name); pressEnter();
	bool isYes = yes_or_no("Take this other path?\n");
	if(isYes) {
		printf("Something about this left hallway seems special. %s heads left and leaves the room.", c->name); pressEnter();
		printf("The hallway continues for a time before making an abrupt right turn. Press enter to see what's around the corner."); pressEnter();
		item_or_spell_found(c, FIREBALL, "On a small table on the side of the hall, there is a magical scroll!\n");
		printf("At the end of the hallway there is a door. Press enter to open it."); pressEnter();
	} else {
		printf("%s decides to stay on the current path and leaves the room.", c->name); pressEnter();
		printf("The next room is charred black; there must have been an intense fire here long ago."); pressEnter();
		item_or_spell_found(c, DEMON_FIRE, "In a tattered robe on the floor is a mysterious vial of liquid.\n");
		printf("The door is charred black; despite this, %s manages to force it open.", c->name); pressEnter();
	}
	printf("In the next room, %s sees a staircase. This must lead to the second floor!", c->name); pressEnter();
	printf("The distant voice of an elderly man echoes mysteriously in the distance: \"Begone from this place, stranger!\""); pressEnter();
	printf("Suddenly, a magical glow fills the air, and a massive plant grows rapidly out of the ground."); pressEnter();
	printf("It resembles a Venus flytrap, only 100 times the size!"); pressEnter();
	Character *m = newCharacter(" appears!", KILLER_PLANT);
	combat_sequence(c, m, 1);
}
/* Floor 2 start */
void lvl2(Character *c) {
	printf("%s droops to the ground, then magically shrinks back down to normal size.", MONSTER_NAMES[KILLER_PLANT]); pressEnter();
	printf("Whoever spoke earlier is gone; there is only silence."); pressEnter();
	printf("It is time to move on. Press enter to proceed up the stairs to the second floor."); pressEnter();
	printf("A faded red carpet lines the stairs and continues into the dark hallway beyond."); pressEnter();
	item_or_spell_found(c, BLUE_POTION, "Right at the top of the stairs is a potion!\n");
	printf("%s is now surrounded by darkness, but notices a faint light coming from the right.", c->name); pressEnter();
	printf(""); pressEnter();
	bool isYes = yes_or_no("Follow the source of light?");
	if(isYes) {
		printf("%s turns right in an attempt to escape the darkness.", c->name); pressEnter();
		printf("The light grows brighter as %s continues down the corridor.", c->name); pressEnter();
		printf("%s enters the next room and is blinded by the light radiating from its center.", c->name); pressEnter();
		item_or_spell_found(c, LIGHT_VIAL, "The light is coming from an item!\n");
		printf("There is a door on the right side of the room. Press enter to exit the room."); pressEnter();
	} else {
		printf("%s opts to continue on the main path.", c->name); pressEnter();
		printf("Walking forward, %s almost falls into a massive hole in the floor. It seems to have no bottom.", c->name); pressEnter();
		isYes = yes_or_no("Jump into the hole?");
		if(isYes) {
			printf("%s jumps into the abyss but immediately lands on something soft... It's a sheep?", c->name); pressEnter();
			printf("The sheep has wings, and majestically flies %s back to safety.", c->name); pressEnter();
			item_or_spell_found(c, SUMMON_SHEEP, "The sheep has a magic scroll in its mouth!");
			printf("The sheep disappears. %s is unsure if it was a hallucination or not.", c->name); pressEnter();
		} else {
			printf("%s makes the sensible decision not to jump into the hole.", c->name); pressEnter();
		}
		printf("Continuing on the path, %s notices a slight glimmer on the floor.", c->name); pressEnter();
		printf("It's a huge claymore, unfortunately shattered and unusable."); pressEnter();
		item_or_spell_found(c, TEARS, "Next to the sword is a vial of some liquid.\n");
		printf("%s reaches a door at the end of the hallway. Press enter to open it.", c->name); pressEnter();
	} //end else
	drink_potion(c);
	printf("Press enter to open the door and continue."); pressEnter();
	printf("Immediately upon entering the room %s feels an unatural presence.", c->name); pressEnter();
	printf("The room is ice cold, and a cold breeze blows the door shut!"); pressEnter();
	printf("A mysterious figure appears floating above the ground. It wears a torn black cloak and wields a large scythe."); pressEnter();
	Character *m = newCharacter(" appears!", WRAITH);
	combat_sequence(c, m, 1);
}
/* Floor 3 start */
void lvl3(Character *c) {
	// printf("%s mutters a name ");
	Character *m = newCharacter(" appears!", MAD_WIZARD);
	combat_sequence(c, m, 2);

}
void lvl4(Character *c) {

	Character *m = newCharacter(" appears!", GOLEM);
	combat_sequence(c, m, 2);
}
/* Floor 4 start */
void lvl5(Character *c) {

	Character *m = newCharacter(" appears!", VAMPIRE_LORD);
	combat_sequence(c, m, 2);
}
void the_end(Character *c) {

}
//@TODO implement a save file in the main function, and perhaps an option to start a new game too
int main() {
	Character *c = newCharacter("", PLAYER); /* Player created in main, monsters in the lvl functions */
	lvl0(c);
	lvl1(c);
	free(c);
	return 0;
}