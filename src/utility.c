#define _CRT_SECURE_NO_WARNINGS /* Removes depreciated warnings for strcpy on windows, for linux compatibility */

#include <assert.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
//#include <stdio_ext.h>

#ifdef _WIN32
	#include <windows.h>
#elif __unix__
	#include <time.h>
#else
	printf("An error has occurred, your operating system may not be supported.\n");
	exit(1);
#endif

#include "constants.h"
#include "defs.h"

/*************** Utility Functions  ***************/
/* https://code-examples.net/en/q/11a859 (slightly altered version) cross platform sleep */
void sleep_ms(int milliseconds) {
	#ifdef _WIN32
	    Sleep(milliseconds);
	#elif __unix__
	    struct timespec ts;
	    ts.tv_sec = milliseconds / 1000;
	    ts.tv_nsec = (milliseconds % 1000) * 1000000;
	    nanosleep(&ts, NULL);
	#else
		printf("An error has occurred, your operating system may not be supported.\n");
		exit(1);
	#endif
}

/** Cross platform strcasecmp/_stricmp */
int case_compare(const char *word1, const char *word2) {
	#ifdef _WIN32
		return _stricmp(word1, word2);
	#elif __unix__
		return strcasecmp(word1, word2);
	#else
		printf("An error has occurred, your operating system may not be supported.\n");
		exit(1);
	#endif
}

void getInput(char input[], char message[]) {
	printf("%s", message);
	//__fpurge(stdin);
	if(fgets(input, MAX_INPUT_LENGTH, stdin) == NULL) {
		fprintf(stderr, "An error has occurred, fgets() returned NULL.\n");
	}
	assert(strlen(input) > 0);
	if(input[strlen(input)-1] == '\n') { // For proper line spacing
		input[strlen(input)-1] = '\0';
	}
	//@TODO better handling of input when > MAX_INPUT_LENGTH
}

/** Ask simple yes or no questions to user.
 *  If yes then return true, if no then return false.
 *  When used, returned value is stored in bool isYes (convention).
 */
bool yes_or_no(char message[]) {
	char input[MAX_INPUT_LENGTH];
	printf("%s", message); /* assumes message will end with \n */
	for(;;) {
		getInput(input, "Yes(y) or No(n): ");
		if(case_compare(input, "yes") == 0 || case_compare(input, "y") == 0) {
			return true;
		} else if(case_compare(input, "no") == 0 || case_compare(input, "n") == 0) {
			return false;
		} else {
			printf("Invalid input.\n"); //repeats loop and asks again
		}
	}
}


/** Version of printf that requires user to press enter to continue. 
 *  No \n needed at end of variables.
 */
int eprintf(const char *format, ...) {
	va_list v;
	va_start(v, format);
	int ret = vfprintf(stdout, format, v);
	va_end(v);
	while(getchar() != '\n');
	return ret;
}
