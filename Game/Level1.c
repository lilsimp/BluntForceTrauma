/*---------------------------------------------------------------------------
Project Title         :       CS 230: Project 1
File Name             :       Level1.c
Author                :       Sawyer Simpson
Creation Date         :       1/7/2016
Purpose               :		    Contains the functions for Level1
History
-1/7/2016    :       Worked on and finished all functions.
-1/7/2016    :       Added file header.
� Copyright 1996-2016, DigiPen Institute of Technology (USA). All rights reserved.
----------------------------------------------------------------------------*/

#include "Level1.h"
#include "GameStateList.h"
#include "BinaryMap.h"
#include <stdio.h> /* fopen_s, fscanf_s, fprintf, fclose */

extern FILE* output;
extern unsigned int GSM_NEXT;

static int i = 0;
static int Flag = 0;

static int Level1_Health;
static int current_health;

/***************************************************************************/
/*!

	\brief
		Load in the amount of health for the start of Level 1

*/
/***************************************************************************/
void Level1_Load(void) {
	FILE* fp = fopen("Inputs/Level1_Health.txt", "rt");
	fscanf(fp, "%d", &Level1_Health);
	fclose(fp);

	printf("Level 1:\n");

	ImportMapDataFromFile("Inputs/Level1_Map.txt");

	PrintRetrievedInformation();

	printf("\n\n");

	fprintf(output, "Level1:Load\n");
}

/***************************************************************************/
/*!

	\brief
		Initialize health

*/
/***************************************************************************/
void Level1_Initialize(void) {
	current_health = Level1_Health;

	fprintf(output, "Level1:Initialize\n");
}

/***************************************************************************/
/*!

	\brief
		Update for Level 1

*/
/***************************************************************************/
void Level1_Update(void) {
	fprintf(output, "\n\n\nLevel1:Update\n");

	if (!(--current_health))
		GSM_NEXT = LEVEL_2;

	fprintf(output, "Cell value (%i, 1) = %i\n", ++i, GetCellValue(i, 1));

	Flag = 0;

	if(i == 1)
		Flag = CheckInstanceBinaryMapCollision(3.4f, 1.7f, 1.0f, 1.0f);
	else if(i == 2)
		Flag = CheckInstanceBinaryMapCollision(1.7f, 2.2f, 1.0f, 1.0f);
	else if(i == 3)
		Flag = CheckInstanceBinaryMapCollision(1.2f, 3.8f, 1.0f, 1.0f);

}

/***************************************************************************/
/*!

	\brief
		Draw for Level 1

*/
/***************************************************************************/
void Level1_Draw(void) {
	float f;

	fprintf(output, "\n\n\nLevel1:Draw\n");

	f = i + 1.73f;
	fprintf(output, "%f got snapped to: ", f);
	SnapToCell(&f);
	fprintf(output, "%f\n", f);

	if(i == 1)
		fprintf(output, "Flag for 3.4f, 1.7f, 1.0f, 1.0f is: %i     //You should get 1\n", Flag);
	else if(i == 2)
		fprintf(output, "Flag for 1.7f, 2.2f, 1.0f, 1.0f is: %i     //You should get 11\n", Flag);
	else if(i == 3)
		fprintf(output, "Flag for 1.2f, 3.8f, 1.0f, 1.0f is: %i     //You should get 15\n", Flag);

}

/***************************************************************************/
/*!

	\brief
		Free for Level 1

*/
/***************************************************************************/
void Level1_Free(void) {
	fprintf(output, "\n\n\nLevel1:Free\n");
}

/***************************************************************************/
/*!

	\brief
		Unload for Level 1

*/
/***************************************************************************/
void Level1_Unload(void) {
	FreeMapData();

	fprintf(output, "Level1:Unload\n");
}
