/*Header file for Property Rental Management System*/
#ifndef RENTAL_SYSTEM_H
#define RENTAL_SYSTEM_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_PROPERTIES 100
#define MAX_TENANTS 100
#define MAX_PAYMENTS 200
#define MAX_STR 100

// Property Structure
typedef struct {
    int propertyID;
    char address[MAX_STR];
    float monthlyRent;
    int rooms;
    int isRented; // 0 = Available, 1 = Rented
} Property;

// Tenant Structure
typedef struct {
    int tenantID;
    char name[MAX_STR];
    char phone[20];
    int assignedPropertyID; // Links to Property.propertyID
} Tenant;

// Payment/Transaction Structure
typedef struct {
    int paymentID;
    int tenantID;
    float amountPaid;
    char date[20];   // Format: DD/MM/YYYY
    char status[20]; // "Paid" or "Pending"
} Payment;

// Utility Input Functions (Student 4)
void clearBuffer(void);
void getStringInput(const char *prompt, char *output, int maxLen);
int getIntInput(const char *prompt);
float getFloatInput(const char *prompt);

// Authentication Module (Student 1)
int loginSystem(void);

// Property Module (Student 1)
void addProperty(Property properties[], int *pCount);
void displayAllProperties(const Property properties[], int pCount);
void savePropertiesToFile(const Property properties[], int pCount);
void loadPropertiesFromFile(Property properties[], int *pCount);

// Tenant Module (Student 2)
void addTenant(Tenant tenants[], int *tCount, Property properties[], int pCount);
void displayAllTenants(const Tenant tenants[], int tCount);
void searchTenantByName(const Tenant tenants[], int tCount);
void saveTenantsToFile(const Tenant tenants[], int tCount);
void loadTenantsFromFile(Tenant tenants[], int *tCount);

// Update, Delete & Payment Module (Student 3)
void updatePropertyDetails(Property properties[], int pCount);
void deletePropertyRecord(Property properties[], int *pCount, Tenant tenants[], int *tCount);
void processRentPayment(Payment payments[], int *payCount, const Tenant tenants[], int tCount,
                        const Property properties[], int pCount);
void savePaymentsToFile(const Payment payments[], int payCount);
void loadPaymentsFromFile(Payment payments[], int *payCount);
void viewPaymentHistory(const Payment payments[], int payCount, const Tenant tenants[], int tCount);

// Reports & Sorting Module (Student 4)
void sortPropertiesByRent(Property properties[], int pCount);
void searchPropertiesByBudget(const Property properties[], int pCount);
void generateSummaryReport(const Property properties[], int pCount, const Tenant tenants[], int tCount, const Payment payments[], int payCount);

#endif // RENTAL_SYSTEM_H