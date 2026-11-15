/**
 * @file tenant_mgmt.c
 * @brief Tenant Registration, Property Assignment, and Substring Search
 * Implemented by Student 2
 */

#include "rental_system.h"

void addTenant(Tenant tenants[], int *tCount, Property properties[], int pCount) {
    if (*tCount >= MAX_TENANTS) {
        printf("[ERROR] Tenant database is full!\n");
        return;
    }

    if (pCount == 0) {
        printf("[ERROR] No properties exist in the system. Add properties first!\n");
        return;
    }

    Tenant newTenant;
    newTenant.tenantID = 501 + *tCount;

    printf("\n--- Register New Tenant (ID: %d) ---\n", newTenant.tenantID);
    getStringInput("Enter Tenant Full Name: ", newTenant.name, MAX_STR);
    getStringInput("Enter Phone Number: ", newTenant.phone, 20);

    printf("\nAvailable Properties for Rental:\n");
    int availableCount = 0;
    for (int i = 0; i < pCount; i++) {
        if (properties[i].isRented == 0) {
            printf(" -> Property ID: %d | Address: %s | Rent: $%.2f\n",
            properties[i].propertyID, properties[i].address, properties[i].monthlyRent);
            availableCount++;
        }
    }

    if (availableCount == 0) {
        printf("[ERROR] No vacant properties available for lease at this time.\n");
        return;
    }

    int targetPropID;
    int foundIndex = -1;

    do {
        targetPropID = getIntInput("Enter Property ID to Assign: ");
        for (int i = 0; i < pCount; i++) {
            if (properties[i].propertyID == targetPropID && properties[i].isRented == 0) {
                foundIndex = i;
                break;
            }
        }
        if (foundIndex == -1) {
            printf("[INVALID] ID invalid or property already rented. Enter a vacant Property ID.\n");
        }
    } while (foundIndex == -1);

    do {
        newTenant.leaseMonths = getIntInput("Enter Lease Duration (Months): ");
        if (newTenant.leaseMonths <= 0) {
            printf("[INVALID] Lease duration must be at least 1 month.\n");
        }
    } while (newTenant.leaseMonths <= 0);

    newTenant.assignedPropertyID = targetPropID;
    properties[foundIndex].isRented = 1;

    tenants[*tCount] = newTenant;
    (*tCount)++;

    printf("[SUCCESS] Tenant '%s' successfully assigned to Property ID %d!\n", newTenant.name, targetPropID);
}

void displayAllTenants(const Tenant tenants[], int tCount) {
    if (tCount == 0) {
        printf("\n[INFO] No tenant records found.\n");
        return;
    }

    printf("\n=================================================================================\n");
    printf("%-8s | %-25s | %-15s | %-18s | %-12s\n", "ID", "Tenant Name", "Phone", "Assigned Prop ID", "Lease (Months)");
    printf("=================================================================================\n");

    for (int i = 0; i < tCount; i++) {
        printf("%-8d | %-25s | %-15s | %-18d | %-12d\n",
               tenants[i].tenantID,
               tenants[i].name,
               tenants[i].phone,
               tenants[i].assignedPropertyID,
               tenants[i].leaseMonths);
    }
    printf("=================================================================================\n");
}

void searchTenantByName(const Tenant tenants[], int tCount) {
    if (tCount == 0) {
        printf("\n[INFO] No tenant records available to search.\n");
        return;
    }

    char searchName[MAX_STR];
    getStringInput("\nEnter Tenant Name (or substring) to search: ", searchName, MAX_STR);

    int found = 0;
    printf("\n--- Search Results ---\n");
    for (int i = 0; i < tCount; i++) {
        char lowerTenant[MAX_STR], lowerSearch[MAX_STR];
        strcpy(lowerTenant, tenants[i].name);
        strcpy(lowerSearch, searchName);

        for (int k = 0; lowerTenant[k]; k++) lowerTenant[k] = (char)tolower((unsigned char)lowerTenant[k]);
        for (int k = 0; lowerSearch[k]; k++) lowerSearch[k] = (char)tolower((unsigned char)lowerSearch[k]);

        if (strstr(lowerTenant, lowerSearch) != NULL) {
            printf("ID: %d | Name: %s | Phone: %s | Property ID: %d | Lease: %d months\n",
                   tenants[i].tenantID, tenants[i].name, tenants[i].phone,
                   tenants[i].assignedPropertyID, tenants[i].leaseMonths);
            found++;
        }
    }

    if (found == 0) {
        printf("[INFO] No tenants matching query '%s' found.\n", searchName);
    } else {
        printf("[INFO] Total records found: %d\n", found);
    }
}

void saveTenantsToFile(const Tenant tenants[], int tCount) {
    FILE *file = fopen("tenants.dat", "wb");
    if (file == NULL) {
        printf("[ERROR] Unable to open tenants.dat for writing!\n");
        return;
    }

    fwrite(&tCount, sizeof(int), 1, file);
    if (tCount > 0) {
        fwrite(tenants, sizeof(Tenant), tCount, file);
    }

    fclose(file);
    printf("[FILE] Tenant data saved to tenants.dat (%d records).\n", tCount);
}

void loadTenantsFromFile(Tenant tenants[], int *tCount) {
    FILE *file = fopen("tenants.dat", "rb");
    if (file == NULL) {
        *tCount = 0;
        return;
    }

    if (fread(tCount, sizeof(int), 1, file) == 1 && *tCount > 0) {
        fread(tenants, sizeof(Tenant), *tCount, file);
        printf("[FILE] Loaded %d tenant records from file.\n", *tCount);
    } else {
        *tCount = 0;
    }

    fclose(file);
}