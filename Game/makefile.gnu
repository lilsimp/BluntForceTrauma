# $(OUTDIR)$(EXE) : $(OBJECTS)
# 	$(CC) -o $(OUTDIR)$(EXE) $(OBJECTS)
#
# $(OUTDIR)Main.o : Main.c GameStateManager.h GameStateList.h System.h
# 	$(CC) $(CFLAGS) Main.c -o $(OUTDIR)Main.o
#
# $(OUTDIR)GameStateManager.o : GameStateManager.c GameStateManager.h GameStateList.h System.h Input.h Level1.h Level2.h Level3.h
# 	$(CC) $(CFLAGS) GameStateManager.c -o $(OUTDIR)GameStateManager.o
#
# $(OUTDIR)Input.o : Input.c Input.h
# 	$(CC) $(CFLAGS) Input.c -o $(OUTDIR)Input.o
#
# $(OUTDIR)System.o : System.c System.h
# 	$(CC) $(CFLAGS) System.c -o $(OUTDIR)System.o
#
# $(OUTDIR)BinaryMap.o : BinaryMap.c BinaryMap.h
# 	$(CC) $(CFLAGS) BinaryMap.c -o $(OUTDIR)BinaryMap.o
#
# $(OUTDIR)Level1.o : Level1.c Level1.h BinaryMap.h GameStateList.h
# 	$(CC) $(CFLAGS) Level1.c -o $(OUTDIR)Level1.o
#
# $(OUTDIR)Level2.o : Level2.c Level2.h BinaryMap.h GameStateList.h
# 	$(CC) $(CFLAGS) Level2.c -o $(OUTDIR)Level2.o
#
# $(OUTDIR)Level3.o : Level3.c Level3.h BinaryMap.h GameStateList.h
# 	$(CC) $(CFLAGS) Level3.c -o $(OUTDIR)Level3.o
#
# clean :
# 	$(ERASE) $(EXE) $(OBJECTS)
#

# Macros ========================================

CFLAGS=
OUTDIR=bin/
OBJECTS=Main.c GameStateManager.c Input.c System.c BinaryMap.c Objects.c Level1.c Level2.c Level3.c
CC=i586-mingw32msvc-gcc.sh
ERASE=rm

VALGRIND_OPTIONS=-q --leak-check=full --show-reachable=yes --trace-children=yes --tool=memcheck

EXE=game.exe
# Targets ========================================

all :
	$(CC) $(OBJECTS) $(CFLAGS) -o $(OUTDIR)$(EXE)

valgrind :
	cd $(OUTDIR) valgrind $(VALGRIND_OPTIONS) ./$(EXE) --line-numbers > /dev/null

clean :
	$(ERASE) $(OUTDIR)$(EXE)
