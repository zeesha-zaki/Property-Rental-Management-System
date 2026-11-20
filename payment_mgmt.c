/*Record Updates, Element Deletion Shifting, and Rent Collections*/

#include "rental_system.h"

// Reads a DD/MM/YYYY date; returns 1 when the date is a valid calendar date.
static int parseDate(const char *text, int *day, int *month, int *year) {
    char extra;
    if (sscanf(text, "%d/%d/%d%c", day, month, year, &extra) != 3) return 0;
    if (*year < 2000 || *year > 2100 || *month < 1 || *month > 12 || *day < 1) return 0;
    int daysInMonth[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    int leap = (*year % 4 == 0 && *year % 100 != 0) || (*year % 400 == 0);
    int limit = daysInMonth[*month - 1] + ((*month == 2 && leap) ? 1 : 0);
    return *day <= limit;
}


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
    int d, m, y;
    do {
        getStringInput("Enter Payment Date (DD/MM/YYYY): ", newPay.date, 20);
        if (!parseDate(newPay.date, &d, &m, &y)) {
            printf("[INVALID] Enter a valid date in DD/MM/YYYY format.\n");
        }
    } while (!parseDate(newPay.date, &d, &m, &y));
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

void checkMonthlyRentStatus(const Payment payments[], int payCount, const Tenant tenants[], int tCount,
                            const Property properties[], int pCount) {
    if (tCount == 0) {
        printf("\n[INFO] No tenants registered.\n");
        return;
    }

    int month, year;
    do {
        month = getIntInput("\nEnter Month (1-12): ");
        if (month < 1 || month > 12) {
            printf("[INVALID] Month must be between 1 and 12.\n");
        }
    } while (month < 1 || month > 12);
    do {
        year = getIntInput("Enter Year (e.g., 2026): ");
        if (year < 2000 || year > 2100) {
            printf("[INVALID] Enter a year between 2000 and 2100.\n");
        }
    } while (year < 2000 || year > 2100);

    printf("\n--- Monthly Rent Status for %02d/%d ---\n", month, year);
    printf("%-8s | %-20s | %-10s | %-10s | %s\n", "Tenant", "Name", "Rent Due", "Paid", "Status");
    printf("----------------------------------------------------------------\n");

    int paidCount = 0, dueCount = 0;
    for (int i = 0; i < tCount; i++) {
        float rent = -1;
        for (int j = 0; j < pCount; j++) {
            if (properties[j].propertyID == tenants[i].assignedPropertyID) {
                rent = properties[j].monthlyRent;
                break;
            }
        }
        if (rent < 0) {
            printf("%-8d | %-20s | %-10s | %-10s | %s\n", tenants[i].tenantID, tenants[i].name, "-", "-", "No property assigned");
            continue;
        }

        float paid = 0;
        for (int k = 0; k < payCount; k++) {
            int d, m, y;
            if (payments[k].tenantID == tenants[i].tenantID && parseDate(payments[k].date, &d, &m, &y) && m == month && y == year) {
                paid += payments[k].amountPaid;
            }
        }

        const char *status;
        if (paid >= rent - 0.005f) { status = "PAID"; paidCount++; }
        else { status = "DUE"; dueCount++; }

        printf("%-8d | %-20s | %-10.2f | %-10.2f | %s\n", tenants[i].tenantID, tenants[i].name, rent, paid, status);
    }
    printf("----------------------------------------------------------------\n");
    printf("Paid: %d | Due: %d\n", paidCount, dueCount);
}