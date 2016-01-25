﻿// ---------------------------------------------------------------------------
// Project Name		:	Asteroid Game
// File Name		:	GameState_Play.c
// Author			:	Antoine Abi Chacra
// Creation Date	:	2008/01/31
// Purpose			:	implementation of the 'play' game state
// History			:
// - 2015/12/10		:	- Implemented C style component based architecture 
// ---------------------------------------------------------------------------

#include "main.h"
#include "Matrix2D.h"

// ---------------------------------------------------------------------------
// Defines

#define SHAPE_NUM_MAX				32					// The total number of different vertex buffer (Shape)
#define GAME_OBJ_INST_NUM_MAX		2048				// The total number of different game object instances


// Feel free to change these values in ordet to make the game more fun
#define SHIP_INITIAL_NUM			3					// Initial number of ship lives
#define SHIP_SIZE					15.0f				// Ship size
#define SHIP_ACCEL_FORWARD			50.0f				// Ship forward acceleration (in m/s^2)
#define SHIP_ACCEL_BACKWARD			-100.0f				// Ship backward acceleration (in m/s^2)
#define SHIP_ROT_SPEED				(2.0f * PI)			// Ship rotation speed (radian/second)
#define HOMING_MISSILE_ROT_SPEED	(PI / 2.0f)			// Homing missile rotation speed (radian/second)
#define BULLET_SPEED				100.0f				// Bullet speed (m/s)
#define MAX_SPEED                   40.0f

// ---------------------------------------------------------------------------

enum OBJECT_TYPE
{
	// list of game object types
	OBJECT_TYPE_SHIP = 0,
	OBJECT_TYPE_BULLET,
	OBJECT_TYPE_ASTEROID_SQUARE,
	OBJECT_TYPE_ASTEROID_CIRCLE,
	OBJECT_TYPE_HOMING_MISSILE,

	OBJECT_TYPE_NUM
};

// ---------------------------------------------------------------------------
// object mFlag definition

#define FLAG_ACTIVE		0x00000001

// ---------------------------------------------------------------------------
// Struct/Class definitions

typedef struct GameObjectInstance GameObjectInstance;			// Forward declaration needed, since components need to point to their owner "GameObjectInstance"

// ---------------------------------------------------------------------------

typedef struct 
{
	unsigned long			mType;				// Object type (Ship, bullet, etc..)
	AEGfxVertexList*		mpMesh;				// This will hold the triangles which will form the shape of the object

	GameObjectInstance *	mpOwner;			// This component's owner
}Shape;

// ---------------------------------------------------------------------------

typedef struct
{
	Shape *mpShape;

	GameObjectInstance *	mpOwner;			// This component's owner
}Component_Sprite;

// ---------------------------------------------------------------------------

typedef struct
{
	Vector2D				mPosition;			// Current position
	float					mAngle;				// Current angle
	float					mScaleX;			// Current X scaling value
	float					mScaleY;			// Current Y scaling value

	Matrix2D					mTransform;			// Object transformation matrix: Each frame, calculate the object instance's transformation matrix and save it here

	GameObjectInstance *	mpOwner;			// This component's owner
}Component_Transform;

// ---------------------------------------------------------------------------

typedef struct
{
	Vector2D				mVelocity;			// Current velocity

	GameObjectInstance *	mpOwner;			// This component's owner
}Component_Physics;

// ---------------------------------------------------------------------------

typedef struct
{
	GameObjectInstance *		mpTarget;		// Target, used by the homing missile

	GameObjectInstance *		mpOwner;		// This component's owner
}Component_Target;

// ---------------------------------------------------------------------------


//Game object instance structure
struct GameObjectInstance
{
	unsigned long				mFlag;						// Bit mFlag, used to indicate if the object instance is active or not

	Component_Sprite			*mpComponent_Sprite;		// Sprite component
	Component_Transform			*mpComponent_Transform;		// Transform component
	Component_Physics			*mpComponent_Physics;		// Physics component
	Component_Target			*mpComponent_Target;		// Target component, used by the homing missile
};

// ---------------------------------------------------------------------------
// Static variables

// List of original vertex buffers
static Shape				sgShapes[SHAPE_NUM_MAX];									// Each element in this array represents a unique shape 
static unsigned long		sgShapeNum;													// The number of defined shapes

// list of object instances
static GameObjectInstance	sgGameObjectInstanceList[GAME_OBJ_INST_NUM_MAX];	        // Each element in this array represents a unique game object instance
static unsigned long		sgGameObjectInstanceNum;				           
// pointer ot the ship object
static GameObjectInstance*			sgpShip;											// Pointer to the "Ship" game object instance

// number of ship available (lives 0 = game over)
static long					sgShipLives;												// The number of lives left

// the score = number of asteroid destroyed
static unsigned long		sgScore;													// Current score

// ---------------------------------------------------------------------------

// functions to create/destroy a game object instance
static GameObjectInstance*			GameObjectInstanceCreate(unsigned int ObjectType);			// From OBJECT_TYPE enum
static void							GameObjectInstanceDestroy(GameObjectInstance* pInst);

// ---------------------------------------------------------------------------

// Functions to add/remove components
static void AddComponent_Transform(GameObjectInstance *pInst, Vector2D *pPosition, float Angle, float ScaleX, float ScaleY);
static void AddComponent_Sprite(GameObjectInstance *pInst, unsigned int ShapeType);
static void AddComponent_Physics(GameObjectInstance *pInst, Vector2D *pVelocity);
static void AddComponent_Target(GameObjectInstance *pInst, GameObjectInstance *pTarget);

static void RemoveComponent_Transform(GameObjectInstance *pInst);
static void RemoveComponent_Sprite(GameObjectInstance *pInst);
static void RemoveComponent_Physics(GameObjectInstance *pInst);
static void RemoveComponent_Target(GameObjectInstance *pInst);

static AEGfxVertexList* make_halfcap_shape(float x, float y, int parts, int color);
static AEGfxVertexList* make_triangle_object(float x, float y, int color);
static AEGfxVertexList* make_rectangle_object(float x, float y, int color);
static AEGfxVertexList* make_circle_object(int parts, int color);

// ---------------------------------------------------------------------------

// "Load" function of this state
void GameStateAsteroidsLoad(void)
{
	float CircleAngleStep;
	int i;

	Shape* pShape = NULL;

	// Zero the shapes array
	memset(sgShapes, 0, sizeof(Shape) * SHAPE_NUM_MAX);
	// No shapes at this point
	sgShapeNum = 0;

	// The ship object instance hasn't been created yet, so this "sgpShip" pointer is initialized to 0
	sgpShip = 0;



	/// Create the game objects(shapes) : Ships, Bullet, Asteroid and Missile
	// How to:
	// -- Remember to create normalized shapes, which means all the vertices' coordinates should be in the [-0.5;0.5] range. Use the object instances' scale values to resize the shape.
	// -- Call “AEGfxMeshStart” to inform the graphics manager that you are about the start sending triangles.
	// -- Call “AEGfxTriAdd” to add 1 triangle.
	// -- A triangle is formed by 3 counter clockwise vertices (points).
	// -- Create all the points between (-0.5, -0.5) and (0.5, 0.5), and use the object instance's scale to change the size.
	// -- Each point can have its own color.
	// -- The color format is : ARGB, where each 2 hexadecimal digits represent the value of the Alpha, Red, Green and Blue respectively. Note that alpha blending(Transparency) is not implemented.
	// -- Each point can have its own texture coordinate (set them to 0.0f in case you’re not applying a texture).
	// -- An object (Shape) can have multiple triangles.
	// -- Call “AEGfxMeshEnd” to inform the graphics manager that you are done creating a mesh, which will return a pointer to the mesh you just created.

	

	// =====================
	// Create the ship shape
	// =====================

	pShape = sgShapes + sgShapeNum++;
	pShape->mType = OBJECT_TYPE_SHIP;
	pShape->mpMesh = make_triangle_object(0.5f, 0.5f, 0xFFFF0000);


	/////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////
	// TO DO 4:
	// -- Create the bullet shape
	/////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////

	pShape = sgShapes + sgShapeNum++;
	pShape->mType = OBJECT_TYPE_BULLET;
	pShape->mpMesh = make_rectangle_object(0.25f, 1.0f, 0x00CCFF);

	/////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////
	// TO DO 6:
	// -- Create the asteroid shape
	/////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////

	pShape = sgShapes + sgShapeNum++;
	pShape->mType = OBJECT_TYPE_ASTEROID_SQUARE;
	pShape->mpMesh = make_rectangle_object(0.35f, 0.8f, 0x663333);

	/////////////////////////////////////////////////////////////////////////////////////////////////
	// -- Create the asteroid shape circle
	/////////////////////////////////////////////////////////////////////////////////////////////////

	pShape = sgShapes + sgShapeNum++;
	pShape->mType = OBJECT_TYPE_ASTEROID_SQUARE;
	pShape->mpMesh = make_circle_object(12, 0x663333);

	/////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////
	// TO DO 9:
	// -- Create the homing missile shape
	/////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////



	/////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////
	// TO DO 10:
	// -- Create the star shape
	/////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////

}

// ---------------------------------------------------------------------------

// "Initialize" function of this state
void GameStateAsteroidsInit(void)
{
	int i;

	AEGfxSetBackgroundColor(0.0f, 0.0f, 0.0f);
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);

	// zero the game object instance array
	memset(sgGameObjectInstanceList, 0, sizeof(GameObjectInstance)* GAME_OBJ_INST_NUM_MAX);
	// No game object instances (sprites) at this point
	sgGameObjectInstanceNum = 0;

	// create the main ship
	sgpShip = GameObjectInstanceCreate(OBJECT_TYPE_SHIP);

	/////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////
	// TO DO 7:
	// -- Create at least 3 asteroid instances, each with a different size, 
	//    using the "GameObjInstCreate" function
	// -- Make sure to add their components!
	/////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////

	for (i = 0; i < 5; ++i) {
		GameObjectInstance* pAsteroid;
		Vector2D velocity;

		Vector2DSet(&velocity, ((float)rand() / (float)(RAND_MAX)) * 3.0f, ((float)rand() / (float)(RAND_MAX)) * 3.0f);

		pAsteroid = GameObjectInstanceCreate((i % 2) ? OBJECT_TYPE_ASTEROID_SQUARE : OBJECT_TYPE_ASTEROID_CIRCLE);
		AddComponent_Transform(pAsteroid, &sgpShip->mpComponent_Transform->mPosition, sgpShip->mpComponent_Transform->mAngle, ((float)rand() / (float)(RAND_MAX)) * 5.0f, ((float)rand() / (float)(RAND_MAX)) * 5.0f);
		AddComponent_Physics(pAsteroid, &velocity);
	}

	// reset the score and the number of ship
	sgScore			= 0;
	sgShipLives		= SHIP_INITIAL_NUM;
}

// ---------------------------------------------------------------------------

// "Update" function of this state
void GameStateAsteroidsUpdate(void)
{
	unsigned long i;
	float winMaxX, winMaxY, winMinX, winMinY;
	double frameTime;

	// ==========================================================================================
	// Getting the window's world edges (These changes whenever the camera moves or zooms in/out)
	// ==========================================================================================
	winMaxX = AEGfxGetWinMaxX();
	winMaxY = AEGfxGetWinMaxY();
	winMinX = AEGfxGetWinMinX();
	winMinY = AEGfxGetWinMinY();

	
	// ======================
	// Getting the frame time
	// ======================

	frameTime = AEFrameRateControllerGetFrameTime();

	// =========================
	// Update according to input
	// =========================

	/////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////
	// TO DO 3:
	// -- Compute the forward/backward acceleration of the ship when Up/Down are pressed
	// -- Use the acceleration to update the velocity of the ship
	// -- Limit the maximum velocity of the ship
	// -- IMPORTANT: The current input code moves the ship by simply adjusting its position
	/////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////
	if (AEInputCheckCurr(VK_UP))
	{
		Vector2D direction;
		float current_speed;

		Vector2DSet(&direction, cosf(sgpShip->mpComponent_Transform->mAngle), sinf(sgpShip->mpComponent_Transform->mAngle));
		Vector2DNormalize(&direction, &direction);
		Vector2DScale(&direction, &direction, SHIP_ACCEL_FORWARD * frameTime);
		Vector2DAdd(&sgpShip->mpComponent_Physics->mVelocity, &sgpShip->mpComponent_Physics->mVelocity, &direction);

		if ((current_speed > (MAX_SPEED * MAX_SPEED)) || (current_speed < (MAX_SPEED * MAX_SPEED))) {
			Vector2D temp;

			Vector2DNormalize(&temp, &sgpShip->mpComponent_Physics->mVelocity);
			Vector2DScale(&sgpShip->mpComponent_Physics->mVelocity, &temp, MAX_SPEED);
		}
	}

	if (AEInputCheckCurr(VK_DOWN))
	{
		Vector2D direction;
		float current_speed;

		Vector2DSet(&direction, -cosf(sgpShip->mpComponent_Transform->mAngle), -sinf(sgpShip->mpComponent_Transform->mAngle));
		Vector2DNormalize(&direction, &direction);
		Vector2DScale(&direction, &direction, SHIP_ACCEL_BACKWARD * frameTime);
		Vector2DAdd(&sgpShip->mpComponent_Physics->mVelocity, &sgpShip->mpComponent_Physics->mVelocity, &direction);

		Vector2DAdd(&sgpShip->mpComponent_Transform->mPosition, &sgpShip->mpComponent_Transform->mPosition, &direction);
	}

	if (AEInputCheckCurr(VK_LEFT))
	{
		sgpShip->mpComponent_Transform->mAngle += SHIP_ROT_SPEED * (float)(frameTime);
		sgpShip->mpComponent_Transform->mAngle = AEWrap(sgpShip->mpComponent_Transform->mAngle, -PI, PI);
	}

	if (AEInputCheckCurr(VK_RIGHT))
	{
		sgpShip->mpComponent_Transform->mAngle -= SHIP_ROT_SPEED * (float)(frameTime);
		sgpShip->mpComponent_Transform->mAngle = AEWrap(sgpShip->mpComponent_Transform->mAngle, -PI, PI);
	}

	/////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////
	// TO DO 5:
	// -- Create a bullet instance when SPACE is triggered, using the "GameObjInstCreate" function
	// -- Make sure to add its components!
	/////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////
	if (AEInputCheckTriggered(VK_SPACE))
	{
		GameObjectInstance* bullet;
		Vector2D velocity;
		
		Vector2DSet(&velocity, BULLET_SPEED, BULLET_SPEED);
		Vector2DAdd(&velocity, &velocity, &sgpShip->mpComponent_Physics->mVelocity);

		bullet = GameObjInstCreate(OBJECT_TYPE_BULLET);
		AddComponent_Transform(bullet, &sgpShip->mpComponent_Transform->mPosition, sgpShip->mpComponent_Transform->mAngle, 0.5f, 0.5f);
		AddComponent_Physics(bullet, &velocity);
	}

	/////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////
	// TO DO 10:
	// -- Create a homing missile instance when M is triggered
	// -- Make sure to add its components!
	/////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////
	if (AEInputCheckTriggered('M'))
	{

	}


	/////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////
	// TO DO 2:
	// Update the positions of all active game object instances
	// -- Positions are updated here (current postion += current velocity * frametime)
	// -- If implemented correctly, you will be able to control the ship (basic 2D movement)
	/////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////
	for (i = 0; i < GAME_OBJ_INST_NUM_MAX; i++)
	{
		GameObjectInstance* pInst = sgGameObjectInstanceList + i;

		// skip non-active object
		if ((pInst->mFlag & FLAG_ACTIVE) == 0)
			continue;

		Vector2DAdd(&pInst->mpComponent_Transform->mPosition, &pInst->mpComponent_Transform->mPosition, &pInst->mpComponent_Physics->mVelocity);
	}

	/////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////
	// TO DO 6: Specific game object behavior, according to type
	// -- Bullet: Destroy when it's outside the viewport
	// -- Asteroids: If it's outside the viewport, wrap around viewport.
	// -- Homing missile: If it's outside the viewport, wrap around viewport.
	// -- Homing missile: Follow/Acquire target
	/////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////
	for (i = 0; i < GAME_OBJ_INST_NUM_MAX; i++)
	{
		GameObjectInstance* pInst = sgGameObjectInstanceList + i;

		// skip non-active object
		if ((pInst->mFlag & FLAG_ACTIVE) == 0)
			continue;
		
		// check if the object is a ship
		if (pInst->mpComponent_Sprite->mpShape->mType == OBJECT_TYPE_SHIP)
		{
			// warp the ship from one end of the screen to the other
			pInst->mpComponent_Transform->mPosition.x = AEWrap(pInst->mpComponent_Transform->mPosition.x, winMinX - SHIP_SIZE, winMaxX + SHIP_SIZE);
			pInst->mpComponent_Transform->mPosition.y = AEWrap(pInst->mpComponent_Transform->mPosition.y, winMinY - SHIP_SIZE, winMaxY + SHIP_SIZE);
		}

		// Bullet behavior
		if (pInst->mpComponent_Sprite->mpShape->mType == OBJECT_TYPE_BULLET) {

		}

		// Asteroid behavior
		if (pInst->mpComponent_Sprite->mpShape->mType == OBJECT_TYPE_ASTEROID_SQUARE || pInst->mpComponent_Sprite->mpShape->mType == OBJECT_TYPE_ASTEROID_CIRCLE) {
			// warp the asteroid from one end of the screen to the other
			pInst->mpComponent_Transform->mPosition.x = AEWrap(pInst->mpComponent_Transform->mPosition.x, winMinX - SHIP_SIZE, winMaxX + SHIP_SIZE);
			pInst->mpComponent_Transform->mPosition.y = AEWrap(pInst->mpComponent_Transform->mPosition.y, winMinY - SHIP_SIZE, winMaxY + SHIP_SIZE);
		}

		// Homing missile behavior (Not every game object instance will have this component!)
	}


	/////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////
	// TO DO 8: Check for collision
	// Important: Take the object instance's scale values into consideration when checking for collision.
	// -- Asteroid - Bullet: Rectangle to Point check. If they collide, destroy both.
	// -- Asteroid - Ship: Rectangle to Rectangle check. If they collide, destroy the asteroid, 
	//    reset the ship position to the center of the screen.
	// -- Asteroid - Homing Missile: Rectangle to Rectangle check.
	/////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////
	/*
	for each object instance: oi1
		if oi1 is not active
			skip

		if oi1 is an asteroid
			for each object instance oi2
				if(oi2 is not active or oi2 is an asteroid)
					skip

				if(oi2 is the ship)
					Check for collision between the ship and the asteroid
					Update game behavior accordingly
					Update "Object instances array"
				else
				if(oi2 is a bullet)
					Check for collision between the bullet and the asteroid
					Update game behavior accordingly
					Update "Object instances array"
				else
				if(oi2 is a missle)
					Check for collision between the missile and the asteroid
					Update game behavior accordingly
					Update "Object instances array"
	*/


	// =====================================
	// calculate the matrix for all objects
	// =====================================

	for (i = 0; i < GAME_OBJ_INST_NUM_MAX; i++)
	{
		Matrix2D		 trans, rot, scale;
		GameObjectInstance* pInst = sgGameObjectInstanceList + i;
		
		// skip non-active object
		if ((pInst->mFlag & FLAG_ACTIVE) == 0)
			continue;


		/////////////////////////////////////////////////////////////////////////////////////////////////
		/////////////////////////////////////////////////////////////////////////////////////////////////
		// TO DO 1:
		// -- Build the transformation matrix of each active game object instance
		// -- After you implement this step, you should see the player's ship
		// -- Reminder: Scale should be applied first, then rotation, then translation.
		/////////////////////////////////////////////////////////////////////////////////////////////////
		/////////////////////////////////////////////////////////////////////////////////////////////////

		// Compute the scaling matrix
		Matrix2DScale(&scale, pInst->mpComponent_Transform->mScaleX, pInst->mpComponent_Transform->mScaleY);

		// Compute the rotation matrix 
		Matrix2DRotDeg(&rot, pInst->mpComponent_Transform->mAngle);

		// Compute the translation matrix
		Matrix2DTranslate(&trans, pInst->mpComponent_Transform->mPosition.x, pInst->mpComponent_Transform->mPosition.y);

		// Concatenate the 3 matrix in the correct order in the object instance's transform component's "mTransform" matrix
		Matrix2DConcat(&trans, &trans, &rot);
		Matrix2DConcat(&pInst->mpComponent_Transform->mTransform, &trans, &scale);
	}
}

// ---------------------------------------------------------------------------

void GameStateAsteroidsDraw(void)
{
	unsigned long i;
	double frameTime;


	AEGfxSetRenderMode(AE_GFX_RM_COLOR);
	AEGfxTextureSet(NULL, 0, 0);
	AEGfxSetTintColor(1.0f, 1.0f, 1.0f, 1.0f);

	// draw all object instances in the list

	for (i = 0; i < GAME_OBJ_INST_NUM_MAX; i++)
	{
		GameObjectInstance* pInst = sgGameObjectInstanceList + i;

		// skip non-active object
		if ((pInst->mFlag & FLAG_ACTIVE) == 0)
			continue;
		
		// Already implemented. Explanation:
		// Step 1 & 2 are done outside the for loop (AEGfxSetRenderMode, AEGfxTextureSet, AEGfxSetTintColor) since all our objects share the same material.
		// If you want to have objects with difference materials (Some with textures, some without, some with transparency etc...)
		// then you'll need to move those functions calls inside the for loop
		// 1 - Set Render Mode (Color or texture)
		// 2 - Set all needed parameters (Color blend, textures, etc..)
		// 3 - Set the current object instance's mTransform matrix using "AEGfxSetTransform"
		// 4 - Draw the shape used by the current object instance using "AEGfxMeshDraw"

		AEGfxSetTransform(pInst->mpComponent_Transform->mTransform.m);
		AEGfxMeshDraw(pInst->mpComponent_Sprite->mpShape->mpMesh, AE_GFX_MDM_TRIANGLES);
	}
}

// ---------------------------------------------------------------------------

void GameStateAsteroidsFree(void)
{
	/////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////
	// TO DO 11:
	//  -- Destroy all the active game object instances, using the “GameObjInstDestroy” function.
	//  -- Reset the number of active game objects instances
	/////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////
	int i;

	for (i = 0; i < GAME_OBJ_INST_NUM_MAX; ++i) {
		GameObjectInstance* pInst = sgGameObjectInstanceList + i;
		GameObjectInstanceDestroy(pInst);
	}
}

// ---------------------------------------------------------------------------

void GameStateAsteroidsUnload(void)
{
	/////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////
	// TO DO 12:
	//  -- Destroy all the shapes, using the “AEGfxMeshFree” function.
	/////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////

	int i;

	for (i = 0; i < SHAPE_NUM_MAX; ++i) {
		Shape* pShape = sgShapes + i;
		AEGfxMeshFree(pShape->mpMesh);
	}
}

// ---------------------------------------------------------------------------

GameObjectInstance* GameObjectInstanceCreate(unsigned int ObjectType)			// From OBJECT_TYPE enum)
{
	unsigned long i;
	
	// loop through the object instance list to find a non-used object instance
	for (i = 0; i < GAME_OBJ_INST_NUM_MAX; i++)
	{
		GameObjectInstance* pInst = sgGameObjectInstanceList + i;

		// Check if current instance is not used
		if (pInst->mFlag == 0)
		{
			// It is not used => use it to create the new instance

			// Active the game object instance
			pInst->mFlag = FLAG_ACTIVE;

			pInst->mpComponent_Transform = 0;
			pInst->mpComponent_Sprite    = 0;
			pInst->mpComponent_Physics   = 0;
			pInst->mpComponent_Target    = 0;

			// Add the components, based on the object type
			switch (ObjectType)
			{
			case OBJECT_TYPE_SHIP:
				AddComponent_Sprite(pInst, OBJECT_TYPE_SHIP);
				AddComponent_Transform(pInst, 0, 0.0f, 1.0f, 1.0f);
				AddComponent_Physics(pInst, 0);
				break;

			case OBJECT_TYPE_BULLET:
				AddComponent_Sprite(pInst, OBJECT_TYPE_BULLET);
				AddComponent_Transform(pInst, 0, 0.0f, 1.0f, 1.0f);
				AddComponent_Physics(pInst, 0);
				break;

			case OBJECT_TYPE_ASTEROID_SQUARE:
				AddComponent_Sprite(pInst, OBJECT_TYPE_ASTEROID_SQUARE);
				AddComponent_Transform(pInst, 0, 0.0f, 1.0f, 1.0f);
				AddComponent_Physics(pInst, 0);
				break;

			case OBJECT_TYPE_ASTEROID_CIRCLE:
				AddComponent_Sprite(pInst, OBJECT_TYPE_ASTEROID_CIRCLE);
				AddComponent_Transform(pInst, 0, 0.0f, 1.0f, 1.0f);
				AddComponent_Physics(pInst, 0);
				break;

			case OBJECT_TYPE_HOMING_MISSILE:
				AddComponent_Sprite(pInst, OBJECT_TYPE_HOMING_MISSILE);
				AddComponent_Transform(pInst, 0, 0.0f, 1.0f, 1.0f);
				AddComponent_Physics(pInst, 0);
				AddComponent_Target(pInst, 0);
				break;
			}

			++sgGameObjectInstanceNum;

			// return the newly created instance
			return pInst;
		}
	}

	// Cannot find empty slot => return 0
	return 0;
}

// ---------------------------------------------------------------------------

void GameObjectInstanceDestroy(GameObjectInstance* pInst)
{
	// if instance is destroyed before, just return
	if (pInst->mFlag == 0)
		return;

	// Zero out the mFlag
	pInst->mFlag = 0;

	RemoveComponent_Transform(pInst);
	RemoveComponent_Sprite(pInst);
	RemoveComponent_Physics(pInst);
	RemoveComponent_Target(pInst);

	--sgGameObjectInstanceNum;
}

static AEGfxVertexList* make_halfcap_shape(float x, float y, int parts, int color) {
	int i;
	float CircleAngleStep;
	Vector2D center;

	center.x = x - (x / 2.0f);
	center.y = y;

	AEGfxMeshStart();

	/* 1st argument: X */
	/* 2nd argument: Y */
	/* 3rd argument: ARGB */
	AEGfxTriAdd(
		-x, -y, color, 0.0f, 0.0f,
		 x, -y, color, 0.0f, 0.0f,
		-x,  y, color, 0.0f, 0.0f);
	AEGfxTriAdd(
		-x,  y, color, 0.0f, 0.0f,
		 x, -y, color, 0.0f, 0.0f,
		 x,  y, color, 0.0f, 0.0f);

	CircleAngleStep = PI / (parts * 2);
	for (i = 0; i < parts; ++i) {
		AEGfxTriAdd(
			                                center.x,                                     center.y, color, 0.0f, 0.0f,
			      cosf(i * 2 * CircleAngleStep)*0.5f,       sinf(i * 2 * CircleAngleStep)*0.5f + y, color, 0.0f, 0.0f,
			cosf((i + 1) * 2 * CircleAngleStep)*0.5f, sinf((i + 1) * 2 * CircleAngleStep)*0.5f + y, color, 0.0f, 0.0f);
	}

	return AEGfxMeshEnd();
}

static AEGfxVertexList* make_triangle_object(float x, float y, int color) {
	AEGfxMeshStart();

	/* 1st argument: X    */
	/* 2nd argument: Y    */
	/* 3rd argument: ARGB */
	AEGfxTriAdd(
		-x, -y, color, 0.0f, 0.0f,
		 x, -y, color, 0.0f, 0.0f,
		-x,  y, color, 0.0f, 0.0f);

	return AEGfxMeshEnd();
}

static AEGfxVertexList* make_rectangle_object(float x, float y, int color) {
	AEGfxMeshStart();

	/* 1st argument: X */
	/* 2nd argument: Y */
	/* 3rd argument: ARGB */
	AEGfxTriAdd(
		-x, -y, color, 0.0f, 0.0f,
		 x, -y, color, 0.0f, 0.0f,
		-x,  y, color, 0.0f, 0.0f);
	AEGfxTriAdd(
		-x,  y, color, 0.0f, 0.0f,
		 x, -y, color, 0.0f, 0.0f,
		 x,  y, color, 0.0f, 0.0f);

	return AEGfxMeshEnd();
}

static AEGfxVertexList* make_circle_object(int parts, int color) {
	float CircleAngleStep;
	int i;
	AEGfxMeshStart();

	/* 1st argument: X */
	/* 2nd argument: Y */
	/* 3rd argument: ARGB */

	/* Creating the circle shape */
	CircleAngleStep = PI / parts;
	for (i = 0; i < parts; ++i) {
		AEGfxTriAdd( 
			                                    0.0f,                                     0.0f, color, 0.0f, 0.0f,
			      cosf(i * 2 * CircleAngleStep)*0.5f,       sinf(i * 2 * CircleAngleStep)*0.5f, color, 0.0f, 0.0f,
			cosf((i + 1) * 2 * CircleAngleStep)*0.5f, sinf((i + 1) * 2 * CircleAngleStep)*0.5f, color, 0.0f, 0.0f);
	}

	return AEGfxMeshEnd();
}

// ---------------------------------------------------------------------------

void AddComponent_Transform(GameObjectInstance *pInst, Vector2D *pPosition, float Angle, float ScaleX, float ScaleY)
{
	if (0 != pInst)
	{
		if (0 == pInst->mpComponent_Transform)
		{
			pInst->mpComponent_Transform = (Component_Transform *)calloc(1, sizeof(Component_Transform));
		}

		Vector2D zeroVec2;
		Vector2DZero(&zeroVec2);

		pInst->mpComponent_Transform->mScaleX   = ScaleX;
		pInst->mpComponent_Transform->mScaleY   = ScaleY;
		pInst->mpComponent_Transform->mPosition = pPosition ? *pPosition : zeroVec2;;
		pInst->mpComponent_Transform->mAngle    = Angle;
		pInst->mpComponent_Transform->mpOwner   = pInst;
	}
}

// ---------------------------------------------------------------------------

void AddComponent_Sprite(GameObjectInstance *pInst, unsigned int ShapeType)
{
	if (0 != pInst)
	{
		if (0 == pInst->mpComponent_Sprite)
		{
			pInst->mpComponent_Sprite = (Component_Sprite *)calloc(1, sizeof(Component_Sprite));
		}
	
		pInst->mpComponent_Sprite->mpShape = sgShapes + ShapeType;
		pInst->mpComponent_Sprite->mpOwner = pInst;
	}
}

// ---------------------------------------------------------------------------

void AddComponent_Physics(GameObjectInstance *pInst, Vector2D *pVelocity)
{
	if (0 != pInst)
	{
		if (0 == pInst->mpComponent_Physics)
		{
			pInst->mpComponent_Physics = (Component_Physics *)calloc(1, sizeof(Component_Physics));
		}

		Vector2D zeroVec2;
		Vector2DZero(&zeroVec2);

		pInst->mpComponent_Physics->mVelocity = pVelocity ? *pVelocity : zeroVec2;
		pInst->mpComponent_Physics->mpOwner = pInst;
	}
}

// ---------------------------------------------------------------------------

void AddComponent_Target(GameObjectInstance *pInst, GameObjectInstance *pTarget)
{
	if (0 != pInst)
	{
		if (0 == pInst->mpComponent_Target)
		{
			pInst->mpComponent_Target = (Component_Target *)calloc(1, sizeof(Component_Target));
		}

		pInst->mpComponent_Target->mpTarget = pTarget;
		pInst->mpComponent_Target->mpOwner = pInst;
	}
}

// ---------------------------------------------------------------------------

void RemoveComponent_Transform(GameObjectInstance *pInst)
{
	if (0 != pInst)
	{
		if (0 != pInst->mpComponent_Transform)
		{
			free(pInst->mpComponent_Transform);
			pInst->mpComponent_Transform = 0;
		}
	}
}

// ---------------------------------------------------------------------------

void RemoveComponent_Sprite(GameObjectInstance *pInst)
{
	if (0 != pInst)
	{
		if (0 != pInst->mpComponent_Sprite)
		{
			free(pInst->mpComponent_Sprite);
			pInst->mpComponent_Sprite = 0;
		}
	}
}

// ---------------------------------------------------------------------------

void RemoveComponent_Physics(GameObjectInstance *pInst)
{
	if (0 != pInst)
	{
		if (0 != pInst->mpComponent_Physics)
		{
			free(pInst->mpComponent_Physics);
			pInst->mpComponent_Physics = 0;
		}
	}
}

// ---------------------------------------------------------------------------

void RemoveComponent_Target(GameObjectInstance *pInst)
{
	if (0 != pInst)
	{
		if (0 != pInst->mpComponent_Target)
		{
			free(pInst->mpComponent_Target);
			pInst->mpComponent_Target = 0;
		}
	}
}