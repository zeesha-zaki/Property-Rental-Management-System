# Property Rental Management System (CS-115 Semester Project)

A modular multi-file application developed in C for the **CS-115 Computer Programming (Fall Semester 2026)**.

## File Map & Assignments
- `rental_system.h` & `auth.c` & `property_admin.c`: Saad Tariq
- `tenant_mgmt.c`: Zeesha Zaki
- `payment_mgmt.c`: Ahsan Jawed
- `reports.c` & `main.c` & `Makefile`: Abiha Laeeq

## Compilation and Running

### GCC One-Liner
```bash
gcc main.c auth.c property_admin.c tenant_mgmt.c payment_mgmt.c reports.c -o rental_app
./rental_app      # Linux / macOS
rental_app.exe    # Windows
