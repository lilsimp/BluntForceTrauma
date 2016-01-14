/*---------------------------------------------------------------------------
Project Title         :       CS 230: Project 2 part 1
File Name             :       Matrix2D.c
Author                :       Sawyer Simpson
Creation Date         :       1/13/2016
Purpose               :       Matrix2D functions
History
-1/13/2016    :       Added file header.
-1/13/2016    :       Implemented all functions.
-1/13/2016    :       Fixed my set_matrix helper, it wasn't working.
� Copyright 1996-2016, DigiPen Institute of Technology (USA). All rights reserved.
----------------------------------------------------------------------------*/

#include "Matrix2D.h"

/***************************************************************************/
/*!

  \brief
    This function sets the matrix Result to Mtx

  \param pResult
    Resultant Matrix

  \param pMtx
    Matrix to set to

*/
/***************************************************************************/
static void set_matrix(Matrix2D *pResult, Matrix2D *pMtx) {
  int i, j;
  for(i = 0; i < 3; ++i)
    for(j = 0; j < 3; ++j)
      pResult->m[i][j] = pMtx->m[i][j];
}

/***************************************************************************/
/*!

  \brief
    This function sets the matrix Result to the identity matrix

  \param pResult
    Resultant Matrix

*/
/***************************************************************************/
void Matrix2DIdentity(Matrix2D *pResult) {
  int i, j;

  for(i = 0; i < 3; ++i)
    for(j = 0; j < 3; ++j)
      pResult->m[i][j] = (i == j);
}

/***************************************************************************/
/*!

  \brief
    This functions calculated the transpose matrix of Mtx and saves it in Result

  \param pResult
    Resultant Matrix

  \param pMtx
    Matrix to transpose

*/
/***************************************************************************/
void Matrix2DTranspose(Matrix2D *pResult, Matrix2D *pMtx) {
  int i, j;
  Matrix2D temp;
  for(i = 0; i < 3; ++i)
    for(j = 0; j < 3; ++j)
      temp.m[i][j] = pMtx->m[j][i];
  set_matrix(pResult, &temp);
}

/***************************************************************************/
/*!

  \brief
    This function multiplies Mtx0 with Mtx1 and saves the result in Result
    Result = Mtx0*Mtx1

  \param pResult
    Resultant Matrix

  \param pMtx0
    First matrix to concat

  \param pMtx1
    Second matrix to concat

*/
/***************************************************************************/
void Matrix2DConcat(Matrix2D *pResult, Matrix2D *pMtx0, Matrix2D *pMtx1) {
  Matrix2D hold = {0};
	int i,j;

	for(i = 0; i < 3; ++i)
		for(j = 0; j < 3; ++j)
			hold.m[i][j] =  (pMtx0->m[i][0] * pMtx1->m[0][j])
			 						  + (pMtx0->m[i][1] * pMtx1->m[1][j])
								    + (pMtx0->m[i][2] * pMtx1->m[2][j]);

	set_matrix(pResult, &hold);
}

/***************************************************************************/
/*!

  \brief
    This function creates a translation matrix from x *p y and saves it in Result

  \param pResult
    Resultant Matrix

  \param x
    X value of the translation

  \param y
    Y value of the translation

*/
/***************************************************************************/
void Matrix2DTranslate(Matrix2D *pResult, float x, float y) {
  Matrix2DIdentity(pResult);
  pResult->m[0][2] = x;
  pResult->m[1][2] = y;
}

/***************************************************************************/
/*!

  \brief
    This function creates a scaling matrix from x *p y and saves it in Result

  \param pResult
    Resultant Matrix

  \param x
    X value of the scale

  \param y
    Y value of the scale

*/
/***************************************************************************/
void Matrix2DScale(Matrix2D *pResult, float x, float y) {
  Matrix2DIdentity(pResult);
  pResult->m[0][0] = x;
  pResult->m[1][1] = y;
}

/***************************************************************************/
/*!

  \brief
    This matrix creates a rotation matrix from "Angle" whose value is in degree.
    Save the resultant matrix in Result

  \param pResult
    Resultant Matrix

  \param Angle
    Angle to make the rotation matrix

*/
/***************************************************************************/
void Matrix2DRotDeg(Matrix2D *pResult, float Angle) {
  Matrix2DRotRad(pResult, Angle * (PI / 180.0f));
}

/***************************************************************************/
/*!

  \brief
    This matrix creates a rotation matrix from "Angle" whose value is in radian.
    Save the resultant matrix in Result

  \param pResult
    Resultant Matrix

  \param Angle
    Angle to make the rotation matrix

*/
/***************************************************************************/
void Matrix2DRotRad(Matrix2D *pResult, float Angle) {
  Matrix2DIdentity(pResult);
  pResult->m[0][0] = cos(Angle);
  pResult->m[1][1] = pResult->m[0][0];
  pResult->m[1][0] = sin(Angle);
  pResult->m[0][1] = -pResult->m[1][0];
}

/***************************************************************************/
/*!

  \brief
    This function multiplies the matrix Mtx with the vector Vec and saves the result in Result
    Result = Mtx * Vec

  \param pResult
    Resultant Vector

  \param pMtx
    The matrix to multiply with

  \param pVec
    The vector to multiply with

*/
/***************************************************************************/
void Matrix2DMultVec(Vector2D *pResult, Matrix2D *pMtx, Vector2D *pVec) {
  float x = pVec->x, y = pVec->y;
  pResult->x = (pMtx->m[0][0] * x) + (pMtx->m[0][1] * y) + pMtx->m[0][2];
  pResult->y = (pMtx->m[1][0] * x) + (pMtx->m[1][1] * y) + pMtx->m[1][2];
}
