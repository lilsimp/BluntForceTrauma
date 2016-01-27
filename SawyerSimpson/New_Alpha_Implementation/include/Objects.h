#ifndef OBJECTS_H
#define OBJECTS_H

#include "WorldSettings.h"
#include "AEEngine.h"
#include "Matrix2D.h"

#define FLAG_ACTIVE      0x00000001

/* --------------------------------------------------------------------------- */

enum OBJECT_TYPE {
	OBJECT_TYPE_EMPTY,		  /* 0 */
	OBJECT_TYPE_COLLISION,	  /* 1 */
	OBJECT_TYPE_SHIP,
	OBJECT_TYPE_BULLET,
	OBJECT_TYPE_ASTEROID_SQUARE,
	OBJECT_TYPE_ASTEROID_CIRCLE,
	OBJECT_TYPE_HOMING_MISSILE,
	OBJECT_TYPE_PLAYER,		  /* 2 */
	OBJECT_TYPE_ENEMY1,		  /* 3 */
	OBJECT_TYPE_PUNCH,        /* 4 */
	OBJECT_TYPE_ORB,          /* 5 */
	OBJECT_TYPE_COIN,	      /* 6 */
	OBJECT_TYPE_PLAYER_BITS   /* 7 */
};

/* --------------------------------------------------------------------------- */

/* State machine states */
enum ENEMY_STATE {
	STATE_NONE,
	STATE_GOING_LEFT,
	STATE_GOING_RIGHT
};

/* --------------------------------------------------------------------------- */

/* State machine inner states */
enum ENEMY_INNER_STATE {
	INNER_STATE_ON_ENTER,
	INNER_STATE_ON_UPDATE,
	INNER_STATE_ON_EXIT
};

/* --------------------------------------------------------------------------- */

/* Forward declaration needed, since components need to point to their owner "GameObjectInstance" */
typedef struct GameObjectInstance GameObjectInstance;

/* --------------------------------------------------------------------------- */

typedef struct Shape {
	unsigned long mType;		 /* Object type (Player, enemy, etc..)  */
	AEGfxVertexList* mpMesh;	 /* This will hold the triangles which will form the shape of the object */
	GameObjectInstance* mpOwner; /* This component's owner */
}Shape;

/* --------------------------------------------------------------------------- */

typedef struct Component_Sprite {
	Shape* mpShape;
	GameObjectInstance* mpOwner; /* This component's owner */
}Component_Sprite;

/* --------------------------------------------------------------------------- */

typedef struct Component_Transform {
	float mAngle;			     /* Current angle                */
	float mScaleX;			     /* Current X scaling value      */
	float mScaleY;			     /* Current Y scaling value      */
	Vector2D mPosition;			 /* Current position             */
	Matrix2D mTransform;		 /* Object transformation matrix */
	GameObjectInstance* mpOwner; /* This component's owner       */
}Component_Transform;

/* --------------------------------------------------------------------------- */

typedef struct Component_Physics {
	Vector2D mVelocity;			 /* Current velocity */
	GameObjectInstance*	mpOwner; /* This component's owner */
}Component_Physics;

/* --------------------------------------------------------------------------- */

typedef struct Component_Target {
	GameObjectInstance*	mpTarget; /* Target, used by the homing missile */
	GameObjectInstance*	mpOwner;  /* This component's owner */
}Component_Target;

/* --------------------------------------------------------------------------- */

/* Game object instance structure */
struct GameObjectInstance {
	/* Bit mFlag, used to active or not */
	unsigned long mFlag;             			
	Component_Sprite*    mpComponent_Sprite;	/* Sprite component    */
	Component_Transform* mpComponent_Transform;	/* Transform component */
	Component_Physics*   mpComponent_Physics;	/* Physics component   */
	Component_Target*    mpComponent_Target;	/* Target component    */
};

/* --------------------------------------------------------------------------- */
/*
GameObjectInstance* GameObjectInstanceCreate(GameObjectInstance* GameObjInstList, Shape* Shapes, int* ObjectAmount, unsigned int ObjectType);
void GameObjectInstanceDestroy(GameObjectInstance* pInst, int* ObjectAmount);

void AddComponent_Transform(GameObjectInstance *pInst, Vector2D *pPosition, float Angle, float ScaleX, float ScaleY);
void AddComponent_Sprite(GameObjectInstance *pInst, Shape* Shapes, unsigned int ShapeType);
void AddComponent_Physics(GameObjectInstance *pInst, Vector2D *pVelocity);
void AddComponent_Target(GameObjectInstance *pInst, GameObjectInstance *pTarget);

void RemoveComponent_Transform(GameObjectInstance *pInst);
void RemoveComponent_Sprite(GameObjectInstance *pInst);
void RemoveComponent_Physics(GameObjectInstance *pInst);
void RemoveComponent_Target(GameObjectInstance *pInst);
*/
AEGfxVertexList* make_triangle_mesh(float x, float y, int color);
AEGfxVertexList* make_rectangle_mesh(float x, float y, int color);
AEGfxVertexList* make_circle_mesh(int parts, int color);
AEGfxVertexList* make_halfcap_mesh(float x, float y, int parts, int color);

#endif
