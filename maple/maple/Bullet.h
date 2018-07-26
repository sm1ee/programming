//총알 구현
#pragma once
#include "MyHeader.h"

class CBullect{
public:
	typedef enum USER_DIRECTION{
		DIR_LEFT,
		DIR_RIGHT,
	}USER_DIR;

	enum USER_TYPE{
		MAN,
		WOMAN,
		MONSTER,
		MAX_SIZE
	};
private:
	static std::list<CBullect*> BullectVec;
	const USER_TYPE UserType;
	bool IsActive;
	bool IsCollision;
	bool IsDeath;
	const POINT BullectSize;
	const POINT UserSize;
	RECT User;
	RECT Bullect;
	USER_DIR uDirection;
public:
	static void ObjectIsDeath(){
		for (std::list<CBullect*>::iterator _iter = BullectVec.begin(); _iter != BullectVec.end();){
			if ((*_iter)->IsDeath){
				delete (*_iter);
				BullectVec.erase(_iter++);
			}
			else
				++_iter;
		}
	}
	static bool descending_compare(CBullect* param1, CBullect* param2){
		return param1->User.left > param2->User.left;
	}
	static bool ascending_compare(CBullect* param1, CBullect* param2){
		return param1->User.left < param2->User.left;
	}
	static void CollisionControl(){
		RECT tmp;
		RECT BullectPos = { 0, 0, 0, 0 };

		USER_DIR _dir = USER_DIR::DIR_LEFT;
		for (std::list<CBullect*>::iterator _iter = BullectVec.begin(); _iter != BullectVec.end(); ++_iter){
			if ((*_iter)->UserType == USER_TYPE::MAN || (*_iter)->UserType == USER_TYPE::WOMAN){
				_dir = (USER_DIR)(*_iter)->uDirection; //일단 한캐릭
				break;
			}
		}
		if (_dir == USER_DIR::DIR_LEFT)
			BullectVec.sort(descending_compare);
		else if (_dir == USER_DIR::DIR_RIGHT)
			BullectVec.sort(ascending_compare);

		for (std::list<CBullect*>::iterator src_it = BullectVec.begin(); src_it != BullectVec.end(); ++src_it){
			for (std::list<CBullect*>::iterator dest_it = BullectVec.begin(); dest_it != BullectVec.end(); ++dest_it){
				if ((*src_it)->IsActive && (((*src_it)->UserType == USER_TYPE::MONSTER && (*dest_it)->UserType != USER_TYPE::MONSTER) || ((*src_it)->UserType != USER_TYPE::MONSTER && (*dest_it)->UserType == USER_TYPE::MONSTER))){ //활성화가 되어있거나, 플레이어간또는 몬스터간이 아닌지
					(*src_it)->SetBullectPos();
					if (IntersectRect(&tmp, &(*src_it)->Bullect, &(*dest_it)->User)){
						(*dest_it)->IsCollision = true;
						if ((*src_it)->UserType == USER_TYPE::MAN || (*src_it)->UserType == USER_TYPE::WOMAN)
							(*src_it)->IsActive = false;
						return;
					}

				}
			}
			if ((*src_it)->UserType == USER_TYPE::MAN || (*src_it)->UserType == USER_TYPE::WOMAN)
				(*src_it)->IsActive = false;
		}
	}
	CBullect(const USER_TYPE& _UserType, const POINT& _UserSize, const POINT& _BullectSize) :UserType(_UserType), UserSize(_UserSize), BullectSize(_BullectSize){ //플레이어 및 몬스터구별, 소유자크기, 총알크기
		this->IsActive = false;
		this->IsCollision = false;
		this->IsDeath = false;
		memset(&User, 0, sizeof(User));
		memset(&Bullect, 0, sizeof(Bullect));
		CBullect::BullectVec.push_back(this);
	}
	~CBullect(){}

	void SetBullectPos(){
		if (this->UserType == USER_TYPE::MONSTER){
			this->Bullect = User;
		}
		else {
			Bullect.top = User.top;
			Bullect.bottom = User.bottom;
			if (this->uDirection == USER_DIR::DIR_LEFT){
				Bullect.right = User.left;
				Bullect.left = Bullect.right - BullectSize.x;
			}
			else{ //USER_DIR::DIR_RIGHT
				Bullect.left = User.right;
				Bullect.right = Bullect.left + BullectSize.x;
			}
		}

	}

	bool GetCollisionState(){
		if (this->IsCollision){
			this->IsCollision = false;
			return true;
		}

		return false;
	}
	void SetUserPos(const POINT& _pt){
		this->User.left = _pt.x;
		this->User.top = _pt.y;
		this->User.right = _pt.x + UserSize.x;
		this->User.bottom = _pt.y + UserSize.y;
	}
	void SetBullectActive(){
		this->IsActive = true;
	}
	void SetDirection(const USER_DIR& _dir){
		this->uDirection = _dir;
	}
	void SetDeath(){
		this->IsDeath = true;
	}

};
