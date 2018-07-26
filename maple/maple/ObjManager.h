#include "MyHeader.h"
#include "Obj.h"
#include "Player.h"
#include "Monster.h"
#include <list>

class ObjManager {

private:
	static ObjManager* Manager_Obj;

	ObjManager(){
		//BackSize = BACK_SIZE::STAGE1SIZE;
	}
	~ObjManager(){}

	std::list<Obj*> MyObject[OBJ_TYPE::OBJ_MAX];
	//BACK_SIZE BackSize;
public:
	static ObjManager* Get_Manager(){
		if (!ObjManager::Manager_Obj)
			ObjManager::Manager_Obj = new ObjManager();

		return ObjManager::Manager_Obj;
	}
	void Set_Staeg(STAGE_TYPE _stage){

		BACK_SIZE BackSize;
		switch (_stage){
		case STAGE_TYPE::STAGE1:
			BackSize = BACK_SIZE::STAGE1SIZE;
			break;
		case STAGE_TYPE::STAGE2:
			BackSize = BACK_SIZE::STAGE2SIZE;
			break;
		}

		for (int i = OBJ_TYPE::PLAYER; i < OBJ_TYPE::OBJ_MAX; i++)
			for (std::list<Obj*>::iterator _iter = MyObject[i].begin(); _iter != MyObject[i].end(); ++_iter)
				(*_iter)->Set_Back_Size(BackSize);

	}

	void Insert_Node(OBJ_TYPE _type, std::string _identify){ //초반 셋팅
		int speed = 0;
		int hp = 0;
		switch (_type) {
		case OBJ_TYPE::PLAYER:
			speed = 0;
			hp = 20;
			MyObject[OBJ_TYPE::PLAYER].push_back(new CPlayer());
			MyObject[_type].back()->Set_Init(speed, hp, _identify);
			break;
		case OBJ_TYPE::MONSTER:
			MyObject[_type].push_back(new CMonster());

			if (!_identify.compare("Mushroom")){
				speed = 20;
				hp = 5;
				MyObject[_type].back()->Set_Init(speed, hp, _identify);

			}
			else if (!_identify.compare("Zombie_Mushroom")){
				speed = 50;
				hp = 40;
				MyObject[_type].back()->Set_Init(speed, hp, _identify);
			}
			else {
				speed = 15;
				hp = 4;
				MyObject[_type].back()->Set_Init(speed, hp, _identify);
			}



			break;
		case OBJ_TYPE::NPC:
			//MyObject[_type].push_back(new CNpc());
			//MyObject[_type].back()->Set_Init();
			break;
		default:
			break;
		}


	}

	void Request_Draw(){
		for (int i = OBJ_TYPE::PLAYER; i < OBJ_TYPE::OBJ_MAX; i++)
			for (std::list<Obj*>::iterator _iter = MyObject[i].begin(); _iter != MyObject[i].end(); ++_iter)
				(*_iter)->Draw();
	}

	void Request_Update(){
		for (int i = OBJ_TYPE::PLAYER; i < OBJ_TYPE::OBJ_MAX; i++)
			for (std::list<Obj*>::iterator _iter = MyObject[i].begin(); _iter != MyObject[i].end(); ++_iter)
				(*_iter)->Update();
	}

	void Request_Update_Next_Image(){
		for (int i = OBJ_TYPE::PLAYER; i < OBJ_TYPE::OBJ_MAX; i++)
			for (std::list<Obj*>::iterator _iter = MyObject[i].begin(); _iter != MyObject[i].end(); ++_iter)
				(*_iter)->Update_Next_Image();
	}
	void Request_Set_Pos(POINT _pt){

	}
	int Request_Get_Dir_State(BACK_STATE _IsEndLine){ //몬스터 및 플레이어 이동
		int Move;
		POINT Monster_Pos;
		for (std::list<Obj*>::iterator _iter = MyObject[OBJ_TYPE::PLAYER].begin(); _iter != MyObject[OBJ_TYPE::PLAYER].end(); ++_iter)
			Move = ((CPlayer*)(*_iter))->BackDirUpdate(_IsEndLine);
		//일단 플레이어 한명일 때만 구현
		for (std::list<Obj*>::iterator _iter = MyObject[OBJ_TYPE::MONSTER].begin(); _iter != MyObject[OBJ_TYPE::MONSTER].end(); ++_iter){
			Monster_Pos = (*_iter)->Get_Pos();
			Monster_Pos.x -= Move;
			(*_iter)->Set_Pos(Monster_Pos.x, Monster_Pos.y); //
			((CMonster*)(*_iter))->Set_Scroll_State(Move);
		}
		return Move;
	}
	void Request_Player_Key_State(SHORT _KEY_VALUE){
		for (std::list<Obj*>::iterator _iter = MyObject[OBJ_TYPE::PLAYER].begin(); _iter != MyObject[OBJ_TYPE::PLAYER].end(); ++_iter)
			(*_iter)->Input_State(_KEY_VALUE);
	}

	void ReleaseControl(){
		//라이프 체크해서 삭제관리
		for (int i = OBJ_TYPE::PLAYER; i < OBJ_TYPE::OBJ_MAX; i++){
			for (std::list<Obj*>::iterator _iter = MyObject[i].begin(); _iter != MyObject[i].end();){
				if ((*_iter)->Get_IsDeath()){
					delete (*_iter);
					MyObject[i].erase(_iter++);
					//삭제
				}
				else ++_iter;
			}
		}
	}
	void Get_PlayerPos_For_Monster(){
		//일단 모든 플레이어 다 순회
		for (std::list<Obj*>::iterator P_iter = MyObject[OBJ_TYPE::PLAYER].begin(); P_iter != MyObject[OBJ_TYPE::PLAYER].end(); ++P_iter){
			for (std::list<Obj*>::iterator M_iter = MyObject[OBJ_TYPE::MONSTER].begin(); M_iter != MyObject[OBJ_TYPE::MONSTER].end(); ++M_iter){
				((CMonster*)(*M_iter))->Get_Player_Pos((*P_iter)->Get_Pos());
			}
		}
	}
public:
	void ReleaseAll(){
		for (std::list<Obj*>::iterator _iter = MyObject[OBJ_TYPE::PLAYER].begin(); _iter != MyObject[OBJ_TYPE::PLAYER].end(); ++_iter){
			for (std::list<Obj*>::iterator _iter = MyObject[OBJ_TYPE::MONSTER].begin(); _iter != MyObject[OBJ_TYPE::MONSTER].end(); ++_iter){
			}
		}
	}
};