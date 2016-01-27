#include "Objects.h"

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
