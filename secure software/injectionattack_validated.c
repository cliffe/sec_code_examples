/*
 * 2012 Leeds Met, Z. Cliffe Schreuders
 * Licensed under the terms of the GPLv3
 */
#define IS_VALID 1
#define NOT_VALID 0
#include <stdio.h>
#include <string.h>

int main() {
	char name [20];
	char command [100];
	printf("What is your name?\n");
	gets((char*)&name);
	if(validate(name) == IS_VALID) {
		sprintf(command, "echo Hello %s; echo The time is currently:; date", name);
		system(command);
	} else {
		printf("Invalid input!\n");
	}
}

int validate(char* input) {
	int i;
	for(i=0; i < strlen(input); i++) {
		if(!isalpha(input[i])) { 
			return NOT_VALID;
		}
	}
	return IS_VALID;
}
