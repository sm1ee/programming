#pragma once
#include "MyHeader.h"
#include "ImgManager.h"

class Obj
{
protected:
	int posX;
	int posY;
	int life;
	int speed;
	int attack;
	int img_index;
	BACK_SIZE back_size;
	static HDC dc;
	HBITMAP bitmap;
	HDC sub_dc;
public:

	static void Set_Dc(HDC _dc){
		Obj::dc = _dc;
	}
	virtual void Set_Back_Size(BACK_SIZE _back_size){};
	virtual void Input_State(SHORT){}
	virtual void Set_Init(int, int, std::string) = 0;
	virtual void Set_Pos(int, int = 0) = 0;
	virtual void Draw() = 0;
	virtual void Update() = 0;
	virtual void Move() = 0;
	virtual void Update_Next_Image() = 0;
	virtual POINT Get_Pos() = 0;
	virtual bool Get_IsDeath(){ return false; }
	Obj(){
		this->posX = 0;
		this->posY = 0;
		this->life = 0;
		this->speed = 0;
	}
	virtual ~Obj(){}
};

