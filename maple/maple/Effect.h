#pragma once
#include "MyHeader.h"
#include "ImgManager.h"
class CEffect {
public:
	enum EFFECT{
		MAGIC_CLAW_EFFECT,
		MAGIC_CLAW_HIT,
		SWORD,
		TELEPORT,
		LIFE,
		ZOMBIE_MUSHROOM,
		MAX_SIZE
	};
	typedef enum EFFECT_DIRECTION{
		DIR_LEFT,
		DIR_RIGHT,
		DIR_MAX_SIZE
	}E_DIR;
private:
	static IMG_VEC* CEffect::ImgVecEffect[E_DIR::DIR_MAX_SIZE][EFFECT::MAX_SIZE];
	static HDC Mem_DC;
	HDC sub_dc;
	E_DIR eDirection;
	EFFECT effect;
	POINT Effect_Pos;
	int img_index;
public:
	static void Set_Init(HDC _dc){
		CEffect::Mem_DC = _dc;
		std::string effect_dir[EFFECT::MAX_SIZE] = { "magic_claw_effect", "magic_claw_hit", "sword", "teleport", "life", "Zombie_Mushroom" };
		std::string direction[E_DIR::DIR_MAX_SIZE] = { "left", "right" };
		std::string dir;
		for (int j = E_DIR::DIR_LEFT; j < E_DIR::DIR_MAX_SIZE; j++){
			for (int i = EFFECT::MAGIC_CLAW_EFFECT; i < EFFECT::MAX_SIZE; i++){
				dir = "./Image/effect/" + direction[j] + "/" + effect_dir[i];
				CEffect::ImgVecEffect[j][i] = ImgManager::Get_ImgManager()->setSprite(dir.c_str());
			}
		}
	}

	CEffect(){
		this->img_index = 0;
		this->eDirection = E_DIR::DIR_LEFT;
		this->effect = EFFECT::MAGIC_CLAW_EFFECT;
		this->sub_dc = CreateCompatibleDC(CEffect::Mem_DC);
		this->Effect_Pos = { 0, 0 };
	}
	~CEffect(){
		DeleteDC(this->sub_dc);
	}
	void SetDirection(const E_DIR& _dir){
		this->eDirection = _dir;
		this->img_index = 0;
	}
	void SetEffect(const EFFECT& _effect){
		this->effect = _effect;
		this->img_index = 0;
	}
	void SetEffectPos(const POINT& _pt){
		this->Effect_Pos = _pt;
	}
	POINT GetNowImageSize(int index){
		POINT _pt = { (*CEffect::ImgVecEffect[eDirection][effect])[index].width, (*CEffect::ImgVecEffect[eDirection][effect])[index].height };
		return _pt;
	}
	void Update_Next_Image(){
		if ((unsigned)++img_index >= (*CEffect::ImgVecEffect[eDirection][effect]).size())
			this->img_index = 0;
	}
	void Draw(){
		HBITMAP HoldBit = (HBITMAP)SelectObject(sub_dc, (*CEffect::ImgVecEffect[eDirection][effect])[img_index].bitmap);
		TransparentBlt(Mem_DC, Effect_Pos.x, Effect_Pos.y, (*CEffect::ImgVecEffect[eDirection][effect])[img_index].width, (*CEffect::ImgVecEffect[eDirection][effect])[img_index].height, sub_dc, 0, 0, (*CEffect::ImgVecEffect[eDirection][effect])[img_index].width, (*CEffect::ImgVecEffect[eDirection][effect])[img_index].height, RGB(163, 73, 164));
		SelectObject(sub_dc, HoldBit);
	}

	void Draw(int img_index, int sizeX = 0, int sizeY = 0){
		
		if (!sizeX && !sizeY){
			sizeX = (*CEffect::ImgVecEffect[eDirection][effect])[img_index].width;
			sizeY = (*CEffect::ImgVecEffect[eDirection][effect])[img_index].height;
		}
		HBITMAP HoldBit = (HBITMAP)SelectObject(sub_dc, (*CEffect::ImgVecEffect[eDirection][effect])[img_index].bitmap);
		TransparentBlt(Mem_DC, Effect_Pos.x, Effect_Pos.y, sizeX, sizeY, sub_dc, 0, 0, (*CEffect::ImgVecEffect[eDirection][effect])[img_index].width, (*CEffect::ImgVecEffect[eDirection][effect])[img_index].height, RGB(163, 73, 164));
		SelectObject(sub_dc, HoldBit);
	}
};