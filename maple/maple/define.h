#pragma once
#include <vector>
#include <Windows.h>
#define WINSIZEX 1024
#define WINSIZEY 720
#define MAIN_DELAY 250
#define INPUT_DELAY 20
//#define STAGE1SIZEX 1680
//#define MOUSE_DELAY 50

enum OBJ_TYPE {
	PLAYER,
	MONSTER,
	NPC,
	OBJ_MAX
};

enum STAGE_TYPE{
	STAGE1,
	STAGE2,
	MAX_SIZE
};

typedef enum BACKGROUND_ENDLINE_STATE{
	NOT_ENDLINE,
	LEFT_ENDLINE,
	RIGHT_ENDLINE,
}BACK_STATE;

typedef enum BACKGROUND_SIZE{
	STAGE1SIZE = 1680,
	STAGE2SIZE
}BACK_SIZE;

struct ImageData{
	HBITMAP bitmap;
	int width;
	int height;
	char name[100];
};

typedef enum PLAYER_STATE {
	STATE_DEFAULT = 0x00,
	STATE_ALERT = 1 << 1,
	STATE_ATTACK = 'Z',
	STATE_INVEN = 'I',
	STATE_JUMP = VK_SPACE,
	STATE_LMOVE = VK_LEFT,
	STATE_RMOVE = VK_RIGHT,
	STATE_SKILL = VK_SHIFT,
	STATE_DIE,
} KEY_STATE;

typedef std::vector<ImageData> IMG_VEC;