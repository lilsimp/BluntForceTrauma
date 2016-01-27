/*---------------------------------------------------------------------------
Project Title         :       CS 230: Project 1
File Name             :       GameStateList.h
Author                :       Sawyer Simpson
Creation Date         :       1/7/2016
Purpose               :       Contains the enum Game_States for level changing
History
-1/7/2016    :       Worked on and finished all prototypes.
-1/7/2016    :       Added file header.
� Copyright 1996-2016, DigiPen Institute of Technology (USA). All rights reserved.
----------------------------------------------------------------------------*/

#ifndef GAME_STATE_LIST_H
#define GAME_STATE_LIST_H

enum Game_States {
	LEVEL_1,
	LEVEL_2,
	LEVEL_3,
	  /* Keep this one last. Used to keep track of the number of levels */
	LEVEL_NUM,

	RESTART,
	QUIT
};

#endif
