/*---------------------------------------------------------------------------
Project Title         :       CS 230: Project 1
File Name             :       System.c
Author                :       Sawyer Simpson
Creation Date         :       1/7/2016
Purpose               :       Contains System calls for the game loop
History
-1/7/2016    :       Worked on and finished all functions.
-1/7/2016    :       Added file header.
� Copyright 1996-2016, DigiPen Institute of Technology (USA). All rights reserved.
----------------------------------------------------------------------------*/

#include "System.h"
#include <stdio.h> /* fopen_s, fprintf, fclose */

FILE* output;

/***************************************************************************/
/*!

	\brief
		Initializes the system

*/
/***************************************************************************/
void System_Initialize(void) {
	fopen_s(&output, "Output.txt", "wt"); /* Open the output file */

	fprintf(output, "System:Initialize\n");
}

/***************************************************************************/
/*!

	\brief
		Proporly exits the system

*/
/***************************************************************************/
void System_Exit(void) {
	fprintf(output, "System:Exit\n");

	fclose(output); /* Close the output file */
}
