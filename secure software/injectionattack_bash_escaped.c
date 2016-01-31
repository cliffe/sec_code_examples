/*
 * 2012 Leeds Met, Z. Cliffe Schreuders
 * Licensed under the terms of the GPLv3
 */
#include <stdio.h>
int main() {
	char name [20];
	char command [100];
	printf("What is your name?\n");
	gets((char*)&name);
	sprintf(command, "echo 'Hello %s'; echo The time is currently:; date", name);
	system(command);
}

