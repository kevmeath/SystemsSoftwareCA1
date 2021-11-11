CC=gcc
CFLAGS=-Wall -g
HEADERS=backup.h paths.h permissions.h transfer.h
OBJECTS=backup.o permissions.o transfer.o 
MAIN_BIN=deparment-report-daemon
TEST_BIN=test
BINARIES=$(MAIN_BIN) $(TEST_BIN)

main: $(OBJECTS)
	$(CC) -o $(MAIN_BIN) daemon.c $(OBJECTS) $(CFLAGS)

test: $(OBJECTS)
	$(CC) -o $(TEST_BIN) test.c $(OBJECTS) $(CFLAGS)

backup.o: backup.c backup.h paths.h
	$(CC) -c backup.c

permissions.o: permissions.c permissions.h paths.h
	$(CC) -c permissions.c

transfer.o: transfer.c transfer.h paths.h
	$(CC) -c transfer.c

clean:
	rm $(OBJECTS) $(BINARIES)
