// ---------------------------------------------------------------------------
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
#include "Objects.h"

// ---------------------------------------------------------------------------

// Feel free to change these values in ordet to make the game more fun
#define SHIP_INITIAL_NUM			3					// Initial number of ship lives
#define SHIP_SIZE					15.0f				// Ship size
#define SHIP_ACCEL_FORWARD			100.0f				// Ship forward acceleration (in m/s^2)
#define SHIP_ACCEL_BACKWARD			-150.0f				// Ship backward acceleration (in m/s^2)
#define SHIP_ROT_SPEED				(PI)     			// Ship rotation speed (radian/second)
#define HOMING_MISSILE_ROT_SPEED	(PI / 2.0f)			// Homing missile rotation speed (radian/second)
#define BULLET_SPEED				500.0f				// Bullet speed (m/s)
#define MAX_SPEED                   200.0f
#define DRAG_SPEED                  0.99f
// ---------------------------------------------------------------------------

extern unsigned int GSM_NEXT;

// List of original vertex buffers

// pointer ot the ship object
static GameObjectInstance*			sgpShip;											// Pointer to the "Ship" game object instance

																						// number of ship available (lives 0 = game over)
static long					sgShipLives;												// The number of lives left

																						// the score = number of asteroid destroyed
static unsigned long		sgScore;													// Current score

																						// --------------------------------------------------------------------------

static Vector2D get_ship_direction();
static void update_ship_velocity();

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
	pShape->mType  = OBJECT_TYPE_SHIP;
	pShape->mpMesh = make_triangle_mesh(0.5f, 0.5f, 0xFFFF0000);


	/////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////
	// TO DO 4:
	// -- Create the bullet shape
	/////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////

	pShape = sgShapes + sgShapeNum++;
	pShape->mType = OBJECT_TYPE_BULLET;
	pShape->mpMesh = make_rectangle_mesh(0.5f, 0.5f, 0xFFD1CE69);

	/////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////
	// TO DO 6:
	// -- Create the asteroid shape
	/////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////

	pShape = sgShapes + sgShapeNum++;
	pShape->mType = OBJECT_TYPE_ASTEROID_SQUARE;
	pShape->mpMesh = make_rectangle_mesh(0.5f, 0.5f, 0xFF663333);

	/////////////////////////////////////////////////////////////////////////////////////////////////
	// -- Create the asteroid shape circle
	/////////////////////////////////////////////////////////////////////////////////////////////////

	pShape = sgShapes + sgShapeNum++;
	pShape->mType = OBJECT_TYPE_ASTEROID_SQUARE;
	pShape->mpMesh = make_circle_mesh(12, 0xFF663333);

	/////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////
	// TO DO 9:
	// -- Create the homing missile shape
	/////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////
	
	pShape = sgShapes + sgShapeNum++;
	pShape->mType = OBJECT_TYPE_HOMING_MISSILE;
	pShape->mpMesh = make_halfcap_mesh(0.5f, 0.5f, 12, 0xFFFF00FF);

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
	AddComponent_Transform(sgpShip, &sgpShip->mpComponent_Transform->mPosition, sgpShip->mpComponent_Transform->mAngle, 40.0f, 30.0f);

	/////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////
	// TO DO 7:
	// -- Create at least 3 asteroid instances, each with a different size, 
	//    using the "GameObjInstCreate" function
	// -- Make sure to add their components!
	/////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////

	for (i = 0; i < 20; ++i) {
		GameObjectInstance* pAsteroid;
		float scale = rand() % 20 + 30;
		Vector2D velocity = { rand() % 30 - 15, rand() % 30 - 15};
		Vector2D spnPoint = { (rand() % (X_MAX * 2)) - X_MAX, (rand() % (Y_MAX * 2)) - Y_MAX };

		pAsteroid = GameObjectInstanceCreate((i % 2) ? OBJECT_TYPE_ASTEROID_SQUARE : OBJECT_TYPE_ASTEROID_CIRCLE);
		AddComponent_Transform(pAsteroid, &spnPoint, sgpShip->mpComponent_Transform->mAngle, scale, scale);
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
	if (AEInputCheckCurr(VK_UP) || AEInputCheckCurr('W'))
	{
		Vector2D direction = get_ship_direction();

		Vector2DScale(&direction, &direction, SHIP_ACCEL_FORWARD * frameTime);

		Vector2DAdd(&sgpShip->mpComponent_Physics->mVelocity, &sgpShip->mpComponent_Physics->mVelocity, &direction);

		update_ship_velocity();
	}

	if (AEInputCheckCurr(VK_DOWN) || AEInputCheckCurr('S'))
	{
		Vector2D direction = get_ship_direction();

		Vector2DScale(&direction, &direction, SHIP_ACCEL_BACKWARD * frameTime);

		Vector2DAdd(&sgpShip->mpComponent_Physics->mVelocity, &sgpShip->mpComponent_Physics->mVelocity, &direction);

		update_ship_velocity();
	}

	if (AEInputCheckCurr(VK_LEFT) || AEInputCheckCurr('A'))
	{
		sgpShip->mpComponent_Transform->mAngle += SHIP_ROT_SPEED * (float)(frameTime);
		sgpShip->mpComponent_Transform->mAngle  = AEWrap(sgpShip->mpComponent_Transform->mAngle, -PI, PI);
	}

	if (AEInputCheckCurr(VK_RIGHT) || AEInputCheckCurr('D'))
	{
		sgpShip->mpComponent_Transform->mAngle -= SHIP_ROT_SPEED * (float)(frameTime);
		sgpShip->mpComponent_Transform->mAngle  = AEWrap(sgpShip->mpComponent_Transform->mAngle, -PI, PI);
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
		Vector2D velocity = get_ship_direction();
		
		Vector2DScaleAdd(&velocity, &velocity, &sgpShip->mpComponent_Physics->mVelocity, BULLET_SPEED);

		bullet = GameObjectInstanceCreate(OBJECT_TYPE_BULLET);
		AddComponent_Transform(bullet, &sgpShip->mpComponent_Transform->mPosition, sgpShip->mpComponent_Transform->mAngle, 10.0f, 5.0f);
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
		GameObjectInstance* bullet;
		Vector2D velocity = get_ship_direction();

		Vector2DScaleAdd(&velocity, &velocity, &sgpShip->mpComponent_Physics->mVelocity, BULLET_SPEED);

		bullet = GameObjectInstanceCreate(OBJECT_TYPE_HOMING_MISSILE);
		AddComponent_Transform(bullet, &sgpShip->mpComponent_Transform->mPosition, sgpShip->mpComponent_Transform->mAngle, 15.0f, 8.0f);
		AddComponent_Physics(bullet, &velocity);
	}

	if (AEInputCheckTriggered('R'))
	{
		GSM_NEXT = RESTART;
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
		Vector2D frameVelocity;
		GameObjectInstance* pInst = sgGameObjectInstanceList + i;

		// skip non-active object
		if ((pInst->mFlag & FLAG_ACTIVE) == 0)
			continue;

		Vector2DScale(&frameVelocity, &pInst->mpComponent_Physics->mVelocity, frameTime);
		Vector2DAdd(&pInst->mpComponent_Transform->mPosition, &frameVelocity, &pInst->mpComponent_Transform->mPosition);
	}

	// Apply drag to the ship
	Vector2DScale(&sgpShip->mpComponent_Physics->mVelocity, &sgpShip->mpComponent_Physics->mVelocity, DRAG_SPEED);

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
		else if (pInst->mpComponent_Sprite->mpShape->mType == OBJECT_TYPE_BULLET) {
			if (pInst->mpComponent_Transform->mPosition.x > X_MAX / 2 || pInst->mpComponent_Transform->mPosition.x < -X_MAX / 2 || pInst->mpComponent_Transform->mPosition.y > Y_MAX / 2 || pInst->mpComponent_Transform->mPosition.y < -Y_MAX / 2)
				GameObjectInstanceDestroy(pInst);

		}

		// Asteroid behavior
		else if (pInst->mpComponent_Sprite->mpShape->mType == OBJECT_TYPE_ASTEROID_SQUARE || pInst->mpComponent_Sprite->mpShape->mType == OBJECT_TYPE_ASTEROID_CIRCLE) {
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
	for (i = 0; i < GAME_OBJ_INST_NUM_MAX; i++)
	{
		unsigned int j;
		GameObjectInstance* pAsteroid = sgGameObjectInstanceList + i;

		// skip non-active object
		if ((pAsteroid->mFlag & FLAG_ACTIVE) == 0)
			continue;

		if (pAsteroid->mpComponent_Sprite->mpShape->mType == OBJECT_TYPE_ASTEROID_SQUARE || pAsteroid->mpComponent_Sprite->mpShape->mType == OBJECT_TYPE_ASTEROID_CIRCLE) {
			for (j = 0; j < GAME_OBJ_INST_NUM_MAX; ++j) {
				GameObjectInstance* pInst = sgGameObjectInstanceList + j;


				if ((pInst->mFlag & FLAG_ACTIVE) == 0 || pInst->mpComponent_Sprite->mpShape->mType == OBJECT_TYPE_ASTEROID_SQUARE || pInst->mpComponent_Sprite->mpShape->mType == OBJECT_TYPE_ASTEROID_CIRCLE)
					continue;

				if (pInst->mpComponent_Sprite->mpShape->mType == OBJECT_TYPE_SHIP) {

				}
				else if (pInst->mpComponent_Sprite->mpShape->mType == OBJECT_TYPE_BULLET || pInst->mpComponent_Sprite->mpShape->mType == OBJECT_TYPE_HOMING_MISSILE) {

				}
			}
		}

	}

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
		Matrix2DRotRad(&rot, pInst->mpComponent_Transform->mAngle);

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

static Vector2D get_ship_direction() {
	Vector2D direction;
	Vector2DSet(&direction, cosf(sgpShip->mpComponent_Transform->mAngle), sinf(sgpShip->mpComponent_Transform->mAngle));
	Vector2DNormalize(&direction, &direction);
	return direction;
}

static void update_ship_velocity() {
	float current_speed = Vector2DSquareLength(&sgpShip->mpComponent_Physics->mVelocity);

	if (current_speed > (MAX_SPEED * MAX_SPEED)) {
		Vector2D temp;

		Vector2DNormalize(&temp, &sgpShip->mpComponent_Physics->mVelocity);
		Vector2DScale(&sgpShip->mpComponent_Physics->mVelocity, &temp, MAX_SPEED);
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
			pInst->mpComponent_Sprite = 0;
			pInst->mpComponent_Physics = 0;
			pInst->mpComponent_Target = 0;

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
				break;

			case OBJECT_TYPE_ASTEROID_SQUARE:
				AddComponent_Sprite(pInst, OBJECT_TYPE_ASTEROID_SQUARE);
				break;

			case OBJECT_TYPE_ASTEROID_CIRCLE:
				AddComponent_Sprite(pInst, OBJECT_TYPE_ASTEROID_CIRCLE);
				break;

			case OBJECT_TYPE_HOMING_MISSILE:
				AddComponent_Sprite(pInst, OBJECT_TYPE_HOMING_MISSILE);
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

		pInst->mpComponent_Transform->mScaleX = ScaleX;
		pInst->mpComponent_Transform->mScaleY = ScaleY;
		pInst->mpComponent_Transform->mPosition = pPosition ? *pPosition : zeroVec2;;
		pInst->mpComponent_Transform->mAngle = Angle;
		pInst->mpComponent_Transform->mpOwner = pInst;
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
