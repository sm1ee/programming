#pragma once
#include "Obj.h"
#include "Effect.h"
#include "Bullet.h"
#include "Fps.h"

class CMonster :
	public Obj
{

private:
	typedef enum MONSTER_TYPE{
		MUSHROOM,
		SLIM,
		BOSS_MUSHROOM,
	}M_TYPE;
	typedef enum MONSTER_DIR {
		DIR_LEFT,
		DIR_RIGHT,
		DIR_MAX
	}M_DIR;
	typedef enum MONSTER_ANIMATON {
		ANI_MOVE,
		ANI_HIT,
		ANI_DIE,
		ANI_ATTACK,
		ANI_MAX,
	}M_ANI;

	struct MonsterState{
		//bool IsJump;
		bool IsAttack;
		//bool IsSkill;
		bool IsMove;
		bool IsHit;
		bool IsDeath;
		bool IsAlert;
		//bool IsInputDelay;
	};
	IMG_VEC *imgVecMonster[M_DIR::DIR_MAX][M_ANI::ANI_MAX];
	M_ANI mAnimationScene; //현재 그릴 이미지를 결정 상태와 이미지가 분리 되어있음
	MonsterState mState;
	M_DIR mDirection;
	M_TYPE mType;
	CBullect *Bullect;
	CEffect Effect;
	CEffect LifeEffect;
	CFps HitFps;
	CFps AlertFps;
	int Max_Move_Range;
	int Move_Range;
	//int Init_PosX;
	int ScrollState;
	int SetLifeLength;

public:
	void Set_Init(int _speed, int _hp, std::string _identify);
	void Set_Pos(int _posX, int _posY = 0);
	void Set_Back_Size(BACK_SIZE);
	void Set_Scroll_State(int ScrollState);
	void Get_Player_Pos(POINT PlayerPos);
	void Draw();
	void Move();
	void Update();
	void Update_Next_Image();
	bool Get_IsDeath();
	POINT Get_Pos();
	CMonster();
	~CMonster();
};