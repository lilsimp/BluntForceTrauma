#include "Objects.h"

#define FLAG_ACTIVE      0x00000001

GameObj* AllGameObjects(GameObj* gmObj, int numObjects) {
	gmObj = (GameObj *)malloc(numObjects * sizeof(GameObj));
	return gmObj;
}
GameObjInst* AllGameObjectInsts(GameObjInst* gmObjInst, int numInsts) {
	gmObjInst = (GameObj *)malloc(numInsts * sizeof(GameObj));
	return gmObjInst;
}

GameObjInst* gameObjInstCreate(GameObjInst* sGameObjInstList, GameObj* sGameObjList, unsigned int type, float scale, Vector2D* pPos, Vector2D* pVel, float dir, enum STATE startState) {
	unsigned int i;
	Vector2D zero = { 0.0f, 0.0f };

	/* loop through the object instance list to find a non-used object instance */
	for (i = 0; i < GAME_OBJ_INST_NUM_MAX; ++i) {
		GameObjInst* pInst = sGameObjInstList + i;

		/* check if current instance is not used */
		if (pInst->flag == 0)	{
			/* it is not used => use it to create the new instance */
			pInst->pObject	 = sGameObjList + type;
			pInst->flag		 = FLAG_ACTIVE;
			pInst->scale	 = scale;
			pInst->posCurr	 = pPos ? *pPos : zero;
			pInst->velCurr	 = pVel ? *pVel : zero;
			pInst->dirCurr	 = dir;
			pInst->pUserData = 0;
			pInst->gridCollisionFlag = 0;
			pInst->state = startState;
			pInst->innerState = INNER_STATE_ON_ENTER;
			pInst->counter = 0;

			/* return the newly created instance */
			return pInst;
		}
	}
}

void gameObjInstDestroy(GameObjInst* pInst) {
	/* if instance is destroyed before, just return */
	if (pInst->flag == 0)
		return;

	/* zero out the flag */
	pInst->flag = 0;
}

void make_triangle_object(GameObj* pObj, float x, float y, int color) {
	AEGfxMeshStart();

	/* 1st argument: X    */
	/* 2nd argument: Y    */
	/* 3rd argument: ARGB */
	AEGfxTriAdd(
		-x, -y, color, 0.0f, 0.0f,
		x, -y, color, 0.0f, 0.0f,
		-x, y, color, 0.0f, 0.0f);
	  pObj->pMesh = AEGfxMeshEnd();

	  pObj->pMesh = AEGfxMeshEnd();
}

void make_square_object(GameObj* pObj, float x, float y, int color) {
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

	pObj->pMesh = AEGfxMeshEnd();
}

void make_circle_object(GameObj* pObj, int parts, int color) {
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

	pObj->pMesh = AEGfxMeshEnd();
}
