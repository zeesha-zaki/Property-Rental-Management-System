/*Property Creation, Listing, and Binary File Operations*/
#include "rental_system.h"

void addProperty(Property properties[], int *pCount) {
    if (*pCount >= MAX_PROPERTIES) {
        printf("[ERROR] Cannot add property. System capacity reached!\n");
        return;
    }

    // New ID = highest existing ID + 1 (starts at 101). Basing the ID on the
    // record count would repeat an ID after a property in the middle is deleted.
    int maxID = 100;
    for (int i = 0; i < *pCount; i++) {
        if (properties[i].propertyID > maxID) {
            maxID = properties[i].propertyID;
        }
    }

    Property newProp;
    newProp.propertyID = maxID + 1;

    printf("\n--- Add New Property (ID: %d) ---\n", newProp.propertyID);
    
    getStringInput("Enter Address: ", newProp.address, MAX_STR);
    
    do {
        newProp.monthlyRent = getFloatInput("Enter Monthly Rent ($): ");
        if (newProp.monthlyRent <= 0) {
            printf("[INVALID] Rent must be greater than zero.\n");
        }
    } while (newProp.monthlyRent <= 0);

    do {
        newProp.rooms = getIntInput("Enter Number of Rooms: ");
        if (newProp.rooms <= 0) {
            printf("[INVALID] Room count must be at least 1.\n");
        }
    } while (newProp.rooms <= 0);

    newProp.isRented = 0; // Default: Available

    properties[*pCount] = newProp;
    (*pCount)++;

    printf("[SUCCESS] Property ID %d added successfully!\n", newProp.propertyID);
}

void displayAllProperties(const Property properties[], int pCount) {
    if (pCount == 0) {
        printf("\n[INFO] No property records available.\n");
        return;
    }

    printf("\n=================================================================================\n");
    printf("%-8s | %-30s | %-12s | %-8s | %-12s\n", "ID", "Address", "Rent ($)", "Rooms", "Status");
    printf("=================================================================================\n");

    for (int i = 0; i < pCount; i++) {
        printf("%-8d | %-30s | %-12.2f | %-8d | %-12s\n",
               properties[i].propertyID,
               properties[i].address,
               properties[i].monthlyRent,
               properties[i].rooms,
               properties[i].isRented ? "Rented" : "Available");
    }
    printf("=================================================================================\n");
}

void savePropertiesToFile(const Property properties[], int pCount) {
    FILE *file = fopen("properties.dat", "wb");
    if (file == NULL) {
        printf("[ERROR] Unable to open properties.dat for writing!\n");
        return;
    }

    fwrite(&pCount, sizeof(int), 1, file);
    if (pCount > 0) {
        fwrite(properties, sizeof(Property), pCount, file);
    }

    fclose(file);
    printf("[FILE] Property data saved to properties.dat (%d records).\n", pCount);
}

void loadPropertiesFromFile(Property properties[], int *pCount) {
    FILE *file = fopen("properties.dat", "rb");
    if (file == NULL) {
        *pCount = 0;
        printf("[INFO] No properties.dat found. Initializing empty database.\n");
        return;
    }

    if (fread(pCount, sizeof(int), 1, file) == 1 && *pCount > 0) {
        fread(properties, sizeof(Property), *pCount, file);
        printf("[FILE] Loaded %d property records from file.\n", *pCount);
    } else {
        *pCount = 0;
    }

    fclose(file);
}