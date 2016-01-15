/*---------------------------------------------------------------------------
Project Title         :       CS 230: Project 1
File Name             :       Level3.c
Author                :       Sawyer Simpson
Creation Date         :       1/7/2016
Purpose               :		    Contains the functions for Level3
History
-1/7/2016    :       Worked on and finished all functions.
-1/7/2016    :       Added file header.
� Copyright 1996-2016, DigiPen Institute of Technology (USA). All rights reserved.
----------------------------------------------------------------------------*/

#include "Level3.h"
#include "GameStateList.h"
#include "BinaryMap.h"
#include <stdio.h> /* fopen_s, fscanf_s, fprintf, fclose */

extern FILE* output;
extern unsigned int GSM_NEXT;

static int Level3_Health;
static int current_health;

/***************************************************************************/
/*!

	\brief
		Load in the amount of health for the start of Level 3

*/
/***************************************************************************/
void Level3_Load(void) {
	FILE* fp = fopen("Inputs/Level3_Health.txt", "rt");
	fscanf(fp, "%d", &Level3_Health);
	fclose(fp);

	printf("Level 3:\n");

	ImportMapDataFromFile("Inputs/Level3_Map.txt");

	PrintRetrievedInformation();

	printf("\n\n");

	fprintf(output, "Level3:Load\n");
}

/***************************************************************************/
/*!

	\brief
		Initialize health

*/
/***************************************************************************/
void Level3_Initialize(void) {
	current_health = Level3_Health;

	fprintf(output, "Level3:Initialize\n");
}

/***************************************************************************/
/*!

	\brief
		Update for Level 3

*/
/***************************************************************************/
void Level3_Update(void) {
	fprintf(output, "Level3:Update\n");

	if (!(--current_health))
		GSM_NEXT = QUIT;
}

/***************************************************************************/
/*!

	\brief
		Draw for Level 3

*/
/***************************************************************************/
void Level3_Draw(void) {
	fprintf(output, "Level3:Draw\n");
}

/***************************************************************************/
/*!

	\brief
		Free for Level 3

*/
/***************************************************************************/
void Level3_Free(void) {
	fprintf(output, "Level3:Free\n");
}

/***************************************************************************/
/*!

	\brief
		Unload for Level 3

*/
/***************************************************************************/
void Level3_Unload(void) {
	FreeMapData();

	fprintf(output, "Level3:Unload\n");
}
