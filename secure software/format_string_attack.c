#include <stdio.h>
int main() {
        char name [100];
        int allowed_access = 0;        
        int secret_number = 42;
        printf("What is your name?\n");
        fgets(name, sizeof(name), stdin);
        printf("You entered:\n");
        printf(name); // oops!
        if(strcmp(name, "secret\n") == 0) {
                allowed_access = 1;
        }
        if(allowed_access == 1) {
                printf("\nThe secret number is %d\n", secret_number);
        } else {
                printf("\nNot telling you the secret\n");
        }
}
