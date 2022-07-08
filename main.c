#include <stdlib.h>
#include "defs.h"

int main() {
	/* Player created in main, monsters in the lvl functions */
	Character *c = newCharacter("", PLAYER);
	Character *m = newCharacter(" slams his staff against the ground and stands to fight!", MAD_WIZARD);
	combat_sequence(c, m, 2);
	exit(0);

	lvl0(c);
	lvl1(c);
	lvl2(c);
	lvl3(c);
	lvl4(c);
	//lvl5(c);
	//the_end(c);

	free(c);
	return 0;
}
