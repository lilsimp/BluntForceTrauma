#include "Objects.h"

#define FLAG_ACTIVE      0x00000001

GameObjectInstance* GameObjectInstanceCreate(GameObjectInstance* GameObjInstList, Shape* Shapes, int* ObjectAmount, unsigned int ObjectType) {
	unsigned long i;

	/* loop through the object instance list to find a non-used object instance */
	for (i = 0; i < GAME_OBJ_INST_NUM_MAX; ++i)	{
		GameObjectInstance* pInst = GameObjInstList + i;

		/* Check if current instance is not used */
		if (!pInst->mFlag) {
			/* It is not used => use it to create the new instance */

			/* Active the game object instance */
			pInst->mFlag = FLAG_ACTIVE;

			pInst->mpComponent_Transform = 0;
			pInst->mpComponent_Sprite = 0;
			pInst->mpComponent_Physics = 0;
			pInst->mpComponent_Target = 0;

			/* Add the components, based on the object type */
			switch (ObjectType) {
			case OBJECT_TYPE_PLAYER:
				AddComponent_Sprite(pInst, Shapes, OBJECT_TYPE_PLAYER);
				break;

			case OBJECT_TYPE_ENEMY1:
				AddComponent_Sprite(pInst, Shapes, OBJECT_TYPE_ENEMY1);
				break;

			case OBJECT_TYPE_PUNCH:
				AddComponent_Sprite(pInst, Shapes, OBJECT_TYPE_PUNCH);
				break;

			case OBJECT_TYPE_ORB:
				AddComponent_Sprite(pInst, Shapes, OBJECT_TYPE_ORB);
				break;

			case OBJECT_TYPE_COIN:
				AddComponent_Sprite(pInst, Shapes, OBJECT_TYPE_COIN);
				break;
			}
			AddComponent_Transform(pInst, 0, 0.0f, 1.0f, 1.0f);
			AddComponent_Physics(pInst, 0);

			++(*ObjectAmount);

			/* return the newly created instance */
			return pInst;
		}
	}

	/* Cannot find empty slot => return 0 */
	return 0;
}

/* --------------------------------------------------------------------------- */

void GameObjectInstanceDestroy(GameObjectInstance* pInst, int* ObjectAmount) {
	/* if instance is destroyed before, just return */
	if (pInst->mFlag == 0)
		return;

	/* Zero out the mFlag */
	pInst->mFlag = 0;

	RemoveComponent_Transform(pInst);
	RemoveComponent_Sprite(pInst);
	RemoveComponent_Physics(pInst);
	RemoveComponent_Target(pInst);

	--(*ObjectAmount);
}

/* --------------------------------------------------------------------------- */

void AddComponent_Transform(GameObjectInstance *pInst, Vector2D *pPosition, float Angle, float ScaleX, float ScaleY) {
	if (pInst) {
		if (!pInst->mpComponent_Transform)
			pInst->mpComponent_Transform = (Component_Transform *)calloc(1, sizeof(Component_Transform));

		Vector2D zeroVec2;
		Vector2DZero(&zeroVec2);

		pInst->mpComponent_Transform->mScaleX = ScaleX;
		pInst->mpComponent_Transform->mScaleY = ScaleY;
		pInst->mpComponent_Transform->mPosition = pPosition ? *pPosition : zeroVec2;;
		pInst->mpComponent_Transform->mAngle = Angle;
		pInst->mpComponent_Transform->mpOwner = pInst;
	}
}

/* --------------------------------------------------------------------------- */

void AddComponent_Sprite(GameObjectInstance *pInst, Shape* Shapes, unsigned int ShapeType) {
	if (pInst) {
		if (!pInst->mpComponent_Sprite)
			pInst->mpComponent_Sprite = (Component_Sprite *)calloc(1, sizeof(Component_Sprite));

		pInst->mpComponent_Sprite->mpShape = Shapes + ShapeType;
		pInst->mpComponent_Sprite->mpOwner = pInst;
	}
}

/* --------------------------------------------------------------------------- */

void AddComponent_Physics(GameObjectInstance *pInst, Vector2D *pVelocity) {
	if (pInst)	{
		if (!pInst->mpComponent_Physics)
			pInst->mpComponent_Physics = (Component_Physics *)calloc(1, sizeof(Component_Physics));

		Vector2D zeroVec2;
		Vector2DZero(&zeroVec2);

		pInst->mpComponent_Physics->mVelocity = pVelocity ? *pVelocity : zeroVec2;
		pInst->mpComponent_Physics->mpOwner = pInst;
	}
}

/* --------------------------------------------------------------------------- */

void AddComponent_Target(GameObjectInstance *pInst, GameObjectInstance *pTarget) {
	if (pInst) {
		if (!pInst->mpComponent_Target)
			pInst->mpComponent_Target = (Component_Target *)calloc(1, sizeof(Component_Target));

		pInst->mpComponent_Target->mpTarget = pTarget;
		pInst->mpComponent_Target->mpOwner = pInst;
	}
}

/* --------------------------------------------------------------------------- */

void RemoveComponent_Transform(GameObjectInstance *pInst) {
	if (pInst) {
		if (pInst->mpComponent_Transform) {
			free(pInst->mpComponent_Transform);
			pInst->mpComponent_Transform = 0;
		}
	}
}

/* --------------------------------------------------------------------------- */

void RemoveComponent_Sprite(GameObjectInstance *pInst) {
	if (pInst) {
		if (pInst->mpComponent_Sprite) {
			free(pInst->mpComponent_Sprite);
			pInst->mpComponent_Sprite = 0;
		}
	}
}

/* --------------------------------------------------------------------------- */

void RemoveComponent_Physics(GameObjectInstance *pInst) {
	if (pInst) {
		if (pInst->mpComponent_Physics) {
			free(pInst->mpComponent_Physics);
			pInst->mpComponent_Physics = 0;
		}
	}
}

/* --------------------------------------------------------------------------- */

void RemoveComponent_Target(GameObjectInstance *pInst) {
	if (pInst) {
		if (pInst->mpComponent_Target) {
			free(pInst->mpComponent_Target);
			pInst->mpComponent_Target = 0;
		}
	}
}

/* --------------------------------------------------------------------------- */

AEGfxVertexList* make_triangle_mesh(float x, float y, int color) {
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

/* --------------------------------------------------------------------------- */

AEGfxVertexList* make_rectangle_mesh(float x, float y, int color) {
	AEGfxMeshStart();

	/* 1st argument: X */
	/* 2nd argument: Y */
	/* 3rd argument: ARGB */
	AEGfxTriAdd(
		-x, -y, color, 0.0f, 0.0f,
		 x, -y, color, 0.0f, 0.0f,
		-x,  y, color, 0.0f, 0.0f);

	AEGfxTriAdd(
		-x,   y, color, 0.0f, 0.0f,
		 x,  -y, color, 0.0f, 0.0f,
		 x,   y, color, 0.0f, 0.0f);

	return AEGfxMeshEnd();
}

/* --------------------------------------------------------------------------- */

AEGfxVertexList* make_circle_mesh(int parts, int color) {
	float CircleAngleStep;
	int i;
	AEGfxMeshStart();

	/* 1st argument: X */
	/* 2nd argument: Y */
	/* 3rd argument: ARGB */

	/* Creating the circle shape */
	CircleAngleStep = PI/12.0f;
	for(i = 0; i < parts; ++i) {
		AEGfxTriAdd(
		                       0.0f,                         0.0f, color, 0.0f, 0.0f,
		    cosf(i*2*PI/parts)*0.5f,      sinf(i*2*PI/parts)*0.5f, color, 0.0f, 0.0f,
		cosf((i+1)*2*PI/parts)*0.5f,  sinf((i+1)*2*PI/parts)*0.5f, color, 0.0f, 0.0f);
	}

	return AEGfxMeshEnd();
}

/* --------------------------------------------------------------------------- */

AEGfxVertexList* make_halfcap_mesh(float x, float y, int parts, int color) {
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
		-x, y, color, 0.0f, 0.0f);
	AEGfxTriAdd(
		-x, y, color, 0.0f, 0.0f,
		x, -y, color, 0.0f, 0.0f,
		x, y, color, 0.0f, 0.0f);

	CircleAngleStep = PI / (parts * 2.0f);
	for (i = 0; i < parts; ++i) {
		AEGfxTriAdd(
			center.x, center.y, color, 0.0f, 0.0f,
			cosf(i * 2 * CircleAngleStep)*0.5f, sinf(i * 2 * CircleAngleStep)*0.5f + y, color, 0.0f, 0.0f,
			cosf((i + 1) * 2 * CircleAngleStep)*0.5f, sinf((i + 1) * 2 * CircleAngleStep)*0.5f + y, color, 0.0f, 0.0f);
	}

	return AEGfxMeshEnd();
}
