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

/*
This function checks if the point P is colliding with the circle whose
center is "Center" and radius is "Radius"
*/
int StaticPointToStaticCircle(Vector2D *pP, Vector2D *pCenter, float Radius) {
	return Vector2DSquareDistance(pCenter, pP) <= Radius * Radius;
}


/*
This function checks if the point Pos is colliding with the rectangle
whose center is Rect, width is "Width" and height is Height
*/
int StaticPointToStaticRect(Vector2D *pPos, Vector2D *pRect, float Width, float Height) {
  Width /= 2;
  Height /= 2;

  if (pPos->x >= (pRect->x - Width) && pPos->x <= (pRect->x + Width))
    return (pPos->y >= (pRect->y - Height) && pPos->y <= (pRect->y + Height));
  return 0;
}

/*
This function checks for collision between 2 circles.
Circle0: Center is Center0, radius is "Radius0"
Circle1: Center is Center1, radius is "Radius1"
*/
int StaticCircleToStaticCircle(Vector2D *pCenter0, float Radius0, Vector2D *pCenter1, float Radius1) {
  return StaticPointToStaticCircle(pCenter0, pCenter1, Radius0 + Radius0);
}

/*
This functions checks if 2 rectangles are colliding
Rectangle0: Center is pRect0, width is "Width0" and height is "Height0"
Rectangle1: Center is pRect1, width is "Width1" and height is "Height1"
*/
int StaticRectToStaticRect(Vector2D *pRect0, float Width0, float Height0, Vector2D *pRect1, float Width1, float Height1) {
  return StaticPointToStaticRect(pRect1, pRect0, ( Width0 + Width1 ), ( Height0 + Height1 ));
}
