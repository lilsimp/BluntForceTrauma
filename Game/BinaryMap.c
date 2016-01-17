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

/*This will contain the collision dataof the binary map. It will be filled in the
"ImportMapDataFromFile" after filling "MapData". Basically, if an array element
in MapData is 1, it represents a collision cell, any other value is a non-collision
cell*/

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


int GetCellValue(unsigned int X, unsigned int Y, int** BinaryCollisionArray) {
  if(X >= BINARY_MAP_WIDTH || Y >= BINARY_MAP_HEIGHT)
		return 0;

  return BinaryCollisionArray[X][Y];
}

int CheckInstanceBinaryMapCollision(int** BinaryCollisionArray, float PosX, float PosY, float scaleX, float scaleY) {
	int FLAG = 0;
	float x1, x2, y1, y2;

	x1 = PosX - scaleX/2;
	y1 = PosY + scaleY/4;

	x2 = PosX - scaleX/2;
	y2 = PosX - scaleY/4;

	if(GetCellValue(x1, y1) || GetCellValue(x2, y2, BinaryCollisionArray))
		FLAG |= COLLISION_LEFT;

	x1 = PosX + scaleX/2;
	y1 = PosY + scaleY/4;

	x2 = PosX + scaleX/2;
	y2 = PosX - scaleY/4;

	if(GetCellValue(x1, y1, BinaryCollisionArray) || GetCellValue(x2, y2, BinaryCollisionArray))
		FLAG |= COLLISION_RIGHT;

	x1 = PosX + scaleX/4;
	y1 = PosY + scaleY/2;

	x2 = PosX - scaleX/4;
	y2 = PosX + scaleY/2;

	if(GetCellValue(x1, y1, BinaryCollisionArray) || GetCellValue(x2, y2, BinaryCollisionArray))
		FLAG |= COLLISION_TOP;

	x1 = PosX + scaleX/4;
	y1 = PosY - scaleY/2;

	x2 = PosX - scaleX/4;
	y2 = PosX - scaleY/2;

	if(GetCellValue(x1, y1, BinaryCollisionArray) || GetCellValue(x2, y2, BinaryCollisionArray))
		FLAG |= COLLISION_BOTTOM;

	return FLAG;
}

void SnapToCell(float *Coordinate) {
	*Coordinate = (int)*Coordinate + 0.5f;
}

int ImportMapDataFromFile(char *FileName, MapData* map) {
	int i, j, value;
	FILE *fp = fopen(FileName, "rt");
	fscanf(fp, "Width %u\n", &map->width);
	fscanf(fp, "Height %u\n", &map->height);

	map->Map = make_grid(map->Map);
	map->BinaryCollisionArray = make_grid(map->BinaryCollisionArray);

	for(j = map->height - 1; j >= 0; --j)
		for(i = 0; i < map->width; ++i) {
			fscanf(fp, "%i ", &value);
			map->Map[i][j] = value;
			map->BinaryCollisionArray[i][j] = ((value == 1) ? 1 : 0);
		}

	fclose(fp);

	return 1;
}

void FreeMapData(MapData* map) {
	free_grid(map->Map);
	free_grid(map->BinaryCollisionArray);
}

void PrintRetrievedInformation(MapData* map) {
	int i, j;

	printf("Width: %i\n", map->width);
	printf("Height: %i\n", map->height);

	printf("Map Data:\n");
	for(j = map->height - 1; j >= 0; --j)	{
		for(i = 0; i < map->width; ++i)
			printf("%i ", map->Map[i][j]);

		printf("\n");
	}

	printf("\n\nBinary Collision Data:\n");
	for(j = map->height - 1; j >= 0; --j) {
		for(i = 0; i < map->width; ++i)
			printf("%i ", map->BinaryCollisionArray[i][j]);

		printf("\n");
	}
}
