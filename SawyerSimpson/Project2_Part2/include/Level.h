#ifndef LEVEL_H
#define LEVEL_H

typedef void (*fp)(void);

typedef struct Level{
	fp pLoad;
	fp pInit;
	fp pUpdate;
	fp pDraw;
	fp pFree;
	fp pUnload;
}Level;

#endif
