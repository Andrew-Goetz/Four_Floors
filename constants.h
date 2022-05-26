#ifndef CONSTANTS_H
#define CONSTANTS_H

/*********** Constants ************/
#define MAX_INPUT_LENGTH 1000/* No input greater than MAX_INPUT_LENGTH characters allowed */
#define SPELLS_IN_GAME 5+1 /* Number of spells in the game, first element left blank (hence the +1) */
#define STATUS_EFFECT_NUM 8 /* Number of status effects in the game, including NONE status effect */
#define MONSTERS_IN_GAME 7 /* Number of monsters in the game, including the player character */
#define SLEEP_DURATION 750 /* Amount of time that passes, in ms, whenever sleep_ms is called */

/* Some color stuff from https://stackoverflow.com/a/3219471 */
/* Use like: printf(C_RED "This is red text.\n" C_RESET); */
#define C_RED     "\x1b[31m"
#define C_GREEN   "\x1b[32m"
#define C_YELLOW  "\x1b[33m"
#define C_BLUE    "\x1b[34m"
#define C_PURPLE  "\x1b[35m"
#define C_CYAN    "\x1b[36m"
#define C_RESET   "\x1b[0m"

static const char *MONSTER_NAMES[MONSTERS_IN_GAME] = {
	"ERROR", "The Beast", "The Killer Plant", "The Wraith", "The Mad Wizard", "The Wizard's Golem", "The Vampire Lord"
};

/* Below goes in order: health, mana, attack, defense */
static const int MONSTER_STATS[MONSTERS_IN_GAME][4] = {
	/* Player Character */	{5, 3, 2, 1},
	/* Beast */ 			{5, 0, 2, 0},
	/* Killer Plant */ 		{9, 0, 2, 1}, /* Almost 1 shot by fireball */
	/* Wraith */ 			{8, 2, 3, 1}, /* Increased damage from light vial */
	/* Mad Wizard */ 		{10, 5, 1, 0}, /* Immune to magic, casts a lot of spells */
	/* Wizard's Golem */ 	{12, 0, 4, 3}, /* All physical damage so iron pellet good against him */
	/* Vampire Lord */		{11, 3, 4, 2}
};

/* To make accessing above array less annoying */
/* MONSTER_STATS[BEAST][HEALTH] gets health of the Beast */
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

typedef enum STATUS_EFFECTS {
	/* None(0) is the default status effect */
	NONE,
	/* Harmful Effects */
	STUN,
	POISON,
	DRAIN,
	/* Positive Effects */
	DEFENSE_UP,
	ATTACK_AND_HEALTH_UP,
	TEARS_ACTIVE,
	BRAND_ACTIVE
} Effect;

/* How long each status effect is active, calculated in combat_sequence().
 * Position in array corresponds to STATUS_EFFECTS enum.
 * Some effects (STUN, for instance) will be changed to a different duration on occasion, these are just defaults.
 * TODO should DEFENSE_UP and ATTACK_AND_HEALTH_UP just last whole fight?
 */ 
static const int EFFECT_DURATIONS[STATUS_EFFECT_NUM] = {
	0, 2, 2, 2, 3, 3, 1, 1
};

typedef enum ITEMS_AND_SPELLS {
	/* Nothing(0) is the default inventory and spell slot value */
	NOTHING,
	/* Spells(1-5) */
	FIREBALL,
	LIGHTNING_STAKE,
	SUMMON_SHEEP,
	SACRIFICIAL_BRAND,
	FROST_RESONANCE,
	/* Potions(6-10) */
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
	"Find powerful items and potions in the mansion.",

	"The magic of the southern deserts. The fire radiates with a blue, magical energy.\nLaunches a fireball at enemies. Consumes 1 mana.",
	"The magic of the dragon slayers of old.\nSmash lightning into the earth, shocking surroundings.",
	"The magic of the mad wizard Pizel.\nSummons a sheep. The magic is unstable, so the sheep may explode.",
	"The magic of the martyrs and their noble sacrifices.\n Reduces caster health to 1. If caster is attacked the following turn, caster takes no damage and the enemy's health is reduced to 1.",
	"The magic of the northern icemen.\nEnvelops enemy in frost, dealing damage overtime.",

	"A red, slimy liquid. Doesn't taste as good as it looks.\nRestores 3 health when used.",
	"A red, slimy liquid. Tastes stronger than the regular potion.\nRestores 5 health when used.",
	"A blue, cold liquid. Is viscous, like syrup.\nRestores 3 mana when used.",
	"A blue, cold liquid. Even thicker than the regular potion.\nRestores 5 mana when used.",
	"The cure-all spoken of in ancient legends. The potion glimmers a brilliant gold.\nRemoves any negative status effects, such as poison.",

	"The tears of a fallen hero and his broken promise.\nUsing immediately before a monster deals fatal damage restores health to full.",
	"A foul tasting, chewy medicine. Used by warriors from the east.\nUsing makes one's skin solid as iron for a short time, increasing defense.",
	"The flames of ancient demons, captured in a vial by coastal wizards.\nSmashing this vial will make enemies erupt into flames.",
	"A drop of pure sunlight, captured in a vial by coastal wizards.\nSmashing this vial will drown a room in the light of day, blinding enemies.",
	"A horn once sounded by Saul, a servant of the gods.\nBlowing this horn will shatter it, temporarily granting its user the strength of the gods."
};

#endif
