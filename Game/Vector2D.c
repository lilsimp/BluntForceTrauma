/*---------------------------------------------------------------------------
Project Title         :       CS 230: Project 2
File Name             :       Vector2D.c
Author                :       Sawyer Simpson
Creation Date         :       1/8/2016
Purpose               :
History
-1/8/2016    :       Added file header.
� Copyright 1996-2016, DigiPen Institute of Technology (USA). All rights reserved.
----------------------------------------------------------------------------*/

#include "Vector2D.h"

#define EPSILON 0.0001

void Vector2DZero(Vector2D* pResult) {
	pResult->x = 0;
	pResult->y = 0;
}


void Vector2DSet(Vector2D* pResult, float x, float y) {
	pResult->x = x;
	pResult->y = y;
}


void Vector2DNeg(Vector2D* pResult, Vector2D* pVec0) {
	pResult->x = pVec0->x * -1.0f;
	pResult->y = pVec0->y * -1.0f;
}


void Vector2DAdd(Vector2D* pResult, Vector2D* pVec0, Vector2D* pVec1) {
	pResult->x = pVec0->x + pVec1->x;
	pResult->y = pVec0->y + pVec1->y;
}


void Vector2DSub(Vector2D* pResult, Vector2D* pVec0, Vector2D* pVec1) {
	Vector2DScale(pVec1, pVec1, -1.0f);

	Vector2DAdd(pResult, pVec0, pVec1);
}


void Vector2DNormalize(Vector2D* pResult, Vector2D* pVec0) {
	float length = Vector2DLength(pVec0);

	Vector2DScale(pResult, pVec0, (1 / length));
}


void Vector2DScale(Vector2D* pResult, Vector2D* pVec0, float c) {
	pResult->x = pVec0->x * c;
	pResult->y = pVec0->y * c;
}


void Vector2DScaleAdd(Vector2D* pResult, Vector2D* pVec0, Vector2D* pVec1, float c) {
	Vector2DScale(pResult, pVec0, c);

	Vector2DAdd(pResult, pResult, pVec1);
}


void Vector2DScaleSub(Vector2D* pResult, Vector2D* pVec0, Vector2D* pVec1, float c) {
	Vector2DScale(pResult, pVec0, c);

	Vector2DSub(pResult, pResult, pVec1);
}


void Vector2DFromAngleDeg(Vector2D* pResult, float angle) {
	pResult->x = cos(angle);
	pResult->y = sin(angle);
}


void Vector2DFromAngleRad(Vector2D* pResult, float angle) {
	Vector2DFromAngleDeg(pResult, angle * 180.0f / PI);
}


float Vector2DLength(Vector2D* pVec0) {
	return sqrt(Vector2DSquareLength(pVec0));
}


float Vector2DSquareLength(Vector2D* pVec0) {
	return (pVec0->x * pVec0->x) + (pVec0->y * pVec0->y);
}


float Vector2DDistance(Vector2D* pVec0, Vector2D* pVec1) {
	return sqrt(Vector2DSquareDistance(pVec0, pVec1));
}


float Vector2DSquareDistance(Vector2D* pVec0, Vector2D* pVec1) {
	return ((pVec0->x - pVec1->x) * (pVec0->x - pVec1->x))
		   + ((pVec0->y - pVec1->y) * (pVec0->y - pVec1->y));
}


float Vector2DDotProduct(Vector2D* pVec0, Vector2D* pVec1) {
	return (pVec0->x * pVec1->x) + (pVec0->y * pVec1->y);
}
