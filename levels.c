#include <stdio.h>
#include <stdlib.h>

#include "constants.h"
#include "defs.h"

/* Floor 1 start */
void lvl0(Character *c) {
	printf("Press enter to advance through dialogue."); pressEnter();
	printf(C_GREEN "\n---------- Level 0 ----------\n" C_RESET);
	printf("A forest of trees surrounds a clearing; it is here that a massive,"
			" four-floor mansion towers above the forest."); pressEnter();
	printf(C_BLUE "%s " C_RESET "wonders: what might lie on the fourth floor?", c->name); pressEnter();
	printf(C_BLUE "%s " C_RESET "reaches the massive front doors of the mansion.\n", c->name);
	bool isYes = yes_or_no("Enter the mansion, beginning a perilous journey?\n");
	if(!isYes) {
		printf("Intimidated by the mansion," C_BLUE " %s " C_RESET "turns around and heads home. "
				"Maybe it's for the best.\n", c->name);
		free(c); exit(0);
	}
	printf("The doors creak open." C_BLUE " %s " C_RESET "enters the mansion.", c->name); pressEnter();
	printf("Light streams into the mansion, revealing the dust floating in the air."); pressEnter();
	printf("SLAM! The door closes behind %s!", c->name); pressEnter();
	printf("%s turns and hears unnatural growls.", c->name); pressEnter();
	Character *m = newCharacter(" appears!\nType help(h) for how to fight!", BEAST);
	combat_sequence(c, m, 1);
}

void lvl1(Character *c) {
	printf(C_GREEN "\n---------- Level 1 ----------\n" C_RESET);
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
	printf("The distant voice of an elderly man echoes mysteriously in the distance: "
			"\"Begone from this place, stranger!\""); pressEnter();
	printf("Suddenly, a magical glow fills the air, and a massive plant grows rapidly out of the ground."); pressEnter();
	printf("It resembles a Venus flytrap, only 100 times the size!"); pressEnter();
	Character *m = newCharacter(" appears!", KILLER_PLANT);
	combat_sequence(c, m, 1);
}

/* Floor 2 start */
void lvl2(Character *c) {
	printf(C_GREEN "\n---------- Level 2 ----------\n" C_RESET);
	printf("%s droops to the ground, then magically shrinks back down to normal size.", MONSTER_NAMES[KILLER_PLANT]); pressEnter();
	printf("Whoever spoke earlier is gone; there is only silence."); pressEnter();
	printf("It is time to move on. Press enter to proceed up the stairs to the second floor."); pressEnter();
	printf("A faded red carpet lines the stairs and continues into the dark hallway beyond."); pressEnter();
	item_or_spell_found(c, BLUE_POTION, "Right at the top of the stairs is a potion!\n");
	printf("%s is now surrounded by darkness, but notices a faint light coming from the right.", c->name); pressEnter();
	bool isYes = yes_or_no("Follow the source of light?\n");
	if(isYes) {
		printf("%s turns right in an attempt to escape the darkness.", c->name); pressEnter();
		printf("The light grows brighter as %s continues down the corridor.", c->name); pressEnter();
		printf("%s enters the next room and is blinded by the light radiating from its center.", c->name); pressEnter();
		item_or_spell_found(c, LIGHT_VIAL, "The light is coming from an item!\n");
		printf("There is a door on the right side of the room. Press enter to exit the room."); pressEnter();
	} else {
		printf("%s opts to continue on the main path.", c->name); pressEnter();
		printf("Walking forward, %s almost falls into a massive hole in the floor. It seems to have no bottom.", c->name); pressEnter();
		isYes = yes_or_no("Jump into the hole?\n");
		if(isYes) {
			printf("%s jumps into the abyss but immediately lands on something soft... It's a sheep?", c->name); pressEnter();
			printf("The sheep has wings, and majestically flies %s back to safety.", c->name); pressEnter();
			item_or_spell_found(c, SUMMON_SHEEP, "The sheep has a magic scroll in its mouth!\n");
			printf("The sheep disappears. %s is unsure if it was a hallucination or not.", c->name); pressEnter();
		} else {
			printf("%s makes the sensible decision not to jump into the hole.", c->name); pressEnter();
		}
		printf("Continuing on the path, %s notices a glimmer on the floor.", c->name); pressEnter();
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
	combat_sequence(c, m, 2);
}

/* Floor 3 start */
void lvl3(Character *c) {
	printf(C_GREEN "\n---------- Level 3 ----------\n" C_RESET);
	printf("The wraith lets out a high pitched shriek as it bursts into blue flames."); pressEnter();
	printf("The black robe drifts elegantly to the floor, while the weighty scythe slams into the wall behind."); pressEnter();
	bool isYes = yes_or_no("Investigate the broken wall?");
	if(isYes) {
		printf("Behind the wall is a small room; it's dusty, but a lit candle hints at the room's recent occupancy."); pressEnter();
		printf("Endless pieces of paper cover a small desk. Press enter to see what lies beneath."); pressEnter();
		item_or_spell_found(c, GREATER_RED_POTION, "Underneath the mess is a potion, bearing the same rune as the Beast did.\n");
		printf("In the lamplight, %s sees a ladder upwards, to the 3rd floor of the mansion.", c->name); pressEnter();
	} else {
		printf("%s ignores the wall, instead opening a door at the far end of the room.", c->name); pressEnter();
		printf("It leads to a narrow staircase leading upwards, to the 3rd floor of the mansion.");
	}
	printf("%s follows a narrow, poorly lit hallway, reaching a crossroad. The new path left is faintly lighter.", c->name); pressEnter();
	isYes = yes_or_no("Proceed left down this new path?");
	if(isYes) {
		//TODO
		item_or_spell_found(c, DEMON_FIRE, "");
	} else {
		//TODO
		item_or_spell_found(c, HORN_OF_SAUL, "");
	}
	printf("The door opens to a magnificently bright room with massive windows adorned with elegant white curtains."); pressEnter();
	printf("%s can see the tops of trees further below. %s has come a long way on the journey so far, an admirable feat.", c->name, c->name); pressEnter();
	drink_potion(c);
	printf("%s's rest here has come to an end. Press enter to open a harsh, black door at the far end of the room.", c->name); pressEnter();
	printf("The door slams closed behind %s. Torches illuminate a figure sitting on a large chair at the far end of the room.", c->name); pressEnter();
	printf("\"You will pay for what you've done!\" This voice... it's the same voice as on the second floor!"); pressEnter();
	printf("The old man who steps forward wears a wizard's hat. His mad, red eyes gaze violently at %s.", c->name); pressEnter();
	Character *m = newCharacter(" appears!", MAD_WIZARD);
	combat_sequence(c, m, 2);
}

void lvl4(Character *c) {
	printf(C_GREEN "\n---------- Level 4 ----------\n" C_RESET);
	printf("%s mutters a name in his dying breath: \"Elizabeth...\"", MONSTER_NAMES[MAD_WIZARD]); pressEnter();
	printf("Behind him, %s sees a number of books, personal notes written by the deceased man.", c->name); pressEnter();
	bool isYes = yes_or_no("Read the open page?");
	if(isYes) {

	} else {
		printf("There is no time to lose: %s opts to continue down the corridor", c->name); pressEnter();
	}



	isYes = yes_or_no("Offer up all items and potions in inventory as tribute to the statue?");
	if(isYes) {
		item_or_spell_found(c, SACRIFICIAL_BRAND, "\n");
	}
	Character *m = newCharacter(" appears!", GOLEM);
	combat_sequence(c, m, 3);
}

/* Floor 4 start */
void lvl5(Character *c) {
	printf(C_RED "\n---------- Final Level ----------\n" C_RESET);
	// printf("%s crumbles into pieces."); pressEnter();
	item_or_spell_found(c, GREATER_BLUE_POTION, "At the heart of the creature is a potion!\n");
	printf("Suddenly, a bookshelf on the far wall slides aside, revealing stairs to the fourth floor!"); pressEnter();

	printf("Press enter to climb the stairs to the fourth floor the mansion."); pressEnter();
	item_or_spell_found(c, PANACEA, "On a pedastal in the center of the hall is a beautiful, rainbow-colored potion.\n");
	printf("At the end of the hall lies a foreboding pair of black doors. Press enter to open them."); pressEnter();
	printf("The next room is extravagantly decorated; stained glass windows adorn the walls, and a cloaked figure sits upon a magnificent throne."); pressEnter();
	printf("He rises slowly from his throne, his crimson-red eyes striking fear into %s's heart.", c->name); pressEnter();
	printf("His sharp teeth make it clear he is a vampire. %s thinks the red liquid in his cup may not be wine.", c->name); pressEnter();
	printf("The vampire clears his throat and speaks: \"You have done well to come this far, and for that you deserve my respect.\""); pressEnter();
	printf("\"Therefore, I will give you one last chance, %s. Leave this place, or I will kill you. Your death is guaranteed if you stay.\"", c->name); pressEnter();
	bool isYes = yes_or_no("Take the vampire up on his offer and leave the mansion?");
	if(isYes) {
		printf("\"Hahaha, I always expected you would be a coward. Now, leave me be and vanish, before I change my mind\""); pressEnter();
		printf("With that, %s leaves the vampire's throne room in shame, never knowing what the vampire guarded so vigorously. Perhaps it's for the best.\n", c->name); pressEnter();
		printf("Press enter to admit defeat and leave the mansion."); pressEnter();
		free(c); exit(0);
	}
	printf("\"You fool! You dare think you can challenge me, Lord of all Vampires? You boast, %s, and unduely so.\"", c->name); pressEnter();
	Character *m = newCharacter(" tears off his cloak, unsheaths a bejeweled rapier, and growls fiercely.", VAMPIRE_LORD);
	printf("\"Your move, %s.\"", c->name);
	combat_sequence(c, m, 0); // last fight so no level ups needed
}

void the_end(Character *c) {
	printf("Human or vampire, a child left alone will surely die of hunger and solitude.\n"); pressEnter();
	bool isYes = yes_or_no("A decision must be made: kill the child (Yes) or spare him, taking him home with you (No)?\n");
	if(isYes) {

	} else {

	}
}
