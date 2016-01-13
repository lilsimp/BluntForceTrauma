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

extern FILE* output;
static Level* ALL_LEVELS;
static Level GSM_LEVEL;
static unsigned int GSM_CURRENT;
static unsigned int GSM_PREVIOUS;
       unsigned int GSM_NEXT;			  /* Extern this wherever it needs to be changed, suc has level files. */

static void SetAllLevels(void) {
	ALL_LEVELS = (Level *)malloc(sizeof(Level) * LEVEL_NUM);

	ALL_LEVELS[0].pLoad   = Level1_Load;
  ALL_LEVELS[0].pInit   = Level1_Initialize;
  ALL_LEVELS[0].pUpdate = Level1_Update;
  ALL_LEVELS[0].pDraw   = Level1_Draw;
  ALL_LEVELS[0].pFree   = Level1_Free;
  ALL_LEVELS[0].pUnload = Level1_Unload;

  ALL_LEVELS[1].pLoad   = Level2_Load;
  ALL_LEVELS[1].pInit   = Level2_Initialize;
  ALL_LEVELS[1].pUpdate = Level2_Update;
  ALL_LEVELS[1].pDraw   = Level2_Draw;
  ALL_LEVELS[1].pFree   = Level2_Free;
  ALL_LEVELS[1].pUnload = Level2_Unload;
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
