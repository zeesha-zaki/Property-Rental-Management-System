/**
 * @file payment_mgmt.c
 * @brief Record Updates, Element Deletion Shifting, and Rent Collections
 * Implemented by Student 3
 */

#include "rental_system.h"

void updatePropertyDetails(Property properties[], int pCount) {
    if (pCount == 0) {
        printf("\n[INFO] No property records available to update.\n");
        return;
    }

    int targetID = getIntInput("\nEnter Property ID to update: ");
    int index = -1;

    for (int i = 0; i < pCount; i++) {
        if (properties[i].propertyID == targetID) {
            index = i;
            break;
        }
    }

    if (index == -1) {
        printf("[ERROR] Property with ID %d not found.\n", targetID);
        return;
    }

    printf("\nUpdating Details for Property ID: %d (%s)\n", properties[index].propertyID, properties[index].address);
    
    float newRent = getFloatInput("Enter New Monthly Rent ($): ");
    int newRooms = getIntInput("Enter New Room Count: ");

    if (newRent > 0) properties[index].monthlyRent = newRent;
    if (newRooms > 0) properties[index].rooms = newRooms;

    printf("[SUCCESS] Property details updated successfully!\n");
}

void deletePropertyRecord(Property properties[], int *pCount, Tenant tenants[], int *tCount) {
    if (*pCount == 0) {
        printf("\n[INFO] No properties available to delete.\n");
        return;
    }

    int targetID = getIntInput("\nEnter Property ID to delete: ");
    int index = -1;

    for (int i = 0; i < *pCount; i++) {
        if (properties[i].propertyID == targetID) {
            index = i;
            break;
        }
    }

    if (index == -1) {
        printf("[ERROR] Property ID %d not found.\n", targetID);
        return;
    }

    if (properties[index].isRented) {
        printf("[WARNING] Property is currently occupied! Unassigning tenant...\n");
        for (int i = 0; i < *tCount; i++) {
            if (tenants[i].assignedPropertyID == targetID) {
                tenants[i].assignedPropertyID = 0;
            }
        }
    }

    // Shift elements left to remove entry from array
    for (int i = index; i < *pCount - 1; i++) {
        properties[i] = properties[i + 1];
    }
    (*pCount)--;

    printf("[SUCCESS] Property ID %d deleted successfully.\n", targetID);
}

void processRentPayment(Payment payments[], int *payCount, const Tenant tenants[], int tCount,
                        const Property properties[], int pCount) {
    if (tCount == 0) {
        printf("\n[INFO] No active tenants available to process payments.\n");
        return;
    }

    if (*payCount >= MAX_PAYMENTS) {
        printf("[ERROR] Payment record log is full!\n");
        return;
    }

    Payment newPay;
    newPay.paymentID = 901 + *payCount;

    int targetTenantID = getIntInput("\nEnter Tenant ID for rent payment: ");
    int tIndex = -1;

    for (int i = 0; i < tCount; i++) {
        if (tenants[i].tenantID == targetTenantID) {
            tIndex = i;
            break;
        }
    }

    if (tIndex == -1) {
        printf("[ERROR] Tenant ID %d not found.\n", targetTenantID);
        return;
    }

    int propIndex = -1;
    for (int i = 0; i < pCount; i++) {
        if (properties[i].propertyID == tenants[tIndex].assignedPropertyID) {
            propIndex = i;
            break;
        }
    }

    if (propIndex == -1) {
        printf("[ERROR] Tenant ID %d has no assigned property, so no rent is due.\n", targetTenantID);
        return;
    }

    float rent = properties[propIndex].monthlyRent;
    printf("Monthly rent for Property ID %d: $%.2f\n", properties[propIndex].propertyID, rent);

    newPay.tenantID = targetTenantID;
    do {
        newPay.amountPaid = getFloatInput("Enter Payment Amount ($): ");
        if (newPay.amountPaid <= 0) {
            printf("[INVALID] Payment amount must be greater than zero.\n");
        } else if (newPay.amountPaid > rent) {
            printf("[INVALID] Payment exceeds the monthly rent of $%.2f. Enter an amount up to the rent.\n", rent);
        }
    } while (newPay.amountPaid <= 0 || newPay.amountPaid > rent);
    getStringInput("Enter Payment Date (DD/MM/YYYY): ", newPay.date, 20);
    strcpy(newPay.status, "Paid");

    payments[*payCount] = newPay;
    (*payCount)++;

    printf("[SUCCESS] Payment of $%.2f logged for Tenant ID %d (Receipt #%d).\n",
           newPay.amountPaid, targetTenantID, newPay.paymentID);
}

void savePaymentsToFile(const Payment payments[], int payCount) {
    FILE *file = fopen("payments.dat", "wb");
    if (file == NULL) {
        printf("[ERROR] Unable to open payments.dat for writing!\n");
        return;
    }

    fwrite(&payCount, sizeof(int), 1, file);
    if (payCount > 0) {
        fwrite(payments, sizeof(Payment), payCount, file);
    }

    fclose(file);
    printf("[FILE] Payment log saved to payments.dat (%d records).\n", payCount);
}

void loadPaymentsFromFile(Payment payments[], int *payCount) {
    FILE *file = fopen("payments.dat", "rb");
    if (file == NULL) {
        *payCount = 0;
        return;
    }

    if (fread(payCount, sizeof(int), 1, file) == 1 && *payCount > 0) {
        fread(payments, sizeof(Payment), *payCount, file);
        printf("[FILE] Loaded %d payment records from file.\n", *payCount);
    } else {
        *payCount = 0;
    }

    fclose(file);
}
void viewPaymentHistory(const Payment payments[], int payCount, const Tenant tenants[], int tCount) {
    if (tCount == 0) {
        printf("\n[INFO] No tenants registered.\n");
        return;
    }

    int targetTenantID = getIntInput("\nEnter Tenant ID to view payment history: ");
    int tIndex = -1;

    for (int i = 0; i < tCount; i++) {
        if (tenants[i].tenantID == targetTenantID) {
            tIndex = i;
            break;
        }
    }

    if (tIndex == -1) {
        printf("[ERROR] Tenant ID %d not found.\n", targetTenantID);
        return;
    }

    printf("\n--- Payment History: %s (Tenant ID %d) ---\n", tenants[tIndex].name, targetTenantID);
    printf("Receipt # | Date         | Amount ($)  | Status\n");
    printf("---------------------------------------------------\n");

    int found = 0;
    float total = 0;
    for (int i = 0; i < payCount; i++) {
        if (payments[i].tenantID == targetTenantID) {
            printf("%-9d | %-12s | %-11.2f | %s\n",
                   payments[i].paymentID, payments[i].date, payments[i].amountPaid, payments[i].status);
            total += payments[i].amountPaid;
            found++;
        }
    }

    if (found == 0) {
        printf("[INFO] No payments recorded for this tenant.\n");
    } else {
        printf("---------------------------------------------------\n");
        printf("Payments: %d | Total paid: $%.2f\n", found, total);
    }
}