#include "Objects.h"

AEGfxVertexList* make_triangle_mesh(float x, float y, int color) {
	AEGfxMeshStart();

	/* 1st argument: X    */
	/* 2nd argument: Y    */
	/* 3rd argument: ARGB */
	AEGfxTriAdd(
		-x,    -y, color, 0.0f, 0.0f,
		-x,    y, color, 0.0f, 0.0f,
		 x, 0.0f, color, 0.0f, 0.0f);

	return AEGfxMeshEnd();
}

AEGfxVertexList* make_rectangle_mesh(float x, float y, int color) {
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

AEGfxVertexList* make_halfcap_mesh(float x, float y, int parts, int color) {
	int i;
	float CircleAngleStep;

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

	CircleAngleStep = PI / (parts * 2);
	for (i = 0; i < parts; ++i) {
		AEGfxTriAdd(
			x, 0, color, 0.0f, 0.0f,
			cosf(i * 2 * CircleAngleStep)*0.5f + x, sinf(i * 2 * CircleAngleStep)*0.5f, color, 0.0f, 0.0f,
			cosf((i + 1) * 2 * CircleAngleStep)*0.5f + x, sinf((i + 1) * 2 * CircleAngleStep)*0.5f, color, 0.0f, 0.0f);
	}

	return AEGfxMeshEnd();
}