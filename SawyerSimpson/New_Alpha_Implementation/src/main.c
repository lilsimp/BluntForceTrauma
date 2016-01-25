// ---------------------------------------------------------------------------
// Project Name		:	Omega Library Test Program
// File Name		:	main.h
// Author			:	Sun Tjen Fam
// Creation Date	:	2007/04/26
// Purpose			:	main entry point for the test program
// History			:
// - 2007/04/26		:	- initial implementation
// ---------------------------------------------------------------------------

// ---------------------------------------------------------------------------
// includes

#include "main.h"

// ---------------------------------------------------------------------------
// Static function protoypes


// ---------------------------------------------------------------------------
// main

int WINAPI WinMain(HINSTANCE instanceH, HINSTANCE prevInstanceH, LPSTR command_line, int show)
{
	// Initialize the system
	if (!System_Initialize(instanceH, prevInstanceH, command_line, show))
		return 1;

	GameStateMgrInit(LEVEL_1);

	GSM_MainLoop();
	
	// free the system
	System_Exit();

	return 1;
}

// ---------------------------------------------------------------------------