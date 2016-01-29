#include "Objects.h"

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
				break;

			case OBJECT_TYPE_BULLET:
				AddComponent_Sprite(pInst, OBJECT_TYPE_BULLET);
				break;

			case OBJECT_TYPE_ASTEROID:
				AddComponent_Sprite(pInst, OBJECT_TYPE_ASTEROID);
				break;

			case OBJECT_TYPE_STAR_1:
				AddComponent_Sprite(pInst, OBJECT_TYPE_STAR_1);
				break;

			case OBJECT_TYPE_STAR_2:
				AddComponent_Sprite(pInst, OBJECT_TYPE_STAR_2);
				break;

			case OBJECT_TYPE_STAR_3:
				AddComponent_Sprite(pInst, OBJECT_TYPE_STAR_3);
				break;

			case OBJECT_TYPE_PLANET:
				AddComponent_Sprite(pInst, OBJECT_TYPE_PLANET);
				break;

			case OBJECT_TYPE_HOMING_MISSILE:
				AddComponent_Sprite(pInst, OBJECT_TYPE_HOMING_MISSILE);
				AddComponent_Target(pInst, 0);
				break;
			}
			AddComponent_Transform(pInst, 0, 0.0f, 1.0f, 1.0f);
			AddComponent_Physics(pInst, 0);

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


AEGfxVertexList* make_triangle_mesh(int color) {
	AEGfxMeshStart();

	/* 1st argument: X    */
	/* 2nd argument: Y    */
	/* 3rd argument: ARGB */
	AEGfxTriAdd(
		-0.5f, -0.5f, color, 0.0f, 0.0f,
		-0.5f,  0.5f, color, 0.0f, 0.0f,
		 0.5f,  0.0f, color, 0.0f, 0.0f);

	return AEGfxMeshEnd();
}

AEGfxVertexList* make_rectangle_mesh(int color) {
	AEGfxMeshStart();

	/* 1st argument: X */
	/* 2nd argument: Y */
	/* 3rd argument: ARGB */
	AEGfxTriAdd(
		-0.5f, -0.5f, color, 0.0f, 1.0f,
		 0.5f, -0.5f, color, 1.0f, 1.0f,
		-0.5f,  0.5f, color, 0.0f, 0.0f);
	AEGfxTriAdd(
		 0.5f, -0.5f, color, 1.0f, 1.0f,
		 0.5f,  0.5f, color, 1.0f, 0.0f,
		-0.5f,  0.5f, color, 0.0f, 0.0f);

	return AEGfxMeshEnd();
}

AEGfxVertexList* make_star_mesh(int color) {
	AEGfxMeshStart();

	AEGfxTriAdd(
		 0.25f,  0.50f, color, 0.0f, 0.0f,
	 	 0.25f, -0.50f, color, 0.0f, 0.0f,
		-0.25f, -0.50f, color, 0.0f, 0.0f);
	AEGfxTriAdd(
		 0.25f,  0.50f, color, 0.0f, 0.0f,
		-0.25f,  0.50f, color, 0.0f, 0.0f,
		-0.25f, -0.50f, color, 0.0f, 0.0f);
	AEGfxTriAdd(
		 0.50f,  0.25f, color, 0.0f, 0.0f,
		 0.25f,  0.25f, color, 0.0f, 0.0f,
		 0.25f, -0.25f, color, 0.0f, 0.0f);
	AEGfxTriAdd(
		 0.50f,  0.25f, color, 0.0f, 0.0f,
		 0.50f, -0.25f, color, 0.0f, 0.0f,
		 0.25f, -0.25f, color, 0.0f, 0.0f);
	AEGfxTriAdd(
		-0.50f,  0.25f, color, 0.0f, 0.0f,
		-0.25f,  0.25f, color, 0.0f, 0.0f,
		-0.25f, -0.25f, color, 0.0f, 0.0f);
	AEGfxTriAdd(
		-0.50f,  0.25f, color, 0.0f, 0.0f,
		-0.50f, -0.25f, color, 0.0f, 0.0f,
		-0.25f, -0.25f, color, 0.0f, 0.0f);

	return AEGfxMeshEnd();
}

AEGfxVertexList* make_circle_mesh(int parts, int color) {
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
			0.0f, 0.0f, color, 0.0f, 0.0f,
			cosf(i * 2 * CircleAngleStep)*0.5f, sinf(i * 2 * CircleAngleStep)*0.5f, color, 0.0f, 0.0f,
			cosf((i + 1) * 2 * CircleAngleStep)*0.5f, sinf((i + 1) * 2 * CircleAngleStep)*0.5f, color, 0.0f, 0.0f);
	}

	return AEGfxMeshEnd();
}

AEGfxVertexList* make_halfcap_mesh(int parts, int color) {
	int i;
	float CircleAngleStep;

	AEGfxMeshStart();

	/* 1st argument: X */
	/* 2nd argument: Y */
	/* 3rd argument: ARGB */
	AEGfxTriAdd(
		-0.5f, -0.5f, color, 0.0f, 0.0f,
		0.5f, -0.5f, color, 0.0f, 0.0f,
		-0.5f, 0.5f, color, 0.0f, 0.0f);
	AEGfxTriAdd(
		-0.5f, 0.5f, color, 0.0f, 0.0f,
		0.5f, -0.5f, color, 0.0f, 0.0f,
		0.5f, 0.5f, color, 0.0f, 0.0f);

	CircleAngleStep = PI / (parts * 2);
	for (i = 0; i < parts; ++i) {
		AEGfxTriAdd(
			0.5f, 0, color, 0.0f, 0.0f,
			cosf(i * 2 * CircleAngleStep)*0.5f + 0.5f, sinf(i * 2 * CircleAngleStep)*0.5f, color, 0.0f, 0.0f,
			cosf((i + 1) * 2 * CircleAngleStep)*0.5f + 0.5f, sinf((i + 1) * 2 * CircleAngleStep)*0.5f, color, 0.0f, 0.0f);
	}

	return AEGfxMeshEnd();
}