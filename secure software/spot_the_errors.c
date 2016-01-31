#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define MAXPASS 7
int main()
{
	char correct_pass[7] = "SecreT\0";
	char input_pass[7];
	do{
		scanf("%s", &input_pass);
		// uncomment the next line to see what is happening...
		//printf("You entered %s, pass is %s\n", input_pass, correct_pass);
		if(strcmp(input_pass, correct_pass) == 0) {
			printf("Access granted\n");
		} else {
			printf("Access denied\n");
		}
	} while(strcmp(input_pass, correct_pass) != 0);
}
