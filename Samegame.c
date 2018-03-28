/*
Samegame.c

Support functions for SameGame
*/

#include "levels.c"

// copies a new tile graphic into tile memory.
void CopySpriteTile(u16 * SpriteTiles, u16 TileNumber, u8 AnimNumber)
{
	u8 i;
	u16 * TileRam = TILE_RAM;
	//Our input parameter. NOT a global constant
	u16 * theSpriteAnim = (u16 *) SpriteTiles;

	TileRam += (TileNumber * 8);
	theSpriteAnim += (AnimNumber * 8);

	//We're only copying a single tile!
	for (i = 0; i < 8; i ++)
	{
		TileRam[i] = *theSpriteAnim++;
	}
}

void Samegame_Initialise()
{
	//Create Palletes
	SetPalette(SCR_2_PLANE, 0, 0, 0, 0, RGB(15,15,15));
	SetPalette(SCR_1_PLANE, 0, 0, RGB(15,15,15), RGB(7,7,7), RGB(3,3,3));
 	SetPalette(SPRITE_PLANE, PAL_CURSOR, 0, RGB(15,15,15), RGB(11,11,11), RGB(7, 7, 7));
 	SetPalette(SCR_2_PLANE, PAL_BLOCK, 0, RGB(11,11,11), RGB(7,7,7), RGB(15, 0, 0));
 	SetPalette(SCR_2_PLANE, PAL_BLOCK+1, 0, RGB(11,11,11), RGB(7,7,7), RGB(0, 15, 0));
 	SetPalette(SCR_2_PLANE, PAL_BLOCK+2, 0, RGB(11,11,11), RGB(7,7,7), RGB(0, 0, 15));
 	SetPalette(SCR_2_PLANE, PAL_BLOCK+3, 0, RGB(11,11,11), RGB(7,7,7), RGB(0, 15, 15));
 	SetPalette(SCR_2_PLANE, PAL_BLOCK+4, 0, RGB(11,11,11), RGB(7,7,7), RGB(15, 15, 0));
 	SetPalette(SCR_2_PLANE, PAL_BLOCK+5, 0, RGB(11,11,11), RGB(7,7,7), RGB(15, 0, 15));
 	SetPalette(SCR_2_PLANE, PAL_BLOCK+6, 0, RGB(11,11,11), RGB(7,7,7), RGB(15, 15, 15));
 	SetPalette(SCR_2_PLANE, PAL_BLOCK+7, 0, RGB(11,11,11), RGB(7,7,7), RGB(0, 0, 0));

	//Create a sprite for the cursor
	SetSprite(SPRITE_CURSOR, TILE_EMPTY, 0, 0, 0, PAL_CURSOR);

	//Install the Blocks tile set
	InstallTileSetAt(Blocks, sizeof(Blocks)/2, TILE_BLOCK);

	//Install the Alphabet tile set
	InstallTileSetAt(ASCII, sizeof(ASCII)/2, 0);
}

CURSOR Samegame_StartGame()
{
	CURSOR cPlayer;

	cPlayer.Animation=0;
	cPlayer.xPosition=10<<10;
	cPlayer.yPosition=8<<10;
	cPlayer.xAcceleration=0;
	cPlayer.yAcceleration=0;
	cPlayer.Score=0;
	cPlayer.Lives=3;
	cPlayer.Flags=0;
	cPlayer.Timer=0;
	cPlayer.TimerIncrement=1;

	//Clear the input buffer
	while (JOYPAD & J_A);

	//Display a menu of some form...
	ClearScreen(SCR_1_PLANE);
	ClearScreen(SCR_2_PLANE);

	PrintString(SCR_1_PLANE, 0, 2, 0, "Same Game");
	PrintString(SCR_1_PLANE, 0, 2, 2, "Select game");
	PrintString(SCR_1_PLANE, 0, 2, 4, "Classic");
	PrintString(SCR_1_PLANE, 0, 2, 5, "Puzzle");
	PrintString(SCR_1_PLANE, 0, 2, 6, "Endless");

	//Create a cursor...
	cPlayer.GameType=0;
	PutTile(SCR_1_PLANE, 0, 1, 4+cPlayer.GameType, TILE_CURSOR);
	//Wait for the user to choose game
	while (!(JOYPAD & J_A))
	{
		//Do stuff
		CopySpriteTile((u16*)Cursor, TILE_CURSOR, cPlayer.Animation >> 6);
		cPlayer.Animation++;
		if (JOYPAD & J_UP)
		{
			PutTile(SCR_1_PLANE, 0, 1, 4+cPlayer.GameType, TILE_EMPTY);
			cPlayer.GameType--;
			if (cPlayer.GameType>2) cPlayer.GameType=2;
			PutTile(SCR_1_PLANE, 0, 1, 4+cPlayer.GameType, TILE_CURSOR);
			//Clear the input buffer
			Sleep(10);
		}
		else if (JOYPAD & J_DOWN)
		{
			PutTile(SCR_1_PLANE, 0, 1, 4+cPlayer.GameType, TILE_EMPTY);
			cPlayer.GameType++;
			if (cPlayer.GameType>2) cPlayer.GameType=0;
			PutTile(SCR_1_PLANE, 0, 1, 4+cPlayer.GameType, TILE_CURSOR);
			Sleep(10);
		}

	}

	//Clear the input buffer again
	while(JOYPAD & J_A);

	if (cPlayer.GameType==SAMEGAME_CLASSIC)
	{
		//Select number of colours...
		PrintString(SCR_1_PLANE, 0, 2, 2, "Select difficulty");
		PrintString(SCR_1_PLANE, 0, 2, 4, "3 Colours (easy)");
		PrintString(SCR_1_PLANE, 0, 2, 5, "4 Colours");
		PrintString(SCR_1_PLANE, 0, 2, 6, "5 Colours");
		PrintString(SCR_1_PLANE, 0, 2, 7, "6 Colours (hard)");

		//Create a cursor...
		cPlayer.Colours=3;
		PutTile(SCR_1_PLANE, 0, 1, 4+cPlayer.GameType, TILE_EMPTY);
		PutTile(SCR_1_PLANE, 0, 1, 1+cPlayer.Colours, TILE_CURSOR);

		while (!(JOYPAD & J_A))
		{
			//Do stuff
			CopySpriteTile((u16*)Cursor, TILE_CURSOR, cPlayer.Animation >> 6);
			cPlayer.Animation++;
			if (JOYPAD & J_UP)
			{
				PutTile(SCR_1_PLANE, 0, 1, 1+cPlayer.Colours, TILE_EMPTY);
				cPlayer.Colours--;
				if (cPlayer.Colours>6) cPlayer.Colours=6;
				PutTile(SCR_1_PLANE, 0, 1, 1+cPlayer.Colours, TILE_CURSOR);
				//Clear the input buffer
				Sleep(10);
			}
			else if (JOYPAD & J_DOWN)
			{
				PutTile(SCR_1_PLANE, 0, 1, 1+cPlayer.Colours, TILE_EMPTY);
				cPlayer.Colours++;
				if (cPlayer.Colours>6) cPlayer.Colours=3;
				PutTile(SCR_1_PLANE, 0, 1, 1+cPlayer.Colours, TILE_CURSOR);
				Sleep(10);
			}

		}
	}
	else if (cPlayer.GameType==SAMEGAME_PUZZLE)
	{
		cPlayer.Level=0;
	}

	//Clear the input buffer again
	while(JOYPAD & J_A);

	return cPlayer;
}

void Samegame_DrawLevel(CURSOR cPlayer)
{
	u8 iLoopX;
	u8 iLoopY;
	u8 iColour;

	//Not much for now. Just randomly populate the background sprite plane with some
	//blocks

	ClearScreen(SCR_1_PLANE);
	ClearScreen(SCR_2_PLANE);

	for (iLoopX=0;iLoopX<MAX_X;iLoopX++)
	{
		for (iLoopY=0;iLoopY<MAX_Y;iLoopY++)
		{
			if (cPlayer.GameType==SAMEGAME_CLASSIC)
			{
				//Classic mode. A simple grid with 3, 4, 5 or 6 differently coloured blocks
				iColour=QRandom()/(260/cPlayer.Colours);
				PutTile(SCR_2_PLANE, PAL_BLOCK+iColour, iLoopX, iLoopY, TILE_BLOCK);
			}
			else if (cPlayer.GameType==SAMEGAME_PUZZLE)
			{
				//Puzzle mode. Get the data from a structure
				PutTile(SCR_2_PLANE, PAL_BLOCK+SameGamePuzzleData[cPlayer.Level][iLoopY][iLoopX], iLoopX, iLoopY, TILE_BLOCK);
			}
			else if (cPlayer.GameType==SAMEGAME_ENDLESS)
			{
				//Endless mode. Start with 3 colours...
				if (iLoopY>5)
				{
					iColour=QRandom()/(260/3);
					PutTile(SCR_2_PLANE, PAL_BLOCK+iColour, iLoopX, iLoopY, TILE_BLOCK);
				}
				else
				{
					PutTile(SCR_2_PLANE, 0, iLoopX, iLoopY, TILE_EMPTY);
				}
			}
			else
			{
				PrintString(SCR_1_PLANE,0, 0, 0, "Out of cheese error");
			}
		}
	}

	//Create a sprite for the cursor
	SetSprite(SPRITE_CURSOR, TILE_CURSOR, 0, 0, 0, PAL_CURSOR);

}

u8 Samegame_InBlock(u8 ScrollPlane, u16 xPosition, u16 yPosition)
{
	u8 iCurrentPalette;
	u8 iMatchPalette;
	u16 iTile;
	u8 iMatchCount;
	//Get the current Palette number
	GetTile(ScrollPlane, &iCurrentPalette, xPosition, yPosition, &iTile);
	iMatchCount=0;
	if (iTile==TILE_BLOCK)
	{
		iMatchCount=1;
		//Test the compass points
		if (yPosition>0)
		{
			//North
			GetTile(ScrollPlane, &iMatchPalette, xPosition, yPosition-1, &iTile);
			if (iTile==TILE_BLOCK)
			{
				if (iMatchPalette==iCurrentPalette)
				{
					iMatchCount++;
				}
			}
		}
		if (xPosition<MAX_X)
		{
			//East
			GetTile(ScrollPlane, &iMatchPalette, xPosition+1, yPosition, &iTile);
			if (iTile==TILE_BLOCK)
			{
				if (iMatchPalette==iCurrentPalette)
				{
					iMatchCount++;
				}
			}
		}
		if (yPosition<MAX_Y)
		{
			//South
			GetTile(ScrollPlane, &iMatchPalette, xPosition, yPosition+1, &iTile);
			if (iTile==TILE_BLOCK)
			{
				if (iMatchPalette==iCurrentPalette)
				{
					iMatchCount++;
				}
			}
		}
		if (xPosition>0)
		{
			//West
			GetTile(ScrollPlane, &iMatchPalette, xPosition-1, yPosition, &iTile);
			if (iTile==TILE_BLOCK)
			{
				if (iMatchPalette==iCurrentPalette)
				{
					iMatchCount++;
				}
			}
		}
	}
	return iMatchCount;
}

//Modify to do the animation phase...
u16 Samegame_ClearBlocksAnimated(u8 ScrollPlane, u16 xPosition, u16 yPosition, u8 Palette, u16 AnimationCount)
{
	u16 iReturnValue;
	u8 iPalette;
	u16 iTile;

	iReturnValue=0;

	//Get the current palette number?
	GetTile(ScrollPlane, &iPalette, xPosition, yPosition, &iTile);

	if (iTile==(AnimationCount) && iPalette==Palette)
	{
		iReturnValue++;
		PutTile(ScrollPlane, Palette, xPosition, yPosition, AnimationCount+1);
		if (xPosition>0)
			iReturnValue+=Samegame_ClearBlocksAnimated(ScrollPlane, xPosition-1, yPosition, Palette, AnimationCount);
		if (xPosition<MAX_X)
			iReturnValue+=Samegame_ClearBlocksAnimated(ScrollPlane, xPosition+1, yPosition, Palette, AnimationCount);
		if (yPosition>0)
			iReturnValue+=Samegame_ClearBlocksAnimated(ScrollPlane, xPosition, yPosition-1, Palette, AnimationCount);
		if (yPosition<MAX_Y)
			iReturnValue+=Samegame_ClearBlocksAnimated(ScrollPlane, xPosition, yPosition+1, Palette, AnimationCount);
	}
	return iReturnValue;
}

u16 Samegame_ClearBlocks(u8 ScrollPlane, u16 xPosition, u16 yPosition, u8 Palette)
{
	u16 iTile;
	u16 iReturnValue;
	//Stub to call Samegame_ClearBlocksAnimated() which does all the work
	iTile=TILE_BLOCK;

	iReturnValue=0;
	while (iTile<TILE_EMPTY)
	{
		iReturnValue=Samegame_ClearBlocksAnimated(ScrollPlane, xPosition, yPosition, Palette, iTile);
		//Pause just long enough to show the animation
		Sleep(5);
		//Increment the tile counter
		iTile++;
	}
	return iReturnValue;
}

void Samegame_ShowScore(CURSOR cPlayer)
{
	PrintString(SCR_1_PLANE, 0, 0, 17, "Score:");
	PrintDecimal(SCR_1_PLANE, 0, 6, 17, cPlayer.Score, 8);
}

void Samegame_Collapse(u8 ScrollPlane)
{
	u8 iLoopX;
	u8 iLoopY;
	u8 iLoopDrop;
	u16 iTileDrop;
	u8 iPaletteDrop;
	u8 iPalette;
	u16 iTile;
	u8 iCollapsed;

	//Collapse the blocks to fill any gaps...

	//This is terribly inefficient. Spends an awful long time in the inner
	//loop when it reaches the end of the game...

	//First. Collapse each column...
	for (iLoopX=0;iLoopX<MAX_X;iLoopX++)
	{
		//From the bottom up
		for (iLoopY=MAX_Y-1;iLoopY>0;iLoopY--)
		{
			//If the current cell is empty. Then swap it with the tile above...
			GetTile(ScrollPlane, &iPalette, iLoopX, iLoopY, &iTile);
			if (iTile==TILE_EMPTY)
			{
				//Find the next highest tile
				//for (iLoopDrop=iLoopY-1;iLoopDrop<MAX_Y;iLoopDrop--)
				//{
				iLoopDrop=0;
				iTileDrop=TILE_EMPTY;
				while ((iTileDrop==TILE_EMPTY) && (iLoopDrop++<iLoopY))
				{
					GetTile(ScrollPlane, &iPaletteDrop, iLoopX, iLoopY-iLoopDrop, &iTileDrop);
				}
				//Swap!
				PutTile(ScrollPlane, iPaletteDrop, iLoopX, iLoopY, iTileDrop);

				PutTile(ScrollPlane, iPalette, iLoopX, iLoopY-iLoopDrop, iTile);
				//}
			}
		}
	}
	//Secondly, collapse any empty columns...

	for (iLoopX=0;iLoopX<MAX_X-1;iLoopX++)
	{
		GetTile(ScrollPlane, &iPalette, iLoopX, 16, &iTile);
		if (iTile==TILE_EMPTY)
		{
			iCollapsed=0;
			for (iLoopDrop=iLoopX;iLoopDrop<MAX_X-1;iLoopDrop++)
			{
				//Bottom element of the column is empty.
				//Therefore, the whole column is empty.
				//Shift all subsequent columns to the Left
				for (iLoopY=0;iLoopY<MAX_Y;iLoopY++)
				{
					//Get the tile.
					GetTile(ScrollPlane, &iPalette, iLoopDrop+1, iLoopY, &iTile);
					if (iTile!=TILE_EMPTY) iCollapsed=1;
					//Move the tile
					PutTile(ScrollPlane, iPalette, iLoopDrop, iLoopY, iTile);
					PutTile(ScrollPlane, 0, iLoopDrop+1,iLoopY, TILE_EMPTY);
				}
			}
			//Bastard. Clears multiple columns, But loops infinitely when it reaches the
			//right hand side...
			if (iCollapsed) iLoopX--;
		}
	}
}

unsigned char Samegame_GameOver(CURSOR cPlayer)
{
	//Test for end of game condition
	u8 iReturn;
	u16 iLoopX;
	u16 iLoopY;

	//Default to true
	iReturn=FLAG_GAMEOVER;

	//Gameover test will vary depending on the game mode.

	if ((cPlayer.GameType==SAMEGAME_CLASSIC) || (cPlayer.GameType==SAMEGAME_PUZZLE))
	{
		//Gameover will be true when there are no "blocks" left...
		for (iLoopX=0;iLoopX<MAX_X;iLoopX++)
		{
			for (iLoopY=0;iLoopY<MAX_Y;iLoopY++)
			{
				if (Samegame_InBlock(SCR_2_PLANE, iLoopX, iLoopY)>1)
				{
					//Hoorah!
					iReturn=0;
				}
			}
		}
	}
	else if (cPlayer.GameType=SAMEGAME_ENDLESS)
	{
		//Endless games finish when the buffer overflows
		iReturn=0;
	}
	else
	{
		//Out of cheese error
		iReturn=FLAG_GAMEOVER;
	}
	return iReturn;

}

CURSOR Samegame_Move(CURSOR cPlayer)
{
	u8 iPalette;
	u16 iTile;
	u16 iScore;
	u8 iFactorial;
	u16 iLoop;

	//Move the sprites
	SetSpritePosition(SPRITE_CURSOR, (cPlayer.xPosition>>7)-4, (cPlayer.yPosition>>7)-4);
	CopySpriteTile((u16*)Cursor, TILE_CURSOR, cPlayer.Animation >> 6);

	//Get input from the player
	//Question. Do I want to make the cursor movement interia driven?
	//I think I do...
	//It would also be quite cool if we "snapped" to a grid position when we stopped...
	cPlayer.Animation++;
	cPlayer.yAcceleration=0;
	cPlayer.xAcceleration=0;
	if (JOYPAD & J_UP) cPlayer.yAcceleration=0-CURSOR_SPEED;
	if (JOYPAD & J_DOWN) cPlayer.yAcceleration=CURSOR_SPEED;
	if (JOYPAD & J_LEFT) cPlayer.xAcceleration=0-CURSOR_SPEED;
	if (JOYPAD & J_RIGHT) cPlayer.xAcceleration=CURSOR_SPEED;

	//Then update the x & y position based on the acceleration
	cPlayer.xPosition+=cPlayer.xAcceleration;
	cPlayer.yPosition+=cPlayer.yAcceleration;

	//And then snap to grid...
	//This *does* tend to snap towards the top-left...
	if (cPlayer.xAcceleration==0)
	{
		cPlayer.xPosition=((cPlayer.xPosition>>10)<<10)+512;
	}
	if (cPlayer.yAcceleration==0)
	{
		cPlayer.yPosition=((cPlayer.yPosition>>10)<<10)+512;
	}

	if ((JOYPAD & J_A) && (!(cPlayer.KeyState & J_A)))
	{
		//'A' Button pressed. Destroy the currently selected group of blocks...
		//Works, apart from the scoring and the general slowness of GetTile()
		if (Samegame_InBlock(SCR_2_PLANE, cPlayer.xPosition>>10, cPlayer.yPosition>>10)>1)
		{
			GetTile(SCR_2_PLANE, &iPalette, cPlayer.xPosition>>10, cPlayer.yPosition>>10, &iTile);
			if (iTile==TILE_BLOCK)
			{
				//Clear the blocks
				iScore=Samegame_ClearBlocks(SCR_2_PLANE, cPlayer.xPosition>>10, cPlayer.yPosition>>10, iPalette);
				Samegame_Collapse(SCR_2_PLANE);

				//Calculate the score
				for (iLoop=0;iLoop<iScore;iLoop++)
					cPlayer.Score+=(iLoop+1);

				//Game over?
				cPlayer.Flags|=Samegame_GameOver(cPlayer);

				//Print the score
				Samegame_ShowScore(cPlayer);

			}
		}
		//Wait for player to let go of the button
		cPlayer.KeyState+=J_A;
	}
	else if ((cPlayer.KeyState & J_A) && (!(JOYPAD & J_A)))
	{
		//Reset the button state
		cPlayer.KeyState-=J_A;
	}
	if ((JOYPAD & J_B) || (JOYPAD & J_OPTION))
	{
		//'B' or 'Option' Button pressed
		//Display the game control menu.
	}

	return cPlayer;
}

void Samegame_SplashScreen()
{
	u8 iLoopX;
	u8 iLoopY;
	u16 iScore;
	u8 iPalette;
	u16 iTile;
	//Display a splash screen...
	ClearScreen(SCR_1_PLANE);
	ClearScreen(SCR_2_PLANE);

	//Will do a bitmap effect at some point...

	//Quick background
	for (iLoopX=0;iLoopX<MAX_X;iLoopX++)
	{
		for (iLoopY=0;iLoopY<MAX_Y;iLoopY++)
		{
			PutTile(SCR_2_PLANE, PAL_BLOCK+SameGameLogoData[iLoopY][iLoopX], iLoopX, iLoopY, TILE_BLOCK);
		}
	}

	NeoTracker_PlayMusic(BGM_SUICIDE);

	while (!(JOYPAD & J_A))
	{
		//If the music is finished. Restart it?
	}

	NeoTracker_StopAll();

	//Clear the screen...

	GetTile(SCR_2_PLANE, &iPalette, 0, 16, &iTile);
	iScore=Samegame_ClearBlocks(SCR_2_PLANE, 0, 16, iPalette);
	Samegame_Collapse(SCR_2_PLANE);
	Sleep (50);

	GetTile(SCR_2_PLANE, &iPalette, 0, 16, &iTile);
	iScore=Samegame_ClearBlocks(SCR_2_PLANE, 0, 16, iPalette);
	Samegame_Collapse(SCR_2_PLANE);
	Sleep (50);

	GetTile(SCR_2_PLANE, &iPalette, 0, 16, &iTile);
	iScore=Samegame_ClearBlocks(SCR_2_PLANE, 0, 16, iPalette);
	Samegame_Collapse(SCR_2_PLANE);
	Sleep (50);

}