#pragma once
#include "Obj.h"
#include "InventoryManager.h"
#include "Effect.h"
#include "Fps.h"
#include "Bullet.h"
class CPlayer :
	public Obj
{
private:
	typedef enum PLAYER_TYPE{
		MAN,
		WOMAN
	}P_TYPE;
	typedef enum PLAYER_DIR{
		DIR_LEFT,
		DIR_RIGHT,
		DIR_MAX
	}P_DIR;

	typedef enum PLAYER_ANIMATION {
		ANI_DEFAULT,
		ANI_ALERT,
		ANI_ATTACK,
		ANI_SKILL,
		ANI_JUMP,
		ANI_MOVE,
		ANI_DIE,
		ANI_MAX
	}P_ANI;

	typedef struct PlayerState{
		bool IsOpenInven;
		bool IsJump;
		bool IsAlert;
		bool IsAttack;
		bool IsSkill;
		bool IsMove;
		bool IsHit;
		bool IsInputDelay;
	}P_STATE;
	IMG_VEC *imgVecPlayer[P_DIR::DIR_MAX][P_ANI::ANI_MAX];
	P_STATE pState;
	P_ANI pAnimationScene;
	P_DIR pDirection;
	P_TYPE pType;
	CBullect *Bullect;
	CEffect Effect;
	CEffect LifeEffect;
	CFps AlertFps;
	CFps HitFps;
	CFps InputDelayFps;
	InventoryManager* Inven_Manager;
	int Acceleration;
	int SetLifeLength;
	SHORT Key_Value;
public:
	void Set_Init(int _speed, int _hp, std::string _identify);
	void Set_Pos(int _posX, int _posY = 0);
	void Set_Back_Size(BACK_SIZE _back_size);
	void Move();
	void Draw();
	void Update();
	void Update_Next_Image();
	void Input_State(SHORT );
	int BackDirUpdate(BACK_STATE _IsEndLine);
	POINT Get_Pos();
	CPlayer();
	~CPlayer();
};

