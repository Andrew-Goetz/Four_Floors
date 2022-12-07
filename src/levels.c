#include <stdio.h>
#include <stdlib.h>

#include "constants.h"
#include "defs.h"

/* Floor 1 start */
void lvl0(Character *c) {
	eprintf("Press enter to advance through dialogue.");
	eprintf("Now %s begins an arduous journey to the fourth floor of the mansion.", c->name);
	eprintf(C_GREEN "\n---------- Level 0 ----------" C_RESET);
	eprintf("%s has come a long way from the village to this ancient forest.", c->name);
	eprintf("The thick treeline gives way to a clearing; it is here that a massive, four-floor mansion towers above the earth.");
	eprintf("%s wonders: what might lie on the fourth floor?", c->name);
	eprintf("%s continues, reaching the massive front doors of the mansion.", c->name);
	bool isYes = yes_or_no("Enter the mansion, beginning a perilous journey?\n");
	if(!isYes) {
		eprintf("Intimidated by the mansion, %s turns around and heads home. Maybe it's for the best.\n", c->name);
		free(c); exit(0);
	}
	eprintf("The doors creak open. %s enters the mansion.", c->name);
	eprintf("Light streams into the mansion, revealing the dust floating in the air.");
	eprintf("SLAM! The door closes behind %s!", c->name);
	eprintf("%s turns and hears unnatural growls.", c->name);
	Character *m = newCharacter(" appears!\nType help(h) for how to fight!", BEAST);
	combat_sequence(c, m, 1);
}

void lvl1(Character *c) {
	eprintf(C_GREEN "\n---------- Level 1 ----------" C_RESET);
	eprintf("Looking at %s on the ground, %s notices a strange brand near the beast's torso. "
		   "Maybe it's important.", MONSTER_NAMES[BEAST], c->name);
	eprintf("With the first foe defeated, it is time to move on! Press enter to continue deeper into the mansion.");
	item_or_spell_found(c, RED_POTION, "After walking down a hallway, a potion can be seen sitting on a bookshelf.\n");
	drink_potion(c);
	eprintf("%s enters a massive room with another hallway branching off to the left.", c->name);
	bool isYes = yes_or_no("Take this other path?\n");
	if(isYes) {
		eprintf("Something about this left hallway seems special. %s heads left and leaves the room.", c->name);
		eprintf("The hallway continues for a time before making an abrupt right turn. Press enter to see what's around the corner.");
		item_or_spell_found(c, FIREBALL, "On a small table on the side of the hall, there is a magical scroll!\n");
		eprintf("At the end of the hallway there is a door. Press enter to open it.");
	} else {
		eprintf("%s decides to stay on the current path and leaves the room.", c->name);
		eprintf("The next room is charred black; there must have been an intense fire here long ago.");
		item_or_spell_found(c, DEMON_FIRE, "In a tattered robe on the floor is a mysterious vial of liquid.\n");
		eprintf("The door is charred black; despite this, %s manages to force it open.", c->name);
	}
	eprintf("In the next room, %s sees a staircase. This must lead to the second floor!", c->name);
	eprintf("The distant voice of an elderly man echoes mysteriously in the distance: "
			C_CYAN "\"Begone from this place, stranger!\"" C_RESET);
	eprintf("Suddenly, a magical glow fills the air, and a massive plant grows rapidly out of the ground.");
	eprintf("It resembles a Venus flytrap, only 100 times the size!");
	Character *m = newCharacter(" appears!", KILLER_PLANT);
	combat_sequence(c, m, 1);
}

/* Floor 2 start */
void lvl2(Character *c) {
	eprintf(C_GREEN "\n---------- Level 2 ----------" C_RESET);
	eprintf("%s droops to the ground, then magically shrinks back down to normal size.", MONSTER_NAMES[KILLER_PLANT]);
	eprintf("Whoever spoke earlier is gone; there is only silence.");
	eprintf("It is time to move on. Press enter to proceed up the stairs to the second floor.");
	eprintf("A faded red carpet lines the stairs and continues into the dark hallway beyond.");
	item_or_spell_found(c, BLUE_POTION, "Right at the top of the stairs is a potion!\n");
	eprintf("%s is now surrounded by darkness, but notices a faint light coming from the right.", c->name);
	bool isYes = yes_or_no("Follow the source of light?\n");
	if(isYes) {
		eprintf("%s turns right in an attempt to escape the darkness.", c->name);
		eprintf("The light grows brighter as %s continues down the corridor.", c->name);
		eprintf("%s enters the next room and is blinded by the light radiating from its center.", c->name);
		item_or_spell_found(c, LIGHT_VIAL, "The light is coming from an item!\n");
		eprintf("There is a door on the right side of the room. Press enter to exit the room.");
	} else {
		eprintf("%s opts to continue on the main path.", c->name);
		eprintf("Walking forward, %s almost falls into a massive hole in the floor. It seems to have no bottom.", c->name);
		isYes = yes_or_no("Jump into the hole?\n");
		if(isYes) {
			eprintf("%s jumps into the abyss but immediately lands on something soft... It's a sheep?", c->name);
			eprintf("The sheep has wings, and majestically flies %s back to safety.", c->name);
			item_or_spell_found(c, SUMMON_SHEEP, "The sheep has a magic scroll in its mouth!\n");
			eprintf("The sheep disappears. %s is unsure if it was a hallucination or not.", c->name);
		} else {
			eprintf("%s makes the sensible decision not to jump into the hole.", c->name);
		}
		eprintf("Continuing on the path, %s notices a glimmer on the floor.", c->name);
		eprintf("It's a huge claymore, unfortunately shattered and unusable.");
		item_or_spell_found(c, TEARS, "Next to the sword is a vial of some liquid.\n");
		eprintf("%s reaches a door at the end of the hallway. Press enter to open it.", c->name);
	} //end else
	drink_potion(c);
	eprintf("Press enter to open the door and continue.");
	eprintf("Immediately upon entering the room %s feels an unatural presence.", c->name);
	eprintf("The room is ice cold, and a cold breeze blows the door shut!");
	eprintf("A mysterious figure appears floating above the ground. It wears a torn black cloak and wields a large scythe.");
	Character *m = newCharacter(" appears!", WRAITH);
	combat_sequence(c, m, 2);
}

/* Floor 3 start */
void lvl3(Character *c) {
	eprintf(C_CYAN "\n---------- Level 3 ----------" C_RESET);
	eprintf("The wraith lets out a high pitched shriek as it bursts into blue flames.");
	eprintf("The black robe drifts elegantly to the floor, while the weighty scythe slams into the wall behind.");
	bool isYes = yes_or_no("Investigate the broken wall?\n");
	if(isYes) {
		eprintf("Behind the wall is a small room; it's dusty, but a lit candle hints at the room's recent occupancy.");
		eprintf("Endless pieces of paper cover a small desk. Press enter to see what lies beneath.");
		item_or_spell_found(c, GREATER_RED_POTION, "Underneath the mess is a potion, bearing the same rune as the Beast did.\n");
		eprintf("In the lamplight, %s sees a ladder upwards, to the 3rd floor of the mansion.", c->name);
	} else {
		eprintf("%s ignores the wall, instead opening a door at the far end of the room.", c->name);
		eprintf("It leads to a narrow staircase leading upwards, to the 3rd floor of the mansion.");
	}
	eprintf("%s follows a narrow, poorly lit hallway, reaching a crossroad. The new path to the left is faintly lighter.", c->name);
	isYes = yes_or_no("Proceed left down this new path?\n");
	if(isYes) {
		eprintf("%s arrives at a door. Its doorknob won't turn. A decrepit sign reads \"The portal room.\"", c->name);
		eprintf("Press enter to force the door open.");
		eprintf("The door leads into a black chared room, with a frightening contraption in the middle.");
		eprintf("The only word %s can make out from notes sitting on a burnt desk is \"Demon\"", c->name);
		item_or_spell_found(c, DEMON_FIRE, "On the desk with the notes is a vial!\n");
		eprintf("A beautiful door at the far end of the room seems uneffected by the darkness of the room. Press enter to open it.");
	} else {
		eprintf("%s continues down the current path.", c->name);
		eprintf("The fading carpet floor gives way to unblemished marble.");
		eprintf("A set of magnificent doors carved from white wood are too heavy for %s to open.", c->name);
		eprintf("At the end of the hall, lying in a chair attached to some wires, is a skeleton double %s's size.", c->name);
		eprintf("He must have been a formidable man in life. %s wonders what had been done to him.", c->name);
		item_or_spell_found(c, HORN_OF_SAUL, "Hung around the skeleton's torso is a magnificent, yet fragile, horn.\n");
		eprintf("A door as fair and white as the horn lies lies ahead. Press enter to open it.");
	}
	eprintf("The door opens to a magnificently bright room, fitted with massive windows adorned with elegant white curtains.");
	eprintf("%s can see the tops of the trees of the forest below. %s has come a long way on the journey so far, an admirable feat.", c->name, c->name);
	drink_potion(c);
	eprintf("%s's rest here has come to an end. Press enter to open a harsh, black door at the far end of the room.", c->name);
	eprintf("The door slams closed behind %s. Torches illuminate a figure sitting on a large chair at the far end of the room.", c->name);
	eprintf("\"You! You will pay for what you've done!\" This voice... it's the same voice as on the second floor!");
	eprintf("The old man who steps forward wears a wizard's hat. His mad, red eyes gaze violently at %s.", c->name);
	Character *m = newCharacter(" slams his staff against the ground and stands to fight!", MAD_WIZARD);
	combat_sequence(c, m, 2);
}

void lvl4(Character *c) {
	eprintf(C_GREEN "\n---------- Level 4 ----------" C_RESET);
	eprintf("%s mutters a name in his dying breath: \"Elizabeth...\"", MONSTER_NAMES[MAD_WIZARD]);
	eprintf("A desk filled with crumbled scrolls lies behind the deceased man.");
	item_or_spell_found(c, FROST_RESONANCE, "The first scroll is cold to the touch.\n");
	item_or_spell_found(c, FIREBALL, "The next scroll is scorching hot.\n");
	item_or_spell_found(c, LIGHTNING_STAKE, "The final scroll makes one's hair stand on end.\n");
	eprintf("Underneath the scrolls, %s sees an open book, personal notes from %s.", c->name, MONSTER_NAMES[MAD_WIZARD]);
	bool isYes = yes_or_no("Skim through the pages?\n");
	if(isYes) {
		//TODO
		eprintf("It's illegible for now. %s continues down the corridor.", c->name);
	} else {
		eprintf("There is no time to lose: %s opts to continue down the corridor", c->name);
	}
	eprintf("The corridor continues straight down a metallic, featureless hallway.");
	item_or_spell_found(c, IRON_PELLET, "Sitting on a glass pedestal is a gooey, metalic, and fragrent substance.\n");
	eprintf("%s continues down the suprisingly inornate hall.", c->name);
	eprintf("Suddenly the hallway ends. A massive statue of pure metal stands many meters tall.");
	eprintf("A pool of water sits below. A sign reads:\n\t\"Offer up all belongings to tread upon the %spath of righteousness%s.\"", C_PURPLE, C_RESET);
	isYes = yes_or_no("Offer up all items and potions in inventory as tribute to the statue? Don't make this choice lightly.\n");
	if(isYes) {
		c->potionSlot = NOTHING;
		c->itemSlot = NOTHING;
		item_or_spell_found(c, SACRIFICIAL_BRAND, "The potions and items are drained away. The statue breaks in two, revealing a scroll!");
		eprintf("Unfortunately, it looks like the statue breaking shattered a healing potion sitting behind the statue.\n");
	} else {
		eprintf("A prudent decision: who would give up such valuable resources at a time like this?");
		item_or_spell_found(c, RED_POTION, "Behind the statue is a red potion!\n");
	}
	eprintf("Proceeding past the statue, %s finds a heavy door shut closed by chains.", c->name);
	eprintf("Press enter to break the chains and open the door.");
	eprintf("Suddenly, a towering pile of stone stands up of its own free will!");
	Character *m = newCharacter(" appears!", GOLEM);
	combat_sequence(c, m, 3);
}

/* Floor 4 start */
void lvl5(Character *c) {
	eprintf(C_RED "\n---------- Final Level ----------" C_RESET);
	// eprintf("%s crumbles into pieces.");
	item_or_spell_found(c, GREATER_BLUE_POTION, "At the heart of the creature is a potion!\n");
	eprintf("Suddenly, a bookshelf on the far wall slides aside, revealing stairs to the fourth floor!");
	eprintf("Press enter to climb the stairs to the fourth floor of the mansion.");
	item_or_spell_found(c, PANACEA, "On a pedestal in the center of the hall is a beautiful, rainbow-colored potion.\n");
	eprintf("At the end of the hall lies a foreboding pair of black doors. Press enter to open them.");
	eprintf("The next room is extravagantly decorated; stained glass windows adorn the walls, and a cloaked figure sits upon a magnificent throne.");
	eprintf("He rises slowly from his throne, his crimson-red eyes striking fear into %s's heart.", c->name);
	eprintf("His sharp teeth and pointed ears make it clear he is a vampire. %s thinks the red liquid in his cup may not be wine.", c->name);
	eprintf("The vampire clears his throat and speaks: \"You have done well to come this far, and for that you deserve my respect.\"");
	eprintf("\"Therefore, I will give you one last chance, %s. Leave this place, or I will kill you. Your death is guaranteed if you stay.\"", c->name);
	bool isYes = yes_or_no("Take the vampire up on his offer and leave the mansion?\n");
	if(isYes) {
		eprintf("\"Hahaha, I always expected you would be a coward. Now, leave me be and vanish, before I change my mind.\"");
		eprintf("With that, %s leaves the vampire's throne room in shame, never knowing what the vampire guarded so vigorously. Perhaps it's for the best.\n", c->name);
		eprintf("Press enter to admit defeat and leave the mansion.");
		free(c); exit(0);
	}
	eprintf("\"You fool! You dare think you can challenge me, Lord of all Vampires? You jest, %s.\"", c->name);
	Character *m = newCharacter(" tears off his cloak, unsheaths a bejeweled rapier, and growls fiercely.", VAMPIRE_LORD);
	printf("\"Your move, %s.\"\n", c->name);
	combat_sequence(c, m, 0); /* last fight so no level ups needed */
}

void the_end(Character *c) {
	eprintf(C_GREEN "\n---------- Conclusion ----------" C_RESET);
	eprintf("The vampire burns into dust, his cloak and sword droping to the ground.");
	eprintf("Beyond the Vampire Lord's lavish throne lies a simple wooden door. Press enter to open it.");
	eprintf("Expecting the riches beyond imagination, %s is shocked: a sleeping child awakes from bed and walks towards %s.", c->name, c->name);
	eprintf("The little boy is adorable, but crooked eyes and pointed ears make obvious the child's lineage.");
	eprintf("The Vampire Lord killed thousands of men in his long lifetime, and his son might very well do the same.");
	eprintf("Human or vampire, a child left alone will surely die of hunger and solitude.");
	bool isYes = yes_or_no("A decision must be made: " C_RED "kill the child" C_RESET " (Yes) or " C_GREEN "raise the child as your own" C_RESET " (No)?\n");
	if(isYes) {
		eprintf("The child's death is swift. The child's limp body droops to the ground.");
		eprintf("%s places the child's detached head on his bed and leaves the mansion, sword stained with fresh blood.", c->name);
		eprintf("%s returns to the village victorious to a life of fame and comfort.", c->name);
		eprintf("All would come to know %s as the slayer of the Great Vampire Lord and his kin, a true hero.", c->name);
	} else {
		eprintf("%s puts down any weapons and reaches a hand out to the child.", c->name);
		eprintf("The child grabs %s's hand. His fangs are visible as he smiles gently at %s.", c->name, c->name);
		eprintf("%s will not be able to return to the village with a demon child in tow.", c->name);
		eprintf("A rough life awaits the vampire child and %s, the old adventurer he now considers family.", c->name);
	}
	eprintf("Press enter to exit the game and accept your decision.");
}
