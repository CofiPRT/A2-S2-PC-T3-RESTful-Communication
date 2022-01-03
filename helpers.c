#include "helpers.h"

int sockfd;

char *trim(char *string) {
	// trim whitespaces

	if (string == NULL) {
		// nothing to do
		return NULL;
	}

	int start = 0;
	int end = strlen(string);
	
	// place start
	while (strchr(WHITESPACES, string[start])) {
		// this char is a whitespace, skip
		start++;
	}

	// place end
	while (end >= 0 && strchr(WHITESPACES, string[end])) {
		// this char is a whitespace, set as end
		string[end--] = '\0';
	}

	return string + start;
}