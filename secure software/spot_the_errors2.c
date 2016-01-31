#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define MAXPASS 7
int main()
{
	char *correct_pass, *input_pass;
	correct_pass = malloc(sizeof(char)*MAXPASS);
	input_pass = malloc(sizeof(char)*MAXPASS);
	strcpy(correct_pass, "SecreT\0");

	printf("Please enter the password: ");
	scanf("%7s", input_pass);
	// uncomment to see what is happening...
	printf("You entered %s, pass is %s\n", input_pass, correct_pass);
	if(strcmp(input_pass, correct_pass) == 0) {
		printf("Access granted\n");
	} else {
		printf("Access denied\n");
	}
}
