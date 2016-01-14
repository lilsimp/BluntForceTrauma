/*---------------------------------------------------------------------------
Project Title         :       CS 230: Project 1
File Name             :       Level2.c
Author                :       Sawyer Simpson
Creation Date         :       1/7/2016
Purpose               :       Contains the functions for Level2
History
-1/7/2016    :       Worked on and finished all functions.
-1/7/2016    :       Added file header.
� Copyright 1996-2016, DigiPen Institute of Technology (USA). All rights reserved.
----------------------------------------------------------------------------*/

#include "Level2.h"
#include "GameStateList.h"
#include "BinaryMap.h"
#include <stdio.h> /* fopen_s, fscanf_s, fprintf, fclose */

extern FILE* output;
extern unsigned int GSM_NEXT;



static int Level2_Health;
static int Level2_Lives;
static int current_health;
static int current_lives;

/***************************************************************************/
/*!

	\brief
		Load in the the amount of health and lives for the start of Level 2

*/
/***************************************************************************/
void Level2_Load(void) {
	FILE *fp;
	fp = fopen("Inputs/Level2_Health.txt", "r");
	fscanf(fp, "%d", &Level2_Health);
	fclose(fp);

	fp = fopen("Inputs/Level2_Lives.txt", "r");
	fscanf(fp, "%d", &Level2_Lives);
	fclose(fp);

	current_lives = Level2_Lives;

	fprintf(output, "Level2:Load\n");
}

/***************************************************************************/
/*!

	\brief
		Initialize health

*/
/***************************************************************************/
void Level2_Initialize(void) {
	current_health = Level2_Health;

	fprintf(output, "Level2:Initialize\n");
}

/***************************************************************************/
/*!

	\brief
		Update for Level 2

*/
/***************************************************************************/
void Level2_Update(void) {
	if (!(--current_health)) {
		GSM_NEXT = RESTART;
		if (!(--current_lives))
			GSM_NEXT = QUIT;
	}

	fprintf(output, "Level2:Update\n");
}

/***************************************************************************/
/*!

	\brief
		Draw for Level 2

*/
/***************************************************************************/
void Level2_Draw(void) {
	fprintf(output, "Level2:Draw\n");
}

/***************************************************************************/
/*!

	\brief
		Free for Level 2

*/
/***************************************************************************/
void Level2_Free(void) {
	fprintf(output, "Level2:Free\n");
}

/***************************************************************************/
/*!

	\brief
		Unload for Level 2

*/
/***************************************************************************/
void Level2_Unload(void) {
	fprintf(output, "Level2:Unload\n");
}
