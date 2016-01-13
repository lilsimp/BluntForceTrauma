#ifndef LEVEL_H
#define LEVEL_H

#include "Level1.h"
#include "Level2.h"

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
