# Macros ========================================

CC=gcc
CFLAGS=-O -ansi -pedantic
OUTDIR=gnu/
ERASE=rm
OBJECTS=$(OUTDIR)Main.o $(OUTDIR)GameStateManager.o $(OUTDIR)Input.o $(OUTDIR)System.o $(OUTDIR)BinaryMap.o $(OUTDIR)Level1.o $(OUTDIR)Level2.o
EXE=driver.exe

VALGRIND_OPTIONS=-q --leak-check=full --show-reachable=yes --trace-children=yes --tool=memcheck
# Targets ========================================

$(OUTDIR)$(EXE) : $(OBJECTS)
	$(CC) -o $(OUTDIR)$(EXE) $(OBJECTS)

$(OUTDIR)Main.o : Main.c
	$(CC) -o $(OUTDIR)Main.o $(CFLAGS) Main.c

$(OUTDIR)GameStateManager.o : GameStateManager.c
	$(CC) -o $(OUTDIR)GameStateManager.o $(CFLAGS) GameStateManager.c

$(OUTDIR)Input.o : Input.c
	$(CC) -o $(OUTDIR)Input.o $(CFLAGS) Input.c

$(OUTDIR)System.o : System.c
	$(CC) -o $(OUTDIR)System.o $(CFLAGS) System.c

$(OUTDIR)BinaryMap.o : BinaryMap.c
	$(CC) -o $(OUTDIR)BinaryMap.o $(CFLAGS) BinaryMap.c

$(OUTDIR)Level1.o : Level1.c
	$(CC) -o $(OUTDIR)Level1.o $(CFLAGS) Level1.c

$(OUTDIR)Level2.o : Level2.c
	$(CC) -o $(OUTDIR)Level2.o $(CFLAGS) Level2.c

clean :
	$(ERASE) $(OUTDIR)$(EXE) $(OBJECTS)
