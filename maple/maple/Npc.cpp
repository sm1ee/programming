#include "Npc.h"

void CNpc::Set_Init(int _speed, int _hp, int _identify){


}
void CNpc::Set_Pos(int _posX, int _posY){
	this->posX = _posX;
	this->posY = _posY;
};

POINT CNpc::Get_Pos(){
	POINT temp;
	temp.x = this->posX;
	temp.y = this->posY;
	return temp;
}

void CNpc::Draw(){}

CNpc::CNpc()
{

}


CNpc::~CNpc()
{

}