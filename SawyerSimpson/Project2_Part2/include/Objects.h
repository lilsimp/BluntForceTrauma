#include "AEEngine.h"
#include "Matrix2D.h"

#define SHAPE_NUM_MAX				32					// The total number of different vertex buffer (Shape)
#define GAME_OBJ_INST_NUM_MAX		2048				// The total number of different game object instances

enum OBJECT_TYPE
{
	// list of game object types
	OBJECT_TYPE_SHIP,
	OBJECT_TYPE_BULLET,
	OBJECT_TYPE_ASTEROID,
	OBJECT_TYPE_HOMING_MISSILE,
	OBJECT_TYPE_STAR_1,
	OBJECT_TYPE_STAR_2,
	OBJECT_TYPE_STAR_3,
	OBJECT_TYPE_PLANET,
	OBJECT_TYPE_NUM
};

// ---------------------------------------------------------------------------
// object mFlag definition

#define FLAG_ACTIVE		0x00000001

// ---------------------------------------------------------------------------

// Forward declaration needed, since components need to point to their owner "GameObjectInstance"
typedef struct GameObjectInstance GameObjectInstance;			

// ---------------------------------------------------------------------------

// Struct/Class definitions

typedef struct Shape {
	unsigned long mType;	 // Object type (Ship, bullet, etc..)
	AEGfxVertexList* mpMesh; // This will hold the triangles which will form the shape of the object
}Shape;

// ---------------------------------------------------------------------------

typedef struct Component_Sprite {
	Shape *mpShape;
	GameObjectInstance* mpOwner; // This component's owner
}Component_Sprite;

// ---------------------------------------------------------------------------

typedef struct Component_Transform {
	Vector2D mPosition;	 // Current position
	float mAngle;		 // Current angle
	float mScaleX;		 // Current X scaling value
	float mScaleY;		 // Current Y scaling value
	Matrix2D mTransform; // Object transformation matrix: Each frame, calculate the object instance's transformation matrix and save it here

		// This component's owner
	GameObjectInstance*	mpOwner;			
}Component_Transform;

// ---------------------------------------------------------------------------

typedef struct Component_Physics {
	Vector2D mVelocity;          // Current velocity
	GameObjectInstance*	mpOwner; // This component's owner
}Component_Physics;

// ---------------------------------------------------------------------------

typedef struct
{
	GameObjectInstance* mpTarget; // Target, used by the homing missile
	GameObjectInstance*	mpOwner;  // This component's owner
}Component_Target;

// ---------------------------------------------------------------------------


//Game object instance structure
struct GameObjectInstance
{
	unsigned long mFlag;						// Bit mFlag, used to indicate if the object instance is active or not
	AEGfxTexture* spriteSource;                 // This component's sprite
	Component_Sprite* mpComponent_Sprite;		// Sprite component
	Component_Transform* mpComponent_Transform;	// Transform component
	Component_Physics* mpComponent_Physics;		// Physics component
	Component_Target* mpComponent_Target;		// Target component, used by the homing missile
};

static Shape sgShapes[SHAPE_NUM_MAX]; // Each element in this array represents a unique shape 
static unsigned long sgShapeNum;	  // The number of defined shapes

// list of object instances
static GameObjectInstance sgGameObjectInstanceList[GAME_OBJ_INST_NUM_MAX];
static unsigned long sgGameObjectInstanceNum;

// ---------------------------------------------------------------------------
// Static variables

// functions to create/destroy a game object instance
GameObjectInstance*	GameObjectInstanceCreate(unsigned int ObjectType);
void GameObjectInstanceDestroy(GameObjectInstance* pInst);

void AddComponent_Transform(GameObjectInstance *pInst, Vector2D *pPosition, float Angle, float ScaleX, float ScaleY);
void AddComponent_Sprite(GameObjectInstance *pInst, unsigned int ShapeType);
void AddComponent_Physics(GameObjectInstance *pInst, Vector2D *pVelocity);
void AddComponent_Target(GameObjectInstance *pInst, GameObjectInstance *pTarget);

void RemoveComponent_Transform(GameObjectInstance *pInst);
void RemoveComponent_Sprite(GameObjectInstance *pInst);
void RemoveComponent_Physics(GameObjectInstance *pInst);
void RemoveComponent_Target(GameObjectInstance *pInst);

AEGfxVertexList* make_triangle_mesh(int color);
AEGfxVertexList* make_rectangle_mesh(int color);
AEGfxVertexList* make_star_mesh(int color);
AEGfxVertexList* make_circle_mesh(int parts, int color);
AEGfxVertexList* make_halfcap_mesh(int parts, int color);

