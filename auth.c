/*User Authentication and Access Security*/
#include "rental_system.h"

int loginSystem(void) {
    char username[MAX_STR];
    char password[MAX_STR];
    int attempts = 3;

    printf("===================================================\n");
    printf("         ADMINISTRATOR LOGIN SYSTEM                \n");
    printf("===================================================\n");

    while (attempts > 0) {
        getStringInput("Enter Username: ", username, MAX_STR);
        getStringInput("Enter Password: ", password, MAX_STR);

        if (strcmp(username, "admin") == 0 && strcmp(password, "admin123") == 0) {
            printf("\n[SUCCESS] Authentication successful! Welcome, Admin.\n\n");
            return 1;
        } else {
            attempts--;
            printf("\n[ERROR] Invalid credentials! Attempts remaining: %d\n\n", attempts);
        }
    }

    printf("[DENIED] Maximum login attempts reached. Access denied.\n");
    return 0;
}