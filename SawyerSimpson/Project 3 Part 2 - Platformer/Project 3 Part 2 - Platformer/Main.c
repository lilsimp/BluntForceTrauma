/*---------------------------------------------------------------------------
Project Title         :       CS 230: Project 1
File Name             :       Main.c
Author                :       Sawyer Simpson
Creation Date         :       1/7/2016
Purpose               :       Contains the Main calls for the game
History
-1/7/2016    :       Worked on and finished all functions.
-1/7/2016    :       Added file header.
� Copyright 1996-2016, DigiPen Institute of Technology (USA). All rights reserved.
----------------------------------------------------------------------------*/

#include "AEEngine.h"
#include "GameStateManager.h"
#include "GameStateList.h"
#include "System.h"

int WINAPI WinMain(HINSTANCE instanceH, HINSTANCE prevInstanceH, LPSTR command_line, int show) {	/* Initialize the system*/
	if (System_Initialize(instanceH, prevInstanceH, command_line, show) == 1)
		return 0;

	/* Initialize the GSM */
	GSM_Initialize(LEVEL_1);

	/* Game loop */
	GSM_MainLoop();

	/* Free the system */
	System_Exit();

	return 0;
}
