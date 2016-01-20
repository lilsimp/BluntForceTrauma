/*---------------------------------------------------------------------------
Project Title         :       CS 230: Project 1
File Name             :       Level1.c
Author                :       Sawyer Simpson
Creation Date         :       1/7/2016
Purpose               :        Contains the functions for Level1
History
-1/7/2016    :       Worked on and finished all functions.
-1/7/2016    :       Added file header.
� Copyright 1996-2016, DigiPen Institute of Technology (USA). All rights reserved.
----------------------------------------------------------------------------*/

#include "Level1.h"
#include <stdio.h> /* fopen_s, fscanf_s, fprintf, fclose */

#define FLAG_ACTIVE      0x00000001

/* Collision flags */
#define  COLLISION_LEFT    0x00000001  /* 0001 */
#define  COLLISION_RIGHT   0x00000002  /* 0010 */
#define  COLLISION_TOP     0x00000004  /* 0100 */
#define  COLLISION_BOTTOM  0x00000008  /* 1000 */

/* GLOBALS */
extern FILE* output;
extern unsigned int GSM_NEXT;

/* list of original objects */
static GameObj      *sGameObjList;
static unsigned int    sGameObjNum;

/* list of object instances */
static GameObjInst    *sGameObjInstList;
static unsigned int    sGameObjInstNum;

/* Player Data */
static int Player_Initial_X;
static int Player_Initial_Y;
static int Player_Health;
static int Player_Lives;
static GameObjInst *pPlayer;

/* Map Information */
static Matrix2D MapTransform;
static MapData Level1_Map;
static int TotalCoins;

/* Current Player info */
static int current_health;
static int current_lives;

/***************************************************************************/
/*!

  \brief
    Load for Level 1

*/
/***************************************************************************/
void Level1_Load(void) {
  /* Used for the cicle shape */
  float CircleAngleStep, i;
  GameObj* pObj;
  Matrix2D mapScale, mapTrans;

  FILE* fp = fopen("Inputs/Level1_Health.txt", "rt");
  fscanf(fp, "%i", &Player_Health);
  fclose(fp);

  fp = fopen("Inputs/Level1_Lives.txt", "r");
  fscanf(fp, "%d", &Player_Lives);
  fclose(fp);

  current_lives = Player_Lives;

  AllGameObjects(sGameObjList, GAME_OBJ_NUM_MAX);
  AllGameObjectInsts(sGameObjInstList, GAME_OBJ_INST_NUM_MAX);

  sGameObjNum = 0;

  /* Creating the black object */
  pObj    = sGameObjList + sGameObjNum++;
  pObj->type  = TYPE_OBJECT_EMPTY;
  make_square_object(0.5f, 0.5f, 0xFF000000);
  pObj->pMesh = AEGfxMeshEnd();

  /* Creating the white object */
  pObj    = sGameObjList + sGameObjNum++;
  pObj->type  = TYPE_OBJECT_COLLISION;
  make_square_object(0.5f, 0.5f, 0xFFFFFFFF);
  pObj->pMesh = AEGfxMeshEnd();

  /* Creating the Player object */
  pObj    = sGameObjList + sGameObjNum++;
  pObj->type  = TYPE_OBJECT_PLAYER;
  make_square_object(0.5f, 0.5f, 0xFF0000FF);
  pObj->pMesh = AEGfxMeshEnd();

  /* Creating the enemey1 object */
  pObj    = sGameObjList + sGameObjNum++;
  pObj->type  = TYPE_OBJECT_ENEMY1;
  make_square_object(0.5f, 0.5f, 0xFFFF0000);
  pObj->pMesh = AEGfxMeshEnd();

  /* Creating the Coin object */
  pObj    = sGameObjList + sGameObjNum++;
  pObj->type  = TYPE_OBJECT_COIN;
  make_circle_object(12, 0xFFFFFF00);
  pObj->pMesh = AEGfxMeshEnd();

  /* MAKING PARTICLES NOW */

  /* Creating the Jump Particles */
  pObj    = sGameObjList + sGameObjNum++;
  pObj->type  = TYPE_OBJECT_BOOSTER;
  make_triangle_object(0.5f, 0.5f, 0xFFFF0000);
  pObj->pMesh = AEGfxMeshEnd();

  /* Creating the Player Explosion Particles */
  pObj    = sGameObjList + sGameObjNum++;
  pObj->type  = TYPE_OBJECT_PLAYER_BITS;
  make_square_object(0.5f, 0.5f, 0xFF0000FF, TYPE_OBJECT_PLAYER_BITS);
  pObj->pMesh = AEGfxMeshEnd();

  if(!ImportMapDataFromFile("Inputs/Level1_Map.txt", &Level1_Map))
    GSM_NEXT = QUIT;

  /*Computing the matrix which take a point out of the normalized coordinates system
  of the binary map */
  /***********
  Compute a transformation matrix and save it in "MapTransform".
  This transformation transforms any point from the normalized coordinates system of the binary map.
  Later on, when rendering each object instance, we should concatenate "MapTransform" with the
  object instance's own transformation matrix
  */

  /*
  Compute a translation matrix (-Grid width/2, -Grid height/2) and save it in "mapTrans"
  Compute a scaling matrix and save it in "mapScale")
  Concatenate scale * translate and save the result in "MapTransform"
  ***********/

  Matrix2DTranslate(&mapTrans, (-Level1_Map.width/2.0f), (-Level1_Map.height/2.0f));
  Matrix2DScale(&mapScale, 30.0f, 30.0f);
  Matrix2DConcat(&MapTransform, &mapScale, &mapTrans);

  fprintf(output, "Level1:Load\n");
}

/***************************************************************************/
/*!

  \brief
    Initialize for Level 1

*/
/***************************************************************************/
void Level1_Initialize(void) {
  int i, j;
  GameObjInst *pInst;
  Vector2D Pos;

  pPlayer = 0;

  /* Creating the map, the main character, the enemies and the coins according to their initial positions in MapData
  Looping twice might be needed in order to create the map cells first */

  /***********
  Loop through all the array elements of MapData (which was initialized in the "GameStatePlatformLoad" function
  from the .txt file

     - Create a white or black cell

     - if the element represents the Player
      Create a Player instance
      Set its position depending on its array indices in MapData
      Save its array indices in Player_Initial_X and Player_Initial_Y (Used when the Player dies and its position needs to be reset)

     - if the element represents an enemy
      Create an enemy instance
      Set its position depending on its array indices in MapData

     - if the element represents a coin
      Create a coin instance
      Set its position depending on its array indices in MapData

  ***********/
  for(i = 0; i < Level1_Map.width; ++i)
    for(j = 0; j < Level1_Map.height; ++j) {
      Vector2D pos;
      Vector2D vel = { 0 };
      pos.x = i + 0.5f;
      pos.y = j + 0.5f;

      switch(Level1_Map.BinaryCollisionArray[i][j]) {
        case TYPE_OBJECT_COLLISION:
          gameObjInstCreate(TYPE_OBJECT_COLLISION, 1.0f, &pos, &vel, 0, STATE_NONE);
          break;

        case TYPE_OBJECT_EMPTY:
          gameObjInstCreate(TYPE_OBJECT_EMPTY, 1.0f, &pos, &vel, 0, STATE_NONE);
          break;
      }
    }

  for(i = 0; i < Level1_Map.width; ++i)
    for(j = 0; j < Level1_Map.height; ++j) {
      Vector2D pos;
      Vector2D vel = { 0 };

      pos.x = i + 0.5f;
      pos.y = j + 0.5f;

      switch(Level1_Map.Map[i][j]) {
        case TYPE_OBJECT_PLAYER:
          pPlayer = gameObjInstCreate(TYPE_OBJECT_PLAYER, 1.0f, &pos, &vel, 0, STATE_NONE);
          Player_Initial_X = i + 0.5f;
          Player_Initial_Y = j + 0.5f;
          current_health   = Player_Health;
          break;

        case TYPE_OBJECT_ENEMY1:
          gameObjInstCreate(TYPE_OBJECT_ENEMY1, 1.0f, &pos, &vel, 0, STATE_GOING_LEFT);
          break;
      }
    }


  current_health = Player_Health;

  fprintf(output, "Level1:Initialize\n");
}

/***************************************************************************/
/*!

  \brief
    Update for Level 1

*/
/***************************************************************************/
void Level1_Update(void) {
  int i, j;
  GameObjInst *pInst;
  float winMaxX, winMaxY, winMinX, winMinY;
  double frameTime;

  /* ========================================================================================== */
  /* Getting the window's world edges (These changes whenever the camera moves or zooms in/out) */
  /* ========================================================================================== */
  winMaxX = AEGfxGetWinMaxX();
  winMaxY = AEGfxGetWinMaxY();
  winMinX = AEGfxGetWinMinX();
  winMinY = AEGfxGetWinMinY();


  /* ====================== */
  /* Getting the frame time */
  /* ====================== */

  frameTime = AEFrameRateControllerGetFrameTime();

/******************************************************************************/
  /* Handle Input */
  /***********
  if right is pressed
    Set Player velocity X to MOVE_VELOCITY_Player
  else
  if left is pressed
    Set Player velocity X to -MOVE_VELOCITY_Player
  else
    Set Player velocity X to 0

  if space is pressed AND Player is colliding from the bottom
    Set Player velocity Y to JUMP_VELOCITY

  if q is pressed
    Exit game
  ***********/
  pPlayer->velCurr.x = 0;
  if(AEInputCheckCurr(VK_RIGHT))
    pPlayer->velCurr.x = 10;

  else if(AEInputCheckCurr(VK_LEFT))
    pPlayer->velCurr.x = -10;

  if(AEInputCheckCurr(VK_SPACE) && pPlayer->gridCollisionFlag & COLLISION_BOTTOM)
    pPlayer->velCurr.y = JUMP_VELOCITY;

  if(AEInputCheckCurr('Q'))
    GSM_NEXT = QUIT;

  if(AEInputCheckCurr('N'))
    GSM_NEXT++;
/******************************************************************************/

/******************************************************************************/
  /* Update object instances physics and behavior */
  for(i = 0; i < GAME_OBJ_INST_NUM_MAX; ++i) {
    GameObjInst *pInst = sGameObjInstList + i;

    /* skip non-active object */
    if ((pInst->flag & FLAG_ACTIVE) == 0)
      continue;

    if(pInst->pObject->type == TYPE_OBJECT_COLLISION ||  pInst->pObject->type == TYPE_OBJECT_EMPTY)
      continue;


    /****************
    Apply gravity
      Velcoty Y = Gravity * Frame Time + Velocity Y

    If object instance is an enemy
      Apply enemy state machine
    ****************/

    pInst->velCurr.y = GRAVITY * frameTime + pInst->velCurr.y;

    if(pInst->pObject->type == TYPE_OBJECT_ENEMY1)
      EnemyStateMachine(pInst);

    if(pInst->pObject->type == TYPE_OBJECT_BOOSTER || pInst->pObject->type == TYPE_OBJECT_PLAYER_BITS) {
      pInst->counter -= frameTime;
      if(pInst->counter <= 0)
        gameObjInstDestroy(pInst);
    }
  }
/******************************************************************************/

/******************************************************************************/
  /* Update object instances positions */
  for(i = 0; i < GAME_OBJ_INST_NUM_MAX; ++i) {
    GameObjInst *pInst = sGameObjInstList + i;

    /* skip non-active object */
    if (0 == (pInst->flag & FLAG_ACTIVE))
      continue;

    /**********
    update the position using: P1 = V1*t + P0
    **********/

    pInst->posCurr.x += pInst->velCurr.x * frameTime;
    pInst->posCurr.y += pInst->velCurr.y * frameTime;
  }
/******************************************************************************/

/******************************************************************************/
  /* Check for grid collision */
  for(i = 0; i < GAME_OBJ_INST_NUM_MAX; ++i) {
    GameObjInst *pInst = sGameObjInstList + i;

    /* skip non-active object instances */
    if (0 == (pInst->flag & FLAG_ACTIVE))
      continue;

    if(pInst->pObject->type == TYPE_OBJECT_PLAYER_BITS)
      continue;

    /*************
    Update grid collision flag

    if collision from bottom
      Snap to cell on Y axis
      Velocity Y = 0

    if collision from top
      Snap to cell on Y axis
      Velocity Y = 0

    if collision from left
      Snap to cell on X axis
      Velocity X = 0

    if collision from right
      Snap to cell on X axis
      Velocity X = 0
    *************/

	pInst->gridCollisionFlag = CheckInstanceBinaryMapCollision(Level1_Map.BinaryCollisionArray, pInst->posCurr.x, pInst->posCurr.y, pInst->scale, pInst->scale);

    if(pInst->pObject->type == TYPE_OBJECT_BOOSTER && pInst->gridCollisionFlag > 0)
      gameObjInstDestroy(pInst);

    if(pInst->gridCollisionFlag & (COLLISION_BOTTOM | COLLISION_TOP))  {
      SnapToCell(&pInst->posCurr.y);
      pInst->velCurr.y = 0;
    }

    if(pInst->gridCollisionFlag & (COLLISION_LEFT | COLLISION_RIGHT))  {
      SnapToCell(&pInst->posCurr.x);
      pInst->velCurr.x = 0;
    }
  }
/******************************************************************************/

/******************************************************************************/
  /* Checking for collision among object instances: */
  /* Player against enemies */
  /* Player against coins */

  /**********
  for each game object instance
    Skip if it's inactive or if it's non collidable

    If it's an enemy
      If collision between the enemy instance and the Player (rectangle - rectangle)
        Decrement Player lives
        Reset the Player's position in case it has lives left, otherwise restart the level

    If it's a coin
      If collision between the coin instance and the Player (circle - rectangle)
        Remove the coin and decrement the coin counter.
        Quit the game in case no more coins are left
  **********/
  for(i = 0; i < GAME_OBJ_INST_NUM_MAX; ++i) {
    GameObjInst *pInst = sGameObjInstList + i;

    if (0 == (pInst->flag & FLAG_ACTIVE))
      continue;

    if(pInst->pObject->type == TYPE_OBJECT_ENEMY1) {
      if(StaticRectToStaticRect(&pInst->posCurr, 1, 1, &pPlayer->posCurr, 1, 1)) {
        current_lives--;

        /* explosionParticleCreate(pPlayer->posCurr); */

        pPlayer->posCurr.x = Player_Initial_X + 0.5f;
        pPlayer->posCurr.y = Player_Initial_Y + 0.5f;

        if(current_lives == 0)
          GSM_NEXT = RESTART;
      }
    }

    if(pInst->pObject->type == TYPE_OBJECT_COIN) {
      if(StaticCircleToStaticRect(&pInst->posCurr, 0.5f, &pPlayer->posCurr, 1, 1)) {
        gameObjInstDestroy(pInst);
        TotalCoins--;
        if(!TotalCoins)
          GSM_NEXT = QUIT;
      }
    }
  }
/******************************************************************************/

/******************************************************************************/
  /* Computing the transformation matrices of the game object instances */
  for(i = 0; i < GAME_OBJ_INST_NUM_MAX; ++i) {
    Matrix2D scale, rot, trans, temp;
    GameObjInst *pInst = sGameObjInstList + i;

    /* skip non-active object */
    if (0 == (pInst->flag & FLAG_ACTIVE))
      continue;

    Matrix2DScale(&scale, pInst->scale, pInst->scale);
    Matrix2DRotDeg(&rot, pInst->dirCurr);
    Matrix2DTranslate(&trans, pInst->posCurr.x, pInst->posCurr.y);

    Matrix2DConcat(&temp, &trans, &rot);
    Matrix2DConcat(&pInst->transform, &temp, &scale);
  }
/******************************************************************************/

  /*
  if(pPlayer->velCurr.y > 0)
    jumpeParticleCreate();
  */
  
  fprintf(output, "Level1:Update\n");
}

/***************************************************************************/
/*!

  \brief
    Draw for Level 1

*/
/***************************************************************************/
void Level1_Draw(void) {
	//Drawing the tile map (the grid)
	int i, j;
	Matrix2D cellTranslation, cellFinalTransformation;
	double frameTime;
	char strBuffer[100];

  /* Drawing the tile map (the grid)
  int i, j;
  Matrix2D cellTranslation, cellFinalTransformation;
  double frameTime;
  char strBuffer[100];


  /* ====================== */
  /* Getting the frame time */
  /* ====================== */

  frameTime = AEFrameRateControllerGetFrameTime();

  AEGfxSetRenderMode(AE_GFX_RM_COLOR);
  AEGfxTextureSet(NULL, 0, 0);
  AEGfxSetTintColor(1.0f, 1.0f, 1.0f, 1.0f);


  /* Drawing the tile map */

  /******REMINDER*****
  You need to concatenate MapTransform with the transformation matrix of any object you want to draw.
  MapTransform transform the instance from the normalized coordinates system of the binary map
  *******************/


  /* Drawing the object instances */
  /**********
  For each active and visible object instance
    Concatenate MapTransform with its transformation matrix
    Send the resultant matrix to the graphics manager using "AEGfxSetTransform"
    Draw the instance's shape using "AEGfxMeshDraw"
  **********/
  for (i = 0; i < GAME_OBJ_INST_NUM_MAX; ++i) {
    GameObjInst* pInst = sGameObjInstList + i;

    /* skip non-active object */
    if (0 == (pInst->flag & FLAG_ACTIVE))
      continue;

    /* Don't forget to concatenate the MapTransform matrix with the transformation of each game object instance */

    Matrix2DConcat(&cellTranslation, &MapTransform, &pInst->transform);
    AEGfxSetTransform(cellTranslation.m);
    AEGfxMeshDraw(pInst->pObject->pMesh, AE_GFX_MDM_TRIANGLES);
  }

  fprintf(output, "Level1:Draw\n");
}

/***************************************************************************/
/*!

  \brief
    Free for Level 1

*/
/***************************************************************************/
void Level1_Free(void) {
  unsigned int i;
  /* Destroy all object in the list */
  for (i = 0; i < GAME_OBJ_INST_NUM_MAX; ++i)
    gameObjInstDestroy(sGameObjInstList + i);

  fprintf(output, "Level1:Free\n");
}

/***************************************************************************/
/*!

  \brief
    Unload for Level 1

*/
/***************************************************************************/
void Level1_Unload(void) {
  unsigned int i;
  /* free all created meshes */
  for (i = 0; i < sGameObjNum; ++i)
    AEGfxMeshFree(sGameObjList[i].pMesh);

  /*********
  Free the map data
  *********/
  FreeMapData(&Level1_Map);

  fprintf(output, "Level1:Unload\n");
}
