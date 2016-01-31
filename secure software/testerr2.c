/*
 * 2012 Leeds Met, Z. Cliffe Schreuders
 * Licensed under the terms of the GPLv3
 */

#include <stdio.h>
int main() {
	char execute [5] = "ls";
	char name [10];

	printf("What is your name?\n");
	gets((char*)&name);
	printf("hello %s, executing the command %s\nThe files in the dir are:\n", name, execute);
	sleep(2);
	system(execute);
}
