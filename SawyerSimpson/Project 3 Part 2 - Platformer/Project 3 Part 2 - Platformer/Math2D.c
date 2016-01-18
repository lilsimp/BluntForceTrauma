/*---------------------------------------------------------------------------
Project Title         :       CS 230: Project 2 part 1
File Name             :       Math2D.c
Author                :       Sawyer Simpson
Creation Date         :       1/13/2016
Purpose               :       Collision functions.
History
-1/13/2016    :       Added file header.
-1/13/2016    :       Implemented all functions.
� Copyright 1996-2016, DigiPen Institute of Technology (USA). All rights reserved.
----------------------------------------------------------------------------*/

#include "Math2D.h"
#include "stdio.h"

/***************************************************************************/
/*!

  \brief
    This function checks if the point is colliding with the circle

  \param pP
    Point to check if colliding with circle

  \param pCenter
    Center of the circle to check for collision

  \param Radius
    Radius of the given circle

  \return
    1 or 0 if the objects collide or not
*/
/***************************************************************************/
int StaticPointToStaticCircle(Vector2D *pP, Vector2D *pCenter, float Radius) {
	return Vector2DSquareDistance(pCenter, pP) <= Radius * Radius;
}


/***************************************************************************/
/*!

  \brief
    This function checks if the point is colliding with the rectangle

  \param pPos
    The point to check if colliding with the rectangle

  \param pRect
    Center of the given rectangle to check for collision

  \param Width
    Width of the given rectangle

  \param Height
    Height of the given rectangle

  \return
    1 or 0 if the objects collide or not
*/
/***************************************************************************/
int StaticPointToStaticRect(Vector2D *pPos, Vector2D *pRect, float Width, float Height) {
  Width /= 2;
  Height /= 2;

  if (pPos->x >= (pRect->x - Width) && pPos->x <= (pRect->x + Width))
    return (pPos->y >= (pRect->y - Height) && pPos->y <= (pRect->y + Height));
  return 0;
}

/***************************************************************************/
/*!

  \brief
    This function checks for collision between 2 circles.

  \param pCenter0
    Center of the first circle given

  \param Radius0
    Radius of the first circle given

  \param pCenter1
    Center of the second circle given

  \param Radius1
    Radius of the second circle given

  \return
    1 or 0 if the objects collide or not
*/
/***************************************************************************/
int StaticCircleToStaticCircle(Vector2D *pCenter0, float Radius0, Vector2D *pCenter1, float Radius1) {
  return StaticPointToStaticCircle(pCenter0, pCenter1, Radius0 + Radius0);
}

/***************************************************************************/
/*!

  \brief
    This functions checks if 2 rectangles are colliding

  \param pRect0
    Center of the first given rectangle

  \param Width0
    Width of the first given rectangle

  \param Height0
    Height of the first given rectangle

  \param pRect1
    Center of the second given rectangle

  \param Width1
    Width of the second given rectangle

  \param Height1
    Height of the second given rectangle

  \return
    1 or 0 if the objects collide or not
*/
/***************************************************************************/
int StaticRectToStaticRect(Vector2D *pRect0, float Width0, float Height0, Vector2D *pRect1, float Width1, float Height1) {
  return StaticPointToStaticRect(pRect1, pRect0, ( Width0 + Width1 ), ( Height0 + Height1 ));
}
