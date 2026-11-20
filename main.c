/*Application Main Driver Menu and Input Sanitization Helpers*/
#include "rental_system.h"

void clearBuffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void getStringInput(const char *prompt, char *output, int maxLen) {
    printf("%s", prompt);
    if (fgets(output, maxLen, stdin) != NULL) {
        size_t len = strlen(output);
        if (len > 0 && output[len - 1] == '\n') {
            output[len - 1] = '\0';
        } else {
            clearBuffer();
        }
    }
}

int getIntInput(const char *prompt) {
    int val;
    while (1) {
        printf("%s", prompt);
        if (scanf("%d", &val) == 1) {
            clearBuffer();
            return val;
        } else {
            printf("[INVALID] Enter a valid integer number.\n");
            clearBuffer();
        }
    }
}

float getFloatInput(const char *prompt) {
    float val;
    while (1) {
        printf("%s", prompt);
        if (scanf("%f", &val) == 1) {
            clearBuffer();
            return val;
        } else {
            printf("[INVALID] Enter a valid decimal number.\n");
            clearBuffer();
        }
    }
}

int main(void) {
    Property properties[MAX_PROPERTIES];
    Tenant tenants[MAX_TENANTS];
    Payment payments[MAX_PAYMENTS];

    int pCount = 0;
    int tCount = 0;
    int payCount = 0;

    // System Authentication
    if (!loginSystem()) {
        printf("Exiting application.\n");
        return 0;
    }

    // Load Data from File Persistence
    loadPropertiesFromFile(properties, &pCount);
    loadTenantsFromFile(tenants, &tCount);
    loadPaymentsFromFile(payments, &payCount);

    int choice;
    while (1) {
        printf("\n===================================================\n");
        printf("       PROPERTY RENTAL MANAGEMENT SYSTEM           \n");
        printf("===================================================\n");
        printf(" 1. Add New Property\n");
        printf(" 2. Display All Properties\n");
        printf(" 3. Register New Tenant\n");
        printf(" 4. Display All Tenants\n");
        printf(" 5. Search Tenant by Name\n");
        printf(" 6. Search Properties by Budget\n");
        printf(" 7. Sort Properties by Monthly Rent\n");
        printf(" 8. Update Property Details\n");
        printf(" 9. Delete Property Record\n");
        printf(" 10. Record Rent Payment\n");
        printf(" 11. View Payment History by Tenant\n");
        printf(" 12. Generate Analytics & Summary Report\n");
        printf(" 13. Save & Exit System\n");
        printf("===================================================\n");

        choice = getIntInput("Enter Choice (1-13): ");

        switch (choice) {
            case 1:
                addProperty(properties, &pCount);
                break;
            case 2:
                displayAllProperties(properties, pCount);
                break;
            case 3:
                addTenant(tenants, &tCount, properties, pCount);
                break;
            case 4:
                displayAllTenants(tenants, tCount);
                break;
            case 5:
                searchTenantByName(tenants, tCount);
                break;
            case 6:
                searchPropertiesByBudget(properties, pCount);
                break;
            case 7:
                sortPropertiesByRent(properties, pCount);
                break;
            case 8:
                updatePropertyDetails(properties, pCount);
                break;
            case 9:
                deletePropertyRecord(properties, &pCount, tenants, &tCount);
                break;
            case 10:
                processRentPayment(payments, &payCount, tenants, tCount, properties, pCount);
                break;
            case 11:
                viewPaymentHistory(payments, payCount, tenants, tCount);
                break;
            case 12:
                generateSummaryReport(properties, pCount, tenants, tCount, payments, payCount);
                break;
            case 13:
                savePropertiesToFile(properties, pCount);
                saveTenantsToFile(tenants, tCount);
                savePaymentsToFile(payments, payCount);
                printf("\n[SUCCESS] All data saved. Exiting system. Goodbye!\n");
                exit(0);
            default:
                printf("\n[INVALID] Selection out of bounds. Enter 1 to 13.\n");
        }
    }

    return 0;
}