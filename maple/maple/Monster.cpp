#include "Monster.h"

CMonster::CMonster() :HitFps(CLOCKS_PER_SEC / 2), AlertFps(CLOCKS_PER_SEC * 20)
{
	this->img_index = 0;
	this->Move_Range = 0;
	this->Max_Move_Range = 0;
	this->ScrollState = 0;
	this->SetLifeLength = 0;
	this->attack = 1; //아직 미 구현
	this->mAnimationScene = M_ANI::ANI_MOVE;
	this->mDirection = M_DIR::DIR_LEFT;
	this->back_size = BACK_SIZE::STAGE1SIZE;
	this->sub_dc = CreateCompatibleDC(Obj::dc);
	this->LifeEffect.SetEffect(CEffect::EFFECT::LIFE);
	memset(&this->mState, 0, sizeof(this->mState));
}

CMonster::~CMonster()
{
	DeleteDC(this->sub_dc);
}


void CMonster::Set_Init(int _speed, int _hp, std::string _identify){
	this->speed = _speed;
	this->life = _hp;
	this->SetLifeLength = _hp;
	if (!_identify.compare("Mushroom"))
		this->mType = M_TYPE::MUSHROOM;
	else if (!_identify.compare("Zombie_Mushroom")){
		this->mType = M_TYPE::BOSS_MUSHROOM;
	}
	else{
		this->mType = M_TYPE::SLIM;
		_identify = "Slim"; //예외처리
	}

	std::string t_state[M_ANI::ANI_MAX] = { "move", "hit", "die", "attack" };
	std::string t_dir[M_DIR::DIR_MAX] = { "left", "right" };
	std::string dir;
	for (int j = M_DIR::DIR_LEFT; j < M_DIR::DIR_MAX; j++){
		for (int i = M_ANI::ANI_MOVE; i < M_ANI::ANI_MAX; i++){
			dir = "./Image/Monster/" + _identify + "/" + t_dir[j] + "/" + t_state[i];
			this->imgVecMonster[j][i] = ImgManager::Get_ImgManager()->setSprite(dir.c_str());
		}
	}

	this->Set_Back_Size(BACK_SIZE::STAGE1SIZE); //임시

	int t_resize = 0;

	switch (mType){
	case M_TYPE::MUSHROOM:
		this->Max_Move_Range = 150;
		t_resize = 0;
		break;
	case M_TYPE::SLIM:
		this->Max_Move_Range = 200;
		t_resize = 20;
		break;
	case M_TYPE::BOSS_MUSHROOM:
		this->Max_Move_Range = 1000; //임시
		t_resize = 0;
		break;
	}

	POINT Monster_Size = { (*imgVecMonster[mDirection][mAnimationScene])[img_index].width, (*imgVecMonster[mDirection][mAnimationScene])[img_index].height + t_resize };

	POINT Bullect_Size = Monster_Size;
	Bullect = new CBullect(CBullect::USER_TYPE::MONSTER, Monster_Size, Bullect_Size);
	this->Bullect->SetBullectActive();

	img_index = rand() % (*imgVecMonster[mDirection][mAnimationScene]).size();
	mDirection = (M_DIR)(rand() % M_DIR::DIR_MAX);

}
void CMonster::Set_Pos(int _posX, int _posY){

	this->posX = _posX;
	this->posY = _posY;
	//this->Init_PosX = this->posX = _posX;
}

void CMonster::Set_Back_Size(BACK_SIZE _back_size){
	this->back_size = _back_size;

	//switch (this->mType){
	//case M_TYPE::SLIM:
	//	break;
	//case M_TYPE::MUSHROOM:
	//	break;
	//case M_TYPE::BOSS_MUSHROOM:
	//	break;
	//}
	switch (_back_size){
	case BACK_SIZE::STAGE1SIZE:
		this->posX = rand() % (BACK_SIZE::STAGE1SIZE - BACK_SIZE::STAGE1SIZE / 3) + BACK_SIZE::STAGE1SIZE / 3;
		this->posY = WINSIZEY - 35 - (*imgVecMonster[mDirection][mAnimationScene])[img_index].height;
	break;
	case BACK_SIZE::STAGE2SIZE:
		this->posX = rand() % (BACK_SIZE::STAGE2SIZE - BACK_SIZE::STAGE2SIZE / 3) + BACK_SIZE::STAGE2SIZE / 3;
		this->posY = WINSIZEY - 35 - (*imgVecMonster[mDirection][mAnimationScene])[img_index].height;
		break;
}
}

POINT CMonster::Get_Pos(){
	POINT MonsterPos = { posX, posY };
	return MonsterPos;
}

void CMonster::Get_Player_Pos(POINT PlayerPos){
	if (mState.IsAlert && !mState.IsHit &&!img_index){
		mDirection = this->posX < PlayerPos.x ? M_DIR::DIR_RIGHT : M_DIR::DIR_LEFT;
		this->Effect.SetDirection(mDirection == M_DIR::DIR_LEFT ? CEffect::EFFECT_DIRECTION::DIR_LEFT : CEffect::EFFECT_DIRECTION::DIR_RIGHT);
	}
}
bool CMonster::Get_IsDeath(){
	return this->mState.IsDeath;
}
void CMonster::Update(){

	POINT _pt = { posX, posY };
	this->Bullect->SetUserPos(_pt);

	if (AlertFps.getFps() && this->mState.IsAlert)
		this->AlertFps = false;

	if (life < 1){
		if (this->mAnimationScene == M_ANI::ANI_DIE){
			if (!this->img_index)
				this->mState.IsDeath = true;
		}
		else {
			this->Bullect->SetDeath();
			this->mAnimationScene = M_ANI::ANI_DIE;
			this->img_index = 0;
			this->life--;
		}
	}
	else if (HitFps.getFps() && this->mState.IsHit){
		this->mState.IsHit = false;
		this->mAnimationScene = M_ANI::ANI_MOVE;
		this->img_index = 0;
	}
	else if (Bullect->GetCollisionState() && !this->mState.IsHit){ //맞았을 때
		this->life--;
		this->mState.IsHit = true;
		this->mState.IsAlert = true;
		this->HitFps.SetStart();
		posX = mDirection == M_DIR::DIR_LEFT ? posX += 10 : posX -= 10;
		this->Effect.SetDirection(mDirection == M_DIR::DIR_LEFT ? CEffect::EFFECT_DIRECTION::DIR_LEFT : CEffect::EFFECT_DIRECTION::DIR_RIGHT);
		this->Effect.SetEffect(CEffect::EFFECT::MAGIC_CLAW_HIT);
		this->mAnimationScene = M_ANI::ANI_HIT;
		this->img_index = 0;
	}

	Move();
}
void CMonster::Set_Scroll_State(int _ScrollState){
	this->ScrollState += _ScrollState;
}
void CMonster::Move(){

	if (!this->mState.IsHit){
		if (this->mType == M_TYPE::MUSHROOM){
			if (img_index == 1){
				posX = mDirection == M_DIR::DIR_LEFT ? posX -= speed : posX += speed;
				Move_Range += speed;
			}

		}
		else if (this->mType == M_TYPE::SLIM){
			if (img_index > 1 && img_index < 5){
				posX = mDirection == M_DIR::DIR_LEFT ? posX -= speed : posX += speed;
				Move_Range += speed;
			}
		}
		else if (this->mType == M_TYPE::BOSS_MUSHROOM){


		}
	}


	if ((Move_Range > Max_Move_Range) && !mState.IsAlert){
		mDirection = mDirection == M_DIR::DIR_LEFT ? M_DIR::DIR_RIGHT : M_DIR::DIR_LEFT;
		this->Effect.SetDirection(mDirection == M_DIR::DIR_LEFT ? CEffect::EFFECT_DIRECTION::DIR_LEFT : CEffect::EFFECT_DIRECTION::DIR_RIGHT);
		Move_Range = 0;
	}

	if (this->posX + ScrollState < 0 && !img_index){
		this->posX = 0;
		mDirection = M_DIR::DIR_RIGHT;
		this->Effect.SetDirection(CEffect::EFFECT_DIRECTION::DIR_RIGHT);
		Move_Range = 0;
	}
	else if (this->posX + ScrollState + (*imgVecMonster[mDirection][mAnimationScene])[img_index].height > this->back_size && !img_index){
		this->posX = back_size - ScrollState - (*imgVecMonster[mDirection][mAnimationScene])[img_index].height;
		mDirection = M_DIR::DIR_LEFT;
		this->Effect.SetDirection(CEffect::EFFECT_DIRECTION::DIR_LEFT);
		Move_Range = 0;
	}
}

void CMonster::Update_Next_Image(){
	if ((unsigned)++img_index >= (*imgVecMonster[mDirection][mAnimationScene]).size())
		img_index = 0;

	Effect.Update_Next_Image();
}

void CMonster::Draw(){

	POINT _pt = { posX, posY };

	HBITMAP hOldBit = (HBITMAP)SelectObject(sub_dc, (*imgVecMonster[mDirection][mAnimationScene])[img_index].bitmap);
	TransparentBlt(Obj::dc, posX, posY, (*imgVecMonster[mDirection][mAnimationScene])[img_index].width, (*imgVecMonster[mDirection][mAnimationScene])[img_index].height, sub_dc, 0, 0, (*imgVecMonster[mDirection][mAnimationScene])[img_index].width, (*imgVecMonster[mDirection][mAnimationScene])[img_index].height, RGB(163, 73, 164));
	SelectObject(sub_dc, hOldBit);

	if (this->mState.IsHit || this->mAnimationScene == M_ANI::ANI_DIE){
		POINT _life_pt = { posX - 3, posY + (*imgVecMonster[mDirection][mAnimationScene])[img_index].height };
		POINT _life_Size = LifeEffect.GetNowImageSize(0); //라이프
		LifeEffect.SetEffectPos(_life_pt);
		LifeEffect.Draw(1);
		LifeEffect.Draw(0, (_life_Size.x / this->SetLifeLength * life), _life_Size.y);
	}
	if (this->mState.IsHit && life >= 0){
		switch (this->mType){
		case M_TYPE::MUSHROOM:
			break;
		case M_TYPE::SLIM:
			_pt.y += 20;
			break;
		case M_TYPE::BOSS_MUSHROOM:
			break;
		}
		Effect.SetEffectPos(_pt);
		this->Effect.Draw();
		
		//this->Effect.Draw(3);
		//this->Effect.Draw(3);
	}
}