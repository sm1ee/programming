#pragma once
#include "Obj.h"
class CNpc : public Obj{

public:

	void Set_Init(int _speed, int _hp, int _identify);
	void Set_Pos(int _posX, int _posY);
	void Draw();
	POINT Get_Pos();
	CNpc();
	~CNpc();
};