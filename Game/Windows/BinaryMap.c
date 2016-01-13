 /*---------------------------------------------------------------------------
Project Title         :       CS 230: Project 3 Part 1
File Name             :       BinaryMap.c
Author                :       Sawyer Simpson
Creation Date         :       2/12/2016
Purpose               :       Functions for BinaryMap.c
History
-2/12/2016    :       Worked on and finished all functions.
-2/12/2016    :       Added file header.
© Copyright 1996-2016, DigiPen Institute of Technology (USA). All rights reserved.
----------------------------------------------------------------------------*/

#include "BinaryMap.h"
#include <stdio.h>
#include <stdlib.h>

/*The number of horizontal elements*/
static unsigned int BINARY_MAP_WIDTH;

/*The number of vertical elements*/
static unsigned int BINARY_MAP_HEIGHT;

/*This will contain all the data of the map, which will be retireved from a file
when the "ImportMapDataFromFile" function is called*/
static int** MapData;

/*This will contain the collision dataof the binary map. It will be filled in the
"ImportMapDataFromFile" after filling "MapData". Basically, if an array element
in MapData is 1, it represents a collision cell, any other value is a non-collision
cell*/
static int** BinaryCollisionArray;

static int** make_grid(int** grid) {
	unsigned int i;

	grid = (int **) malloc(BINARY_MAP_HEIGHT * sizeof(int *));
	for(i = 0; i < BINARY_MAP_HEIGHT; ++i)
		grid[i] = (int *) malloc(BINARY_MAP_WIDTH * sizeof(int));

	return grid;
}

static void free_grid(int** grid) {
	unsigned int i;

	for(i = 0; i < BINARY_MAP_HEIGHT; ++i)
		free(grid[i]);
	free(grid);
}


int GetCellValue(unsigned int X, unsigned int Y) {
  if(X >= BINARY_MAP_WIDTH || Y >= BINARY_MAP_HEIGHT)
		return 0;

  return BinaryCollisionArray[X][Y];
}

int CheckInstanceBinaryMapCollision(float PosX, float PosY, float scaleX, float scaleY) {
	int FLAG = 0;
	float x1, x2, y1, y2;

	x1 = PosX - scaleX/2;
	y1 = PosY + scaleY/4;

	x2 = PosX - scaleX/2;
	y2 = PosX - scaleY/4;

	if(GetCellValue(x1, y1) || GetCellValue(x2, y2))
		FLAG |= COLLISION_LEFT;

	x1 = PosX + scaleX/2;
	y1 = PosY + scaleY/4;

	x2 = PosX + scaleX/2;
	y2 = PosX - scaleY/4;

	if(GetCellValue(x1, y1) || GetCellValue(x2, y2))
		FLAG |= COLLISION_RIGHT;

	x1 = PosX + scaleX/4;
	y1 = PosY + scaleY/2;

	x2 = PosX - scaleX/4;
	y2 = PosX + scaleY/2;

	if(GetCellValue(x1, y1) || GetCellValue(x2, y2))
		FLAG |= COLLISION_TOP;

	x1 = PosX + scaleX/4;
	y1 = PosY - scaleY/2;

	x2 = PosX - scaleX/4;
	y2 = PosX - scaleY/2;

	if(GetCellValue(x1, y1) || GetCellValue(x2, y2))
		FLAG |= COLLISION_BOTTOM;

	return FLAG;
}

void SnapToCell(float *Coordinate) {
	*Coordinate = (int)*Coordinate + 0.5f;
}

int ImportMapDataFromFile(char *FileName) {
	int i, j, value;
	FILE *fp = 0;
	fopen_s(fp, FileName, "rt");
	fscanf_s(fp, "Width %u\n", &BINARY_MAP_WIDTH);
	fscanf_s(fp, "Height %u\n", &BINARY_MAP_HEIGHT);

	MapData = make_grid(MapData);
	BinaryCollisionArray = make_grid(BinaryCollisionArray);

	for(j = BINARY_MAP_HEIGHT - 1; j >= 0; --j)
		for(i = 0; i < BINARY_MAP_WIDTH; ++i) {
			fscanf_s(fp, "%i ", &value);
			MapData[i][j] = value;
			BinaryCollisionArray[i][j] = ((value == 1) ? 1 : 0);
		}

	fclose(fp);

	return 1;
}

void FreeMapData(void) {
	free_grid(MapData);
	free_grid(BinaryCollisionArray);
}

void PrintRetrievedInformation(void) {
	int i, j;

	printf("Width: %i\n", BINARY_MAP_WIDTH);
	printf("Height: %i\n", BINARY_MAP_HEIGHT);

	printf("Map Data:\n");
	for(j = BINARY_MAP_HEIGHT - 1; j >= 0; --j)	{
		for(i = 0; i < BINARY_MAP_WIDTH; ++i)
			printf("%i ", MapData[i][j]);
		printf("\n");
	}

	printf("\n\nBinary Collision Data:\n");
	for(j = BINARY_MAP_HEIGHT - 1; j >= 0; --j) {
		for(i = 0; i < BINARY_MAP_WIDTH; ++i)
			printf("%i ", BinaryCollisionArray[i][j]);
		printf("\n");
	}
}
