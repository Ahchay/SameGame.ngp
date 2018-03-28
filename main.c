//Standard library header files
#include "ngpc.h"
#include "carthdr.h"
#include "library.h"

//Graphics
#include "Cursor.c"
#include "Blocks.c"
#include "ASCII.c"

//Music
#include "Canyon.h"
#include "Suicide.h"

//Game-specific functions
#include "samegame.h"
#include "samegame.c"

void main()
{
	CURSOR cPlayer;
	u8 iPalette;
	u16 iTile;
	u8 iColour;
	u8 iDropLoop;

	InitNGPC();
	SysSetSystemFont();
	InitialiseQRandom();

	//Neotracker
	NeoTracker_InstallDriver();
	NeoTracker_SendGroup(Suicide_Data, Suicide_Len);

	ClearScreen(SCR_1_PLANE);
	ClearScreen(SCR_2_PLANE);
	SetBackgroundColour(RGB(0, 0, 0));

	//Install tile sets, create Palettes etc

	while(1)
	{

		Samegame_Initialise();

		Samegame_SplashScreen();

		//Create a new game
		cPlayer=Samegame_StartGame();

		//Need another structure in here when I want to implement different game modes...

		while (cPlayer.Flags!=FLAG_GAMEOVER)
		{

			//Draw level
			Samegame_DrawLevel(cPlayer);

			while (cPlayer.Flags!=FLAG_GAMEOVER)
			{
				cPlayer=Samegame_Move(cPlayer);

				//For "Endless" mode. Determine whether to drop another line of blocks
				//onto the puzzle. Not sure whether this should be time based or
				//event based (ie. when score reaches a threshold, or a number of
				//blocks have been cleared)
				//Or. Maybe a combination of the two.
				//Okay.
				//Timer determines tiles dropped from above.
				//Tiles will also be pushed onto the stack from the right hand side if
				//there is room...
				if (cPlayer.GameType==SAMEGAME_ENDLESS)
				{
					//Condition 1. Is there an empty column at the right hand side
					GetTile(SCR_2_PLANE, &iPalette, MAX_X-1, MAX_Y-1, &iTile);
					if (iTile==TILE_EMPTY)
					{
						//Yes there is. Drop *some* random tiles in the right hand column
						for (iDropLoop=0;iDropLoop<10;iDropLoop++)
						{
							iColour=QRandom()/(260/3);
							PutTile(SCR_2_PLANE, PAL_BLOCK+iColour, MAX_X-1, iDropLoop, TILE_BLOCK);
						}
						//Collapse the tiles
						Samegame_Collapse(SCR_2_PLANE);
					}

					//Condition 2. Has the timer counted down?
					cPlayer.Timer+=cPlayer.TimerIncrement;
					if (cPlayer.Timer<cPlayer.TimerIncrement)
					{
						cPlayer.TimerIncrement++;
						PrintDecimal(SCR_1_PLANE, 0, 0, 18, cPlayer.TimerIncrement, 4);
						//Drop some stuff!
						for (iDropLoop=0;iDropLoop<MAX_X;iDropLoop++)
						{
							GetTile(SCR_2_PLANE, &iPalette, iDropLoop, 0, &iTile);
							if (iTile!=TILE_EMPTY)
							{
								//This doesn't appear to be working too well
								cPlayer.Flags=FLAG_GAMEOVER;
								//Will work perfectly if I put the right number of "=" signs in...
							}
							else
							{
								iColour=QRandom()/(260/3);
								PutTile(SCR_2_PLANE, PAL_BLOCK+iColour, iDropLoop, 0, TILE_BLOCK);
							}
						}
						//Collapse the tiles
						Samegame_Collapse(SCR_2_PLANE);
					}
				}

			}
			//For Puzzle mode. Catch the "GAME OVER" condition and, if the puzzle
			//is cleared, move to the next level.

			if (cPlayer.GameType==SAMEGAME_PUZZLE)
			{
				GetTile(SCR_2_PLANE, &iPalette, 0, 16, &iTile);
				if (iTile!=TILE_BLOCK)
				{
					//It's cleared!
					cPlayer.Flags=0;
					cPlayer.Level++;
					if (cPlayer.Level>MAX_LEVEL) cPlayer.Level=0;
				}
			}

		}

		//Game over man
		//Display an encouraging message and get the fuck out of here...
		PrintString(SCR_1_PLANE,0, 0, 18, "GAME OVER");
		Sleep(250);
	}
}


