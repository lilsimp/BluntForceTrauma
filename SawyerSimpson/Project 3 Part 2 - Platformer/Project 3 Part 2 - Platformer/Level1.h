/*---------------------------------------------------------------------------
Project Title         :       CS 230: Project 1
File Name             :       Level1.h
Author                :       Sawyer Simpson
Creation Date         :       1/7/2016
Purpose               :       Contains prototypes for Level 1
History
-1/7/2016    :       Worked on and finished all prototypes.
-1/7/2016    :       Added file header.
� Copyright 1996-2016, DigiPen Institute of Technology (USA). All rights reserved.
----------------------------------------------------------------------------*/

#ifndef LEVEL_1_H
#define LEVEL_1_H

#include "Objects.h"
#include "BinaryMap.h"
#include "WorldSettings.h"
#include "PlayerStats.h"
#include "Matrix2D.h"
#include "Math2D.h"
#include "GameStateList.h"
#include "AEEngine.h"

void Level1_Load(void);

void Level1_Initialize(void);

void Level1_Update(void);

void Level1_Draw(void);

void Level1_Free(void);

void Level1_Unload(void);

#endif
