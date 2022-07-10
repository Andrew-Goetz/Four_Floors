#include <stdlib.h>
#include "defs.h"

int main() {
	/* Player created in main, monsters in the lvl functions */
	Character *c = newCharacter("", PLAYER);
	//lvl0(c);
	//lvl1(c);
	//lvl2(c);
	//lvl3(c);
	//lvl4(c);
	//lvl5(c);
	Character *m = newCharacter(" tears off his cloak, unsheaths a bejeweled rapier, and growls fiercely.", VAMPIRE_LORD);
	c->defense = 50;
	combat_sequence(c, m, 0); /* last fight so no level ups needed */

	the_end(c);

	free(c);
	return 0;
}
