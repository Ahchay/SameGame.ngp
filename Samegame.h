#define SameGameTileBase 128

#define SPRITE_CURSOR 0
#define TILE_CURSOR 128
#define TILE_BLOCK 132
#define TILE_EMPTY 137
#define PAL_CURSOR 0
#define PAL_BLOCK 1

#define BLOCK_FRAMECOUNT 4

#define CURSOR_SPEED 2
#define MAX_ACCELERATION 16

//Might change the cursor movement to be momentum driven
typedef struct CursorSprite
{
	u16 xPosition;
	u16 yPosition;
	s8 xAcceleration;
	s8 yAcceleration;
	u16 Rotation;
	u8 GameType;
	u8 Colours;
	u8 Level;
	u16 Timer;
	u8 TimerIncrement;
	u8 Lives;
	u16 Score;
	u8 Flags;
	u8 Animation;
	u8 KeyState;
} CURSOR;

#define MAX_LEVEL 1

#define FLAG_GAMEOVER 1

#define MAX_Y 17
#define MAX_X 20

#define SAMEGAME_CLASSIC 0
#define SAMEGAME_PUZZLE 1
#define SAMEGAME_ENDLESS 2

