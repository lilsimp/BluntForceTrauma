/*---------------------------------------------------------------------------
Project Title         :       CS 230: Project 2
File Name             :       Matrix2D.c
Author                :       Sawyer Simpson
Creation Date         :       1/8/2016
Purpose               :
History
-1/8/2016    :       Added file header.
� Copyright 1996-2016, DigiPen Institute of Technology (USA). All rights reserved.
----------------------------------------------------------------------------*/

#include "Matrix2D.h"

static void copyMatrix(Matrix2D* pResult, Matrix2D* pMtx) {
	int i, j;

	for(i = 0; i < 3; ++i)
		for(j = 0; j < 3; ++j)
			pResult->m[i][j] = pMtx->m[i][j];
}

/*
This function sets the matrix Result to the identity matrix
*/
void Matrix2DIdentity(Matrix2D* pResult) {
	int i, j;

	for(i = 0; i < 3; i++)
		for(j = 0; j < 3; j++)
			pResult->m[i][j] = 0.0f;

	for(i = 0; i < 3; i++)
		pResult->m[i][i] = 1;
}

/*
This function calculates the transpose matrix of Mtx and saves it in Result
*/
void Matrix2DTranspose(Matrix2D* pResult, Matrix2D* pMtx) {
	Matrix2D hold = {0};
	int i,j;

	for(i = 0; i < 3; ++i)
		for(j = 0; j < 3; ++j)
			hold.m[i][j] = pMtx->m[j][i];

	copyMatrix(pResult, &hold);
}

/*
This function multiplies Mtx0 with Mtx1 and saves the result in Result
Result = Mtx0*Mtx1
*/
void Matrix2DConcat(Matrix2D* pResult, Matrix2D* pMtx0, Matrix2D* pMtx1) {
	Matrix2D hold = {0};
	int i,j;

	for(i = 0; i < 3; ++i)
		for(j = 0; j < 3; ++j)
			hold.m[i][j] =  (pMtx0->m[i][0] * pMtx1->m[0][j])
			 						  + (pMtx0->m[i][1] * pMtx1->m[1][j])
								    + (pMtx0->m[i][2] * pMtx1->m[2][j]);

	copyMatrix(pResult, &hold);
}

/*
This function creates a translation matrix from x *p y and saves it in Result
*/
void Matrix2DTranslate(Matrix2D* pResult, float x, float y) {
	Matrix2DIdentity(pResult);
	pResult->m[0][2] = x;
	pResult->m[1][2] = y;
}

/*
This function creates a scaling matrix from x *p y and saves it in Result
*/
void Matrix2DScale(Matrix2D* pResult, float x, float y) {
	Matrix2DIdentity(pResult);
	pResult->m[0][0] = x;
	pResult->m[1][1] = y;
}

/*
This matrix creates a rotation matrix from "Angle" whose value is in degree.
Save the resultant matrix in Result
*/
void Matrix2DRotDeg(Matrix2D* pResult, float Angle) {
	Angle = Angle * PI / 180;

	Matrix2DRotRad(pResult, Angle);
}

/*
This matrix creates a rotation matrix from "Angle" whose value is in radian.
Save the resultant matrix in Result
*/
void Matrix2DRotRad(Matrix2D* pResult, float Angle) {
	float x = cos(Angle);
	float y = sin(Angle);

	Matrix2DIdentity(pResult);
	pResult->m[0][0] = x;
	pResult->m[1][1] = x;
	pResult->m[0][1] = -y;
	pResult->m[1][0] = y;
}


/*
This function multiplies the matrix Mtx with the vector Vec and saves the result
in Result.
Result = Mtx * Vec
*/
void Matrix2DMultVec(Vector2D* pResult, Matrix2D* pMtx, Vector2D* pVec) {
	Matrix2D temp;
	Matrix2DTranslate(&temp, pVec->x, pVec->y);
	Matrix2DConcat(&temp, pMtx, &temp);
	Vector2DSet(pResult, temp.m[0][2], temp.m[1][2]);
}
