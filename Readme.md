# Property Rental Management System (CS-115 Term Project)

A modular multi-file application developed in C for the **CS-115 Computer Programming (Fall Semester 2026)** course[cite: 1, 2].

## File Map & Assignments
- `rental_system.h` & `auth.c` & `property_admin.c`: Student 1
- `tenant_mgmt.c`: Student 2
- `payment_mgmt.c`: Student 3
- `reports.c` & `main.c` & `Makefile`: Student 4

## Compilation and Running

### GCC One-Liner
```bash
gcc main.c auth.c property_admin.c tenant_mgmt.c payment_mgmt.c reports.c -o rental_app
./rental_app      # Linux / macOS
rental_app.exe    # Windows