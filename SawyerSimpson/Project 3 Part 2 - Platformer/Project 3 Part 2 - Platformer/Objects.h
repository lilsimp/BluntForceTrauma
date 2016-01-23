#ifndef OBJECTS_H
#define OBJECTS_H

#include "WorldSettings.h"
#include "AEEngine.h"
#include "Matrix2D.h"

enum TYPE_OBJECT {
	TYPE_OBJECT_EMPTY,		   	/* 0 */
	TYPE_OBJECT_COLLISION,		/* 1 */
	TYPE_OBJECT_PLAYER,		  	/* 2 */
	TYPE_OBJECT_ENEMY1,		  	/* 3 */
	TYPE_OBJECT_COIN,			    /* 4 */
	TYPE_OBJECT_BOOSTER,      /* 5 */
	TYPE_OBJECT_PLAYER_BITS   /*6 */
};

/* State machine states */
enum STATE {
	STATE_NONE,
	STATE_GOING_LEFT,
	STATE_GOING_RIGHT
};

/* State machine inner states */
enum INNER_STATE {
	INNER_STATE_ON_ENTER,
	INNER_STATE_ON_UPDATE,
	INNER_STATE_ON_EXIT
};

typedef struct GameObj {
	unsigned int		type;		/*  object type */
	AEGfxVertexList*	pMesh;		/*  pbject */
}GameObj;

typedef struct GameObjInst {
	GameObj*		pObject;	/*  pointer to the 'original' */
	unsigned int	flag;		/*  bit flag or-ed together */
	float			scale;
	Vector2D		posCurr;	/* object current position */
	Vector2D		velCurr;	/* object current velocity */
	float			dirCurr;	/* object current direction */

	Matrix2D		transform;	/* object drawing matrix */

	/* Used to hold the current */
	int				gridCollisionFlag;

	/* pointer to custom data specific for each object type */
	void*			pUserData;

	/* State of the object instance */
	enum			STATE state;
	enum			INNER_STATE innerState;

	/* General purpose counter (This variable will be used for the enemy state machine) */
	double			counter;
}GameObjInst;

GameObj* AllGameObjects(int numObjects);
GameObjInst* AllGameObjectInsts(int numInsts);
GameObjInst* gameObjInstCreate(GameObjInst* sGameObjInstList, GameObj* sGameObjList, unsigned int type, float scale, Vector2D* pPos, Vector2D* pVel, float dir, enum STATE startState);
void gameObjInstDestroy(GameObjInst* pInst);

void make_triangle_object(GameObj* pObj, float x, float y, int color);
void make_square_object(GameObj* pObj, float x, float y, int color);
void make_circle_object(GameObj* pObj, int parts, int color);

#endif
