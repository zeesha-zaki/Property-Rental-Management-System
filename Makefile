CC = gcc
CFLAGS = -Wall -Wextra -std=c99
TARGET = rental_app
OBJS = main.o auth.o property_admin.o tenant_mgmt.o payment_mgmt.o reports.o

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

%.o: %.c rental_system.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f *.o $(TARGET) *.dat summary_report.txt

run: $(TARGET)
	./$(TARGET)