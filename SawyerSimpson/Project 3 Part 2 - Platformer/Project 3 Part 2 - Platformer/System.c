/*---------------------------------------------------------------------------
Project Title         :       CS 230: Project 1
File Name             :       System.c
Author                :       Sawyer Simpson
Creation Date         :       1/7/2016
Purpose               :       Contains System calls for the game loop
History
-1/7/2016    :       Worked on and finished all functions.
-1/7/2016    :       Added file header.
� Copyright 1996-2016, DigiPen Institute of Technology (USA). All rights reserved.
----------------------------------------------------------------------------*/

#include "System.h"
#include <stdio.h> /* fopen_s, fprintf, fclose */

FILE* output;

/***************************************************************************/
/*!

	\brief
		Initializes the system

*/
/***************************************************************************/
int System_Initialize(HINSTANCE instanceH, HINSTANCE prevInstanceH, LPSTR command_line, int show) {
	/* Initialize the system */
	AESysInitInfo sysInitInfo;

	sysInitInfo.mCreateWindow = 1;
	sysInitInfo.mWindowHandle = NULL;
	sysInitInfo.mAppInstance = instanceH;
	sysInitInfo.mShow = show;
	sysInitInfo.mWinWidth = 1280;
	sysInitInfo.mWinHeight = 800;
	sysInitInfo.mCreateConsole = 1;
	sysInitInfo.mMaxFrameRate = 60;
	sysInitInfo.mpWinCallBack = NULL;
	sysInitInfo.mClassStyle = CS_HREDRAW | CS_VREDRAW;
	sysInitInfo.mWindowStyle = WS_OVERLAPPEDWINDOW;
	sysInitInfo.mHandleWindowMessages = 1;

	if (0 == AESysInit(&sysInitInfo))
		return 1;

	fopen_s(&output, "Output.txt", "wt"); /* Open the output file */

	fprintf(output, "System:Initialize\n");

	return 0;
}

/***************************************************************************/
/*!

	\brief
		Proporly exits the system

*/
/***************************************************************************/
void System_Exit(void) {
	/* free the system */
	AESysExit();

	fprintf(output, "System:Exit\n");

	fclose(output); /* Close the output file */
}
