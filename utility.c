#ifdef _WIN32
	#include <windows.h>
#elif __unix__
	#include <time.h>
#else
	printf("Your operating system is not supported. The program will now exit.\n");
	exit(1);
#endif

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


/** https://stackoverflow.com/questions/1406421/press-enter-to-continue-in-c 
 *  Also important: don't use \n at end of printf when pressEnter() is called immediately after,
 *  because the user pressing enter already goes to the next line.
 */
void pressEnter(void) {
	//printf("%s", message);
	while(getchar() != '\n');
}

/** Ask simple yes or no questions to user.
 *  If yes then return true, if no then return false.
 *  When used, returned value is stored in bool isYes.
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
