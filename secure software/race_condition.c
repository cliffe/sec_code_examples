/*
 * 2012 Leeds Met, Z. Cliffe Schreuders
 * Licensed under the terms of the GPLv3
 */
#include<stdio.h>
#include<sys/stat.h>
int main()
{
	char tmpname [20] = "/tmp/not_so_random";
	char command [100];

	struct stat buf;
	int ok = stat(tmpname, &buf);
	sleep(5);
	if(ok == 0) {
		printf("Temporary file already exists, we need a new name...");
	} else {
		printf("File does not exist, writing...");
		sprintf(command, "echo Hello > %s", tmpname);
		system(command);
	}
}
