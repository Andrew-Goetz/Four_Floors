#define _CRT_SECURE_NO_WARNINGS /* Removes depreciated warnings for strcpy on windows, for linux compatibility */
#include <assert.h>
#include <stdbool.h> 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef _WIN32
	#include <windows.h>
#elif __unix__
	#include <time.h>
#else
	printf("Your operating system is not supported. The program will now exit.\n");
	exit(1);
#endif

#include "constants.h"
#include "defs.h"


int main() {
	Character *c = newCharacter("", PLAYER); /* Player created in main, monsters in the lvl functions */
	lvl0(c);
	lvl1(c);
	lvl2(c);
	//lvl3(c);
	//lvl4(c);
	//lvl5(c);
	the_end(c);
	free(c);
	return 0;
}
