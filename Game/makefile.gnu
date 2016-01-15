# Macros ========================================

CC=gcc
CFLAGS=-O -ansi -pedantic
ERASE=rm
OBJECTS=Main.o GameStateManager.o Input.o System.o BinaryMap.o Level1.o Level2.o Level3.o
EXE=driver.exe

VALGRIND_OPTIONS=-q --leak-check=full --show-reachable=yes --trace-children=yes --tool=memcheck
# Targets ========================================

$(EXE) : $(OBJECTS)
	$(CC) -o $(EXE) $(OBJECTS)

Main.o : Main.c
	$(CC) -o Main.o $(CFLAGS) Main.c

GameStateManager.o : GameStateManager.c
	$(CC) -o GameStateManager.o $(CFLAGS) GameStateManager.c

Input.o : Input.c
	$(CC) -o Input.o $(CFLAGS) Input.c

System.o : System.c
	$(CC) -o System.o $(CFLAGS) System.c

BinaryMap.o : BinaryMap.c
	$(CC) -o BinaryMap.o $(CFLAGS) BinaryMap.c

Level1.o : Level1.c
	$(CC) -o Level1.o $(CFLAGS) Level1.c

Level2.o : Level2.c
	$(CC) -o Level2.o $(CFLAGS) Level2.c

Level3.o : Level3.c
	$(CC) -o Level3.o $(CFLAGS) Level3.c

clean :
	$(ERASE) $(EXE) $(OBJECTS)
