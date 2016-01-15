/*---------------------------------------------------------------------------
Project Title         :       CS 230: Project 1
File Name             :       GameStateManager.c
Author                :       Sawyer Simpson
Creation Date         :       1/7/2016
Purpose               :       Manages the game and which level is playing
History
-1/7/2016    :       Worked on and finished all functions.
-1/7/2016    :       Added file header.
� Copyright 1996-2016, DigiPen Institute of Technology (USA). All rights reserved.
----------------------------------------------------------------------------*/

#include <stdio.h>  /* fprintf */
#include <stdlib.h>

#include "GameStateManager.h"
#include "GameStateList.h"
#include "Level1.h"
#include "Level2.h"
#include "Level.h"
#include "Input.h"

#define SET_ALL_LEVELS(NUM) \
	ALL_LEVELS[NUM - 1].pLoad   = Level##NUM##_Load; \
  ALL_LEVELS[NUM - 1].pInit   = Level##NUM##_Initialize; \
  ALL_LEVELS[NUM - 1].pUpdate = Level##NUM##_Update; \
  ALL_LEVELS[NUM - 1].pDraw   = Level##NUM##_Draw; \
  ALL_LEVELS[NUM - 1].pFree   = Level##NUM##_Free; \
  ALL_LEVELS[NUM - 1].pUnload = Level##NUM##_Unload


extern FILE* output;
static Level* ALL_LEVELS;
static Level GSM_LEVEL;
static unsigned int GSM_CURRENT;
static unsigned int GSM_PREVIOUS;
       unsigned int GSM_NEXT;			  /* Extern this wherever it needs to be changed, such as level files. */

static void SetAllLevels(void) {
	int i;
	ALL_LEVELS = (Level *)malloc(sizeof(Level) * LEVEL_NUM);
	SET_ALL_LEVELS(1);
	SET_ALL_LEVELS(2);
	SET_ALL_LEVELS(3);
}

/***************************************************************************/
/*!

	\brief
		Initializes the game loop to start at the given StartingState

*/
/***************************************************************************/
void GSM_Initialize(unsigned int StartingState) {
	GSM_CURRENT  = StartingState;
	GSM_PREVIOUS = StartingState;
	GSM_NEXT     = StartingState;

	SetAllLevels();

	fprintf(output, "GSM:Initialize\n");
}

/***************************************************************************/
/*!

	\brief
		Update the main gameloop if a level is requested to change

*/
/***************************************************************************/
void GSM_Update(void) {
	GSM_LEVEL = ALL_LEVELS[GSM_CURRENT];

	fprintf(output, "GSM:Update\n");
}

/***************************************************************************/
/*!

	\brief
		Holds the main gameloop

*/
/***************************************************************************/
void GSM_MainLoop(void) {
	while (GSM_CURRENT != QUIT) {
		if (GSM_CURRENT == RESTART) {
			GSM_CURRENT = GSM_PREVIOUS;
			GSM_NEXT    = GSM_PREVIOUS;
		}
		else {
			GSM_Update();
			GSM_LEVEL.pLoad();
		}

		GSM_LEVEL.pInit();

		while (GSM_CURRENT == GSM_NEXT) {
			Input_Handle();
			GSM_LEVEL.pUpdate();
			GSM_LEVEL.pDraw();
		}

		GSM_LEVEL.pFree();

		if (GSM_NEXT != RESTART)
			GSM_LEVEL.pUnload();

		GSM_PREVIOUS = GSM_CURRENT;
		GSM_CURRENT  = GSM_NEXT;
	}
}
