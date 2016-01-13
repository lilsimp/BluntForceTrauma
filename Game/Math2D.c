/*---------------------------------------------------------------------------
Project Title         :       CS 230: Project 2
File Name             :       Math2D.c
Author                :       Sawyer Simpson
Creation Date         :       1/8/2016
Purpose               :
History
-1/8/2016    :       Added file header.
� Copyright 1996-2016, DigiPen Institute of Technology (USA). All rights reserved.
----------------------------------------------------------------------------*/

#include "Math2D.h"
#include <stdlib.h>

/*
This function checks if the point P is colliding with the circle whose
center is "Center" and radius is "Radius"
*/
int StaticPointToStaticCircle(Vector2D* pP, Vector2D* pCenter, float Radius) {
	if (Vector2DSquareDistance(pP, pCenter) > (Radius * Radius) )
		return 0;

	return 1;
}

/*
This function checks if the point Pos is colliding with the rectangle
whose center is Rect, width is "Width" and height is Height
*/
int StaticPointToStaticRect(Vector2D *pPos, Vector2D* pRect, float Width, float Height) {
	float x1, x2;
	float y1, y2;

	x1 = pRect->x - ( Width / 2 );
	x2 = pRect->x + ( Width / 2 );

	y1 = pRect->y - ( Height / 2 );
	y2 = pRect->y + ( Height / 2 );

	if (pPos->x >= x1 && pPos->x <= x2)
		if (pPos->y >= y1 && pPos->y <= y2)
			return 1;

	return 0;
}

/*
This function checks for collision between 2 circles.
Circle0: Center is Center0, radius is "Radius0"
Circle1: Center is Center1, radius is "Radius1"
*/
int StaticCircleToStaticCircle(Vector2D* pCenter0, float Radius0, Vector2D* pCenter1, float Radius1) {
	if(Vector2DDistance(pCenter0, pCenter1) <= ( Radius0 + Radius1 ))
		return 1;

	return 0;
}

/*
This functions checks if 2 rectangles are colliding
Rectangle0: Center is pRect0, width is "Width0" and height is "Height0"
Rectangle1: Center is pRect1, width is "Width1" and height is "Height1"
*/
int StaticRectToStaticRect(Vector2D* pRect0, float Width0, float Height0, Vector2D* pRect1, float Width1, float Height1) {
	if( StaticPointToStaticRect(pRect1, pRect0, ( Width0 + Width1 ), ( Height0 + Height1 )) )
		return 1;

	return 0;
}

//////////////////////
// New to Project 3 //
//////////////////////

/*
This function checks if a static circle is intersecting with a static rectangle

Circle:		Center is "Center", radius is "Radius"
Rectangle:	Center is "Rect", width is "Width" and height is "Height"
Function returns true is the circle and rectangle are intersecting, otherwise it returns false
*/

int StaticCircleToStaticRect(Vector2D* pCenter, float Radius, Vector2D* pRect, float Width, float Height) {
	float x = fabsf(pCenter->x - pRect->x);
	float y = fabsf(pCenter->y - pRect->y);
	float dist;

	if (x  > (Width  / 2 + Radius)) return 0;
	if (y  > (Height / 2 + Radius)) return 0;

	if (x <= (Width  / 2))          return 1;
	if (y <= (Height / 2))          return 1;

	dist = ((x - Width / 2) * (x - Width / 2)) + ((y - Height / 2) * (y - Height / 2));

	return (int)(dist <= (Radius * Radius));
}
