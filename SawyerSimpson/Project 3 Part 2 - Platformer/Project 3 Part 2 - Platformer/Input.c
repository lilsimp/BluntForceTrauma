/*---------------------------------------------------------------------------
Project Title         :       CS 230: Project 1
File Name             :       Input.c
Author                :       Sawyer Simpson
Creation Date         :       1/7/2016
Purpose               :       Handles the inputs for the game
History
-1/7/2016    :       Worked on and finished all functions.
-1/7/2016    :       Added file header.
� Copyright 1996-2016, DigiPen Institute of Technology (USA). All rights reserved.
----------------------------------------------------------------------------*/

#include "Input.h"
#include "AEInput.h"
#include <stdio.h> /* fprintf */

extern FILE* output;

/***************************************************************************/
/*!

	\brief
		Gathers the inputs for the next game update

*/
/***************************************************************************/
void Input_Handle(void) {
	fprintf(output, "Input:Handle\n");
}
