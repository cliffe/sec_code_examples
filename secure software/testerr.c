/*
 * 2012-14 Leeds Met, Z. Cliffe Schreuders
 * Licensed under the terms of the GPLv3
 */

#include <stdio.h>
int main() {
	char execute [15] = "ls";
	char name [10];

	printf("What is your name?\n");
	scanf("%[^\n]s", &name);
	printf("Hello %s, executing the command %s\nThe files in the dir are:\n", name, execute);
	sleep(2);
	system(execute);
}
