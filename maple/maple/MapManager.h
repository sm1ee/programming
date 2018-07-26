#pragma once
#include "MyHeader.h"

class MapManager{
private:
	int pos_X;
	int pos_Y;
	int size;
	RECT rc;
	HBITMAP HoldBit;
	HDC DC;
	HDC sub_dc;

public:
	void Set_Rect(){
		this->rc.left = pos_X;
		this->rc.right = pos_X + size;
		this->rc.top = pos_Y;
		this->rc.bottom = pos_Y + size;
	}

	void set_Pos(int x, int y){
		this->pos_X = x;
		this->pos_Y = y;
	}

	RECT Return_Rect(){
		return this->rc;
	}
};