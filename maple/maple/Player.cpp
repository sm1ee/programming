#include "Player.h"
#define MAX_ACCEL 14

CPlayer::CPlayer() :AlertFps(CLOCKS_PER_SEC * 10), HitFps(CLOCKS_PER_SEC * 2), InputDelayFps(CLOCKS_PER_SEC / 2){
	this->img_index = 0;
	this->Key_Value = 0;
	this->attack = 1;
	
	this->pAnimationScene = P_ANI::ANI_DEFAULT;
	this->back_size = BACK_SIZE::STAGE1SIZE;
	this->pDirection = P_DIR::DIR_RIGHT;
	this->Inven_Manager = InventoryManager::Get_InventoryManager();
	this->Effect.SetDirection(CEffect::E_DIR::DIR_RIGHT);
	this->sub_dc = CreateCompatibleDC(Obj::dc);
	this->LifeEffect.SetEffect(CEffect::EFFECT::LIFE);
	this->Acceleration = MAX_ACCEL;
	memset(&pState, false, sizeof(pState));
}


CPlayer::~CPlayer()
{
	this->Bullect->SetDeath();
	DeleteDC(this->sub_dc);
}


void CPlayer::Set_Init(int _speed, int _hp, std::string _identify){
	this->speed = _speed;
	this->life = _hp;
	this->SetLifeLength = _hp;
	if (!_identify.compare("Boy"))
		this->pType = P_TYPE::MAN;
	else{
		this->pType = P_TYPE::WOMAN;
		_identify = "Girl"; //예외처리
	}

	std::string t_state[P_ANI::ANI_MAX] = { "default", "alert", "attack", "skill", "jump", "move", "die" };
	std::string t_dir[P_DIR::DIR_MAX] = { "left", "right" };
	std::string dir;
	for (int j = P_DIR::DIR_LEFT; j < P_DIR::DIR_MAX; j++){
		for (int i = P_ANI::ANI_DEFAULT; i < P_ANI::ANI_MAX; i++){
			dir = "./Image/Player/" + _identify + "/" + t_dir[j] + "/" + t_state[i];
			this->imgVecPlayer[j][i] = ImgManager::Get_ImgManager()->setSprite(dir.c_str());
		}
	}
	this->Set_Back_Size(BACK_SIZE::STAGE1SIZE); //임시

	POINT Player_Size = { (*imgVecPlayer[pDirection][pAnimationScene])[img_index].width, (*imgVecPlayer[pDirection][pAnimationScene])[img_index].height };
	POINT Bullect_Size;
	if (this->pType == P_TYPE::MAN){
		Bullect_Size = { 80, (*imgVecPlayer[pDirection][pAnimationScene])[img_index].height };
		Bullect = new CBullect(CBullect::USER_TYPE::MAN, Player_Size, Bullect_Size);
	}
	else if (this->pType == P_TYPE::WOMAN){
		Bullect_Size = { 200, (*imgVecPlayer[pDirection][pAnimationScene])[img_index].height };
		Bullect = new CBullect(CBullect::USER_TYPE::WOMAN, Player_Size, Bullect_Size);
	}

}

void CPlayer::Set_Pos(int _posX, int _posY){
	if (!_posY)
		this->posY = WINSIZEY - 35 - (*imgVecPlayer[pDirection][P_ANI::ANI_DEFAULT])[img_index].height;
	else
		this->posY = _posY;
	this->posX = _posX;
}

void CPlayer::Set_Back_Size(BACK_SIZE _back_size){

	this->back_size = _back_size;

	switch (_back_size){

	case BACK_SIZE::STAGE1SIZE:
		this->posX = 20;
		this->posY = WINSIZEY - 35 - (*imgVecPlayer[pDirection][pAnimationScene])[img_index].height;
		if(pType == P_TYPE::MAN)
			posY += 10;
		break;
		break;
	case BACK_SIZE::STAGE2SIZE:
		this->posX = 20;
		this->posY = WINSIZEY - 35 - (*imgVecPlayer[pDirection][pAnimationScene])[img_index].height;
		break;
	}
}

int CPlayer::BackDirUpdate(BACK_STATE _IsEndLine){
	int Move = 0;
	int _posX = posX + (*imgVecPlayer[pDirection][pAnimationScene])[img_index].width / 2;

	POINT _pt = { posX, posY };
	this->Bullect->SetUserPos(_pt);
	

	if (_posX < WINSIZEX / 2 && pDirection == P_DIR::DIR_LEFT && pAnimationScene != P_ANI::ANI_SKILL){
		if (_IsEndLine == BACK_STATE::LEFT_ENDLINE){
			speed = 3;
			Move = 0;
		}
		else if (pState.IsMove){
			speed = -2;
			Move = -3;
		}
		else{
			posX += 3;
			Move = -3;
		}
	}
	else if (_posX > WINSIZEX / 2 && pDirection == P_DIR::DIR_RIGHT && pAnimationScene != P_ANI::ANI_SKILL){
		if (_IsEndLine == BACK_STATE::RIGHT_ENDLINE){
			speed = 3;
			Move = 0;
		}
		else if (pState.IsMove){
			speed = -2;
			Move = 3;
		}
		else{
			posX -= 3;
			Move = 3;
		}
	}
	else if (_posX == WINSIZEX / 2 && pDirection == P_DIR::DIR_LEFT && !_IsEndLine) {
		if (pState.IsMove){
			speed = 0;
			Move = -2;
		}
	}
	else if (_posX == WINSIZEX / 2 && pDirection == P_DIR::DIR_RIGHT && !_IsEndLine){
		if (pState.IsMove){
			speed = 0;
			Move = 2;
		}
	}
	else{
		speed = 3;
		Move = 0;
	}
	return Move;
}
void CPlayer::Update(){

	if (this->InputDelayFps.getFps() && this->pState.IsInputDelay)
		this->pState.IsInputDelay = false;

	if (Bullect->GetCollisionState()){
		if (HitFps.getFps()){
			this->life--;
			if (this->pDirection == P_DIR::DIR_LEFT){
				posX += 50;
			}	
			else{
				posX -= 50;
			}
			this->img_index = 0;
			this->AlertFps.SetStart();
			this->InputDelayFps.SetStart();
			this->pState.IsInputDelay = true;
			this->pState.IsAlert = true;
			this->pState.IsHit = true;
			this->pState.IsMove = false;
			this->pState.IsSkill = false;
			this->pState.IsJump = false;
			this->pAnimationScene = P_ANI::ANI_ALERT;
		}
	}
	if (pAnimationScene == P_ANI::ANI_ATTACK && (img_index == 1 || img_index == 3)){ //애니메이션은 공격하는 것 처럼 보이지만
		this->pState.IsAttack = true;  					    				//실제 데미지를 입히는 총알 생성
		this->Bullect->SetBullectActive();
	}
	if (pAnimationScene == P_ANI::ANI_SKILL && img_index == 1){
		this->Effect.SetEffect(CEffect::EFFECT::TELEPORT);
		this->pState.IsSkill = true;
		Move();
	}
}
void CPlayer::Move() {
	//예외처리 및 이동

	if (posY + (*imgVecPlayer[pDirection][pAnimationScene])[img_index].height < WINSIZEY - 36 && !this->pState.IsJump){
		this->pState.IsJump = true;
		this->img_index = 0;
		this->Acceleration = 0;
		this->pAnimationScene = P_ANI::ANI_JUMP;
	}
	else if (this->pState.IsJump){
		this->posY -= this->Acceleration;
		Acceleration--;
			if (posY + (*imgVecPlayer[pDirection][pAnimationScene])[img_index].height >= WINSIZEY - 34){
				this->posY = WINSIZEY - 35 - (*imgVecPlayer[pDirection][pAnimationScene])[img_index].height;
				this->pAnimationScene = P_ANI::ANI_DEFAULT;
				this->pState.IsJump = false;
				this->pState.IsMove = false;
				this->pState.IsHit = false;
			}
	}
	if (this->pState.IsSkill && !this->pState.IsJump)
		this->posX = (this->pDirection == P_DIR::DIR_LEFT) ? posX - 200 : posX + 200;
	if (this->pState.IsJump && this->pState.IsMove)
		this->posX = (this->pDirection == P_DIR::DIR_LEFT) ? posX - speed - 2 : posX + speed + 2;
	else if (this->pState.IsMove)
		this->posX = this->pDirection == P_DIR::DIR_LEFT ? posX - this->speed : posX + this->speed;
}
void CPlayer::Input_State(SHORT _KeyValue){

	if (!this->pState.IsInputDelay){
		this->pState.IsHit = false;
		switch (_KeyValue){
		case KEY_STATE::STATE_INVEN:
			pState.IsOpenInven ^= 1;
			break;
		case KEY_STATE::STATE_DEFAULT:
			
			if((this->pAnimationScene != P_ANI::ANI_ATTACK && this->pAnimationScene != P_ANI::ANI_SKILL)|| img_index != 0)
			this->pState.IsMove = false;
			this->pState.IsSkill = false;
			this->pState.IsAttack = false;
			
			if (this->pAnimationScene == P_ANI::ANI_ATTACK){
				this->pAnimationScene = P_ANI::ANI_ALERT;
				this->img_index = 0;
				AlertFps.SetStart();
			}
			else if (this->pState.IsJump)
				this->pAnimationScene = P_ANI::ANI_JUMP;
			else if (this->pState.IsAlert){
				//fps 매니저 이용해서 5초 정도 랜더링
				if (AlertFps.getFps()){
					this->pAnimationScene = P_ANI::ANI_DEFAULT;
					this->pState.IsAlert = false;
					this->img_index = 0;
				}
				else
					this->pAnimationScene = P_ANI::ANI_ALERT;
			}
			else
				this->pAnimationScene = P_ANI::ANI_DEFAULT;
			this->Move();
			break;
		case KEY_STATE::STATE_LMOVE:
			if (this->pAnimationScene != P_ANI::ANI_MOVE || this->pDirection != P_DIR::DIR_LEFT){
				if (!this->pState.IsJump)
					this->pAnimationScene = P_ANI::ANI_MOVE;
				this->pDirection = P_DIR::DIR_LEFT;
				this->Bullect->SetDirection(CBullect::USER_DIR::DIR_LEFT);
				this->Effect.SetDirection(CEffect::E_DIR::DIR_LEFT);
				this->img_index = 0;
				this->pState.IsMove = true;
				this->pState.IsSkill = false;
				this->pState.IsAttack = false;
			}
			this->Move();
			break;
		case KEY_STATE::STATE_RMOVE:
			if (this->pAnimationScene != P_ANI::ANI_MOVE || this->pDirection != P_DIR::DIR_RIGHT){
				if (!this->pState.IsJump)
					this->pAnimationScene = P_ANI::ANI_MOVE;
				this->img_index = 0;
				this->pDirection = P_DIR::DIR_RIGHT;
				this->Bullect->SetDirection(CBullect::USER_DIR::DIR_RIGHT);
				this->Effect.SetDirection(CEffect::E_DIR::DIR_RIGHT);
				this->pState.IsMove = true;
				this->pState.IsSkill = false;
				this->pState.IsAttack = false;
			}
			this->Move();
			break;
		case KEY_STATE::STATE_JUMP:
			if (!this->pState.IsJump){
				this->Acceleration = MAX_ACCEL;
				this->img_index = 0;
				//this->Update();
				this->pState.IsJump = true;
				this->pState.IsSkill = false;
				this->pState.IsAttack = false;
				this->pAnimationScene = P_ANI::ANI_JUMP;
			}
			this->Move();
			break;
		case KEY_STATE::STATE_ATTACK:
			if (this->pAnimationScene != P_ANI::ANI_ATTACK && !this->pState.IsJump){

				if (pType == P_TYPE::WOMAN)
					this->Effect.SetEffect(CEffect::EFFECT::MAGIC_CLAW_EFFECT);
				else
					this->Effect.SetEffect(CEffect::EFFECT::SWORD);
				this->pAnimationScene = P_ANI::ANI_ATTACK;
				this->pState.IsAlert = true;
				this->pState.IsMove = false;
				this->pState.IsSkill = false;
				this->pState.IsAttack = false;
				this->img_index = 0;
			}
			else if (this->pState.IsAlert)
				AlertFps.SetStart();
			this->Move();
			break;
		case KEY_STATE::STATE_SKILL:
			if (this->pAnimationScene != P_ANI::ANI_SKILL && pType == P_TYPE::WOMAN && !this->pState.IsJump){
				this->pAnimationScene = P_ANI::ANI_SKILL;
				this->pState.IsAlert = true;
				this->pState.IsMove = false;
				this->pState.IsSkill = false;
				this->pState.IsAttack = false;
				this->img_index = 0;
			}
			else if (!this->pState.IsSkill)
				this->Move();
			else if (this->pState.IsAlert)
				AlertFps.SetStart();
			break;
		}
	}
	else
		Move();
}

void CPlayer::Update_Next_Image(){
	if ((unsigned)++img_index >= (*imgVecPlayer[pDirection][pAnimationScene]).size())
		img_index = 0;

	Effect.Update_Next_Image();
}

void CPlayer::Draw(){

	POINT _pt = { posX, posY };
	if (this->posX < 0)
		posX = 0;
	else if (this->posX + (*imgVecPlayer[pDirection][pAnimationScene])[img_index].width > WINSIZEX)
		posX = WINSIZEX - (*imgVecPlayer[pDirection][pAnimationScene])[img_index].width;

	HBITMAP hOldBit = (HBITMAP)SelectObject(sub_dc, (*imgVecPlayer[pDirection][pAnimationScene])[img_index].bitmap);
	TransparentBlt(Obj::dc, posX, posY, (*imgVecPlayer[pDirection][pAnimationScene])[img_index].width, (*imgVecPlayer[pDirection][pAnimationScene])[img_index].height, sub_dc, 0, 0, (*imgVecPlayer[pDirection][pAnimationScene])[img_index].width, (*imgVecPlayer[pDirection][pAnimationScene])[img_index].height, RGB(163, 73, 164));
	SelectObject(sub_dc, hOldBit);

	if (this->pState.IsHit){
		POINT _life_pt = { posX - 3, posY + (*imgVecPlayer[pDirection][pAnimationScene])[0].height };
		POINT _life_Size = LifeEffect.GetNowImageSize(0);
		this->LifeEffect.SetEffectPos(_life_pt);
		LifeEffect.Draw(1);
		LifeEffect.Draw(0, (_life_Size.x / this->SetLifeLength * life), _life_Size.y);
	}
	switch (pAnimationScene){
	case P_ANI::ANI_SKILL:
		///if(img_index != 0)
		_pt.y -= 40;
		if (!this->pState.IsSkill)
			Effect.SetEffectPos(_pt);
		else if (img_index == 0)
			this->pState.IsSkill = false;
		else
			Effect.Draw();
		break;
	case P_ANI::ANI_ATTACK:
		Effect.SetEffectPos(_pt);
		Effect.Draw();
		break;
	default:
		break;
	}

	if (this->pState.IsOpenInven)
		Inven_Manager->Draw();
}

POINT CPlayer::Get_Pos(){
	POINT temp;
	temp.x = this->posX;
	temp.y = this->posY;
	return temp;
}

