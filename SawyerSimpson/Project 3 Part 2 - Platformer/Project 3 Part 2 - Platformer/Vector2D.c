/*---------------------------------------------------------------------------
Project Title         :       CS 230: Project 2 part 1
File Name             :       Vector2D.c
Author                :       Sawyer Simpson
Creation Date         :       1/13/2016
Purpose               :       Vector2D functions
History
-1/13/2016    :       Added file header.
-1/13/2016    :       Implemented all functions.
� Copyright 1996-2016, DigiPen Institute of Technology (USA). All rights reserved.
----------------------------------------------------------------------------*/

#include "Vector2D.h"

#define EPSILON 0.0001

/***************************************************************************/
/*!

  \brief
		This function sets the coordinates of the 2D vector (pResult) to 0

  \param pResult
    Resultant Vector

*/
/***************************************************************************/
void Vector2DZero(Vector2D *pResult) {
	pResult->x = 0;
	pResult->y = 0;
}

/***************************************************************************/
/*!

  \brief
		This function sets the coordinates of the 2D vector (pResult) to x &y

  \param pResult
    Resultant Vector

  \param x

  \param y

*/
/***************************************************************************/
void Vector2DSet(Vector2D *pResult, float x, float y) {
	pResult->x = x;
	pResult->y = y;
}

/***************************************************************************/
/*!

  \brief
		In this function, pResult will be set to the opposite of pVec0

  \param pResult
    Resultant Vector

  \param pVec0
    Vector to set negative

*/
/***************************************************************************/
void Vector2DNeg(Vector2D *pResult, Vector2D *pVec0) {
	pResult->x = -pVec0->x;
  pResult->y = -pVec0->y;
}

/***************************************************************************/
/*!

  \brief
		In this function, pResult will be the sum of pVec0 and pVec1

  \param pResult
    Resultant Vector

  \param pVec0
    1st Vector to add with

  \param pVec1
    2nd Vector to add with

*/
/***************************************************************************/
void Vector2DAdd(Vector2D *pResult, Vector2D *pVec0, Vector2D *pVec1) {
	pResult->x = pVec0->x + pVec1->x;
	pResult->y = pVec0->y + pVec1->y;
}

/***************************************************************************/
/*!

  \brief
		In this function, pResult will be the difference between pVec0 *pVec1: pVec0 - pVec1

  \param pResult
    Resultant Vector

  \param pVec0
    1st Vector to subtracte with

  \param pVec1
    2nd Vector to subtracte with

*/
/***************************************************************************/
void Vector2DSub(Vector2D *pResult, Vector2D *pVec0, Vector2D *pVec1) {
	pResult->x = pVec0->x - pVec1->x;
	pResult->y = pVec0->y - pVec1->y;
}

/***************************************************************************/
/*!

  \brief
		In this function, pResult will be the unit vector of pVec0

  \param pResult
    Resultant Vector

  \param pVec0
    Vector to normalize from

*/
/***************************************************************************/
void Vector2DNormalize(Vector2D *pResult, Vector2D *pVec0) {
	Vector2DScale(pResult, pVec0, (1 / Vector2DLength(pVec0)));
}

/***************************************************************************/
/*!

  \brief
		In this function, pResult will be the vector pVec0 scaled by the value c

  \param pResult
    Resultant Vector

  \param pVec0
    Vector to scale from

  \param c
    The value to scale with

*/
/***************************************************************************/
void Vector2DScale(Vector2D *pResult, Vector2D *pVec0, float c) {
	pResult->x = pVec0->x * c;
	pResult->y = pVec0->y * c;
}

/***************************************************************************/
/*!

  \brief
		In this function, pResult will be the vector pVec0 scaled by c and added to pVec1

  \param pResult
    Resultant Vector

  \param pVec0
    Vector 1st used

  \param pVec1
    Vector 2nd used

  \param c
    The value to scale with

*/
/***************************************************************************/
void Vector2DScaleAdd(Vector2D *pResult, Vector2D *pVec0, Vector2D *pVec1, float c) {
	Vector2DScale(pResult, pVec0, c);
	Vector2DAdd(pResult, pResult, pVec1);
}

/***************************************************************************/
/*!

  \brief
		In this function, pResult will be the vector pVec0 scaled by c and pVec1 will be subtracted from it

  \param pResult
    Resultant Vector

  \param pVec0
    Vector 1st used

  \param pVec1
    Vector 2nd used

  \param c
    The value to scale with

*/
/***************************************************************************/
void Vector2DScaleSub(Vector2D *pResult, Vector2D *pVec0, Vector2D *pVec1, float c) {
	Vector2DScale(pResult, pVec0, c);
	Vector2DSub(pResult, pResult, pVec1);
}

/***************************************************************************/
/*!

  \brief
		This function returns the length of the vector pVec0

  \param pVec0
    Vector to find the length of

  \return
    The length of the two vectors
*/
/***************************************************************************/
float Vector2DLength(Vector2D *pVec0) {
	return sqrt(Vector2DSquareLength(pVec0));
}

/***************************************************************************/
/*!

  \brief
		This function returns the square of pVec0's length. Avoid the square root

  \param pVec0
    Vector to find the length of

  \return
    The squared length of the two vectors
*/
/***************************************************************************/
float Vector2DSquareLength(Vector2D *pVec0) {
	return (pVec0->x * pVec0->x) + (pVec0->y * pVec0->y);
}

/***************************************************************************/
/*!

  \brief
    In this function, pVec0 and pVec1 are considered as 2D points.
    The distance between these 2 2D points is returned

  \param pVec0
    Vector 1st used to find the distance

  \param pVec1
    Vector 2nd used to find the distance

  \return
    The distance of the two vectors
*/
/***************************************************************************/
float Vector2DDistance(Vector2D *pVec0, Vector2D *pVec1) {
	return sqrt(Vector2DSquareDistance(pVec0, pVec1));
}

/***************************************************************************/
/*!

  \brief
    In this function, pVec0 and pVec1 are considered as 2D points.
    The squared distance between these 2 2D points is returned. Avoid the square root

  \param pVec0
    Vector 1st used to find the distance

  \param pVec1
    Vector 2nd used to find the distance

  \return
    The squared distance of the two vectors
*/
/***************************************************************************/
float Vector2DSquareDistance(Vector2D *pVec0, Vector2D *pVec1) {
	return ((pVec0->x - pVec1->x) * (pVec0->x - pVec1->x))
				+((pVec0->y - pVec1->y) * (pVec0->y - pVec1->y));
}

/***************************************************************************/
/*!

  \brief
		This function returns the dot product between pVec0 and pVec1

  \param pVec0
    Vector 1st used in the dot product

  \param pVec1
    Vector 2nd used in the dot product

  \return
    The dot product of the two vectors
*/
/***************************************************************************/
float Vector2DDotProduct(Vector2D *pVec0, Vector2D *pVec1) {
	return (pVec0->x * pVec1->x) + (pVec0->y * pVec1->y);
}

/***************************************************************************/
/*!

  \brief
		This function computes the coordinates of the vector represented by the angle "angle", which is in Degrees

  \param pResult
    Resultant Vector

  \param angle
    The angle to make the vector from

*/
/***************************************************************************/
void Vector2DFromAngleDeg(Vector2D *pResult, float angle) {
	Vector2DFromAngleRad(pResult, angle * PI / 180.0f);
}

/***************************************************************************/
/*!

  \brief
		This function computes the coordinates of the vector represented by the angle "angle", which is in Radian

  \param pResult
    Resultant Vector

  \param angle
    The angle to make the vector from

*/
/***************************************************************************/
void Vector2DFromAngleRad(Vector2D *pResult, float angle) {
	pResult->x = cos(angle);
	pResult->y = sin(angle);
}
