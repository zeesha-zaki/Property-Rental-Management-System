/*Rent Sorting, Budget Filtering, and Summary Reports */

#include "rental_system.h"

void sortPropertiesByRent(Property properties[], int pCount) {
    if (pCount < 2) {
        printf("\n[INFO] Need at least 2 properties to perform sorting.\n");
        return;
    }

    // Bubble Sort Algorithm (Descending order by monthly rent)
    for (int i = 0; i < pCount - 1; i++) {
        for (int j = 0; j < pCount - i - 1; j++) {
            if (properties[j].monthlyRent < properties[j + 1].monthlyRent) {
                Property temp = properties[j];
                properties[j] = properties[j + 1];
                properties[j + 1] = temp;
            }
        }
    }

    printf("\n[SUCCESS] Properties sorted by monthly rent (Highest -> Lowest).\n");
    displayAllProperties(properties, pCount);
}

void searchPropertiesByBudget(const Property properties[], int pCount) {
    if (pCount == 0) {
        printf("\n[INFO] No properties available to filter.\n");
        return;
    }

    float maxBudget = getFloatInput("\nEnter Maximum Monthly Budget ($): ");
    int matches = 0;

    printf("\n--- Properties Within Budget ($%.2f) ---\n", maxBudget);
    for (int i = 0; i < pCount; i++) {
        if (properties[i].monthlyRent <= maxBudget) {
            printf("ID: %d | Address: %s | Rent: $%.2f | Rooms: %d | Status: %s\n",
                   properties[i].propertyID, properties[i].address,
                   properties[i].monthlyRent, properties[i].rooms,
                   properties[i].isRented ? "Rented" : "Available");
            matches++;
        }
    }

    if (matches == 0) {
        printf("[INFO] No properties found within budget threshold $%.2f\n", maxBudget);
    }
}

void generateSummaryReport(const Property properties[], int pCount, const Tenant tenants[], int tCount, const Payment payments[], int payCount) {
    float totalExpectedRent = 0;
    int rentedCount = 0;
    float totalCollectedRent = 0;

    for (int i = 0; i < pCount; i++) {
        if (properties[i].isRented) {
            rentedCount++;
            totalExpectedRent += properties[i].monthlyRent;
        }
    }

    for (int i = 0; i < payCount; i++) {
        totalCollectedRent += payments[i].amountPaid;
    }

    float occupancyRate = (pCount > 0) ? ((float)rentedCount / pCount) * 100.0f : 0.0f;

    // Output to Console
    printf("\n===================================================\n");
    printf("     PROPERTY MANAGEMENT SYSTEM SUMMARY REPORT    \n");
    printf("===================================================\n");
    printf(" Total Properties Registered : %d\n", pCount);
    printf(" Occupied / Rented Units     : %d\n", rentedCount);
    printf(" Vacant Units                : %d\n", pCount - rentedCount);
    printf(" Occupancy Rate              : %.2f%%\n", occupancyRate);
    printf(" Total Active Tenants        : %d\n", tCount);
    printf(" Monthly Expected Revenue    : $%.2f\n", totalExpectedRent);
    printf(" Total Collections Logged    : $%.2f\n", totalCollectedRent);
    printf("===================================================\n");

    // Output to Text File summary_report.txt
    FILE *file = fopen("summary_report.txt", "w");
    if (file != NULL) {
        fprintf(file, "===================================================\n");
        fprintf(file, "     PROPERTY MANAGEMENT SYSTEM SUMMARY REPORT    \n");
        fprintf(file, "===================================================\n");
        fprintf(file, " Total Properties Registered : %d\n", pCount);
        fprintf(file, " Occupied / Rented Units     : %d\n", rentedCount);
        fprintf(file, " Vacant Units                : %d\n", pCount - rentedCount);
        fprintf(file, " Occupancy Rate              : %.2f%%\n", occupancyRate);
        fprintf(file, " Total Active Tenants        : %d\n", tCount);
        fprintf(file, " Monthly Expected Revenue    : $%.2f\n", totalExpectedRent);
        fprintf(file, " Total Collections Logged    : $%.2f\n", totalCollectedRent);
        fprintf(file, "===================================================\n");
        fclose(file);
        printf("[FILE] Executive report saved to summary_report.txt successfully!\n");
    }
}
void searchPropertiesByRooms(const Property properties[], int pCount) {
    if (pCount == 0) {
        printf("\n[INFO] No properties available to filter.\n");
        return;
    }

    int minRooms;
    do {
        minRooms = getIntInput("\nEnter Minimum Number of Rooms: ");
        if (minRooms <= 0) {
            printf("[INVALID] Number of rooms must be greater than zero.\n");
        }
    } while (minRooms <= 0);

    int matches = 0;
    printf("\n--- Properties With At Least %d Room(s) ---\n", minRooms);
    for (int i = 0; i < pCount; i++) {
        if (properties[i].rooms >= minRooms) {
            printf("ID: %d | Address: %s | Rent: $%.2f | Rooms: %d | Status: %s\n",
                   properties[i].propertyID, properties[i].address,
                   properties[i].monthlyRent, properties[i].rooms,
                   properties[i].isRented ? "Rented" : "Available");
            matches++;
        }
    }

    if (matches == 0) {
        printf("[INFO] No properties found with at least %d room(s).\n", minRooms);
    }
}