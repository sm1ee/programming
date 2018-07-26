#pragma once
#include "MyHeader.h"
#include "ImgManager.h"
#define FOUR 4



class InventoryManager{
private:

	static InventoryManager *Inventory_Obj;
	enum INVENTORY{
		IN_WINDOWS,
		IN_ITEM,
		IN_MAX_SIZE
	};

	enum ITEMS{
		IT_CAKE,
		IT_DDUK,
		IT_EGG,
		IT_PUMKIN,
		IT_TUNA,
		IT_MAX_SiZE
	};

	

	int available_count[INVENTORY::IN_MAX_SIZE];
	IMG_VEC* imgVecInventory[INVENTORY::IN_MAX_SIZE];
	
	int Is_Click_Count;
	
	bool check;
	HDC sub_dc;
	HDC Mem_DC;

	int Pos_x;
	int Pos_y;

	InventoryManager(){
		
		memset(&available_count, 0, sizeof(available_count));
		
		std::string tmp;

		this->imgVecInventory[INVENTORY::IN_WINDOWS] = ImgManager::Get_ImgManager()->setSprite("./Image/Windows");
		this->imgVecInventory[INVENTORY::IN_ITEM] = ImgManager::Get_ImgManager()->setSprite("./Image/Item");
		this->check = false;
		sub_dc = NULL;
		Mem_DC = NULL;
	}

	~InventoryManager(){}

public:

	struct Inven_Window{
		int Take_Item[InventoryManager::IT_MAX_SiZE];

		
	};

	typedef std::list<Inven_Window> WINVEC;

	WINVEC* Set_Inven(){

		WINVEC* _tmpin_win = NULL;
		_tmpin_win = new WINVEC();

		Inven_Window in_windat;

		memset(in_windat.Take_Item, 0, InventoryManager::IT_MAX_SiZE);
		in_windat.Take_Item[InventoryManager::IT_MAX_SiZE];
		(*_tmpin_win).push_back(in_windat);

		return _tmpin_win; //�ڱ� �κ��丮�� �ּ� ����
	}



	static InventoryManager *Get_InventoryManager(){
		if (!Inventory_Obj){
			Inventory_Obj = new InventoryManager();
		}

		return Inventory_Obj;
	}

	void Set_Dc(HDC _dc){
			this->Mem_DC = _dc;
			this->sub_dc = CreateCompatibleDC(_dc);
	}
	void Input_State(SHORT _Key_Value){
		//if (_Key_Value == KEY_STATE::STATE_INVEN)
			
	}
	void Draw(){
		Pos_x = 700; //�κ��丮 â x ��ǥ ��ġ
		Pos_y = 200;//      "      y ��ǥ ��ġ

		HBITMAP HoldBit = (HBITMAP)SelectObject(sub_dc, (*imgVecInventory[INVENTORY::IN_WINDOWS])[INVENTORY::IN_WINDOWS].bitmap); //�κ��丮â ���
		TransparentBlt(Mem_DC, Pos_x, Pos_y, (*imgVecInventory[INVENTORY::IN_WINDOWS])[INVENTORY::IN_WINDOWS].width, (*imgVecInventory[INVENTORY::IN_WINDOWS])[INVENTORY::IN_WINDOWS].height, sub_dc, 0, 0, (*imgVecInventory[INVENTORY::IN_WINDOWS])[INVENTORY::IN_WINDOWS].width, (*imgVecInventory[INVENTORY::IN_WINDOWS])[INVENTORY::IN_WINDOWS].height, RGB(163, 73, 164));
		SelectObject(sub_dc, HoldBit);

		Pos_x += 11; //ù ������ x ��ǥ ��ġ
		Pos_y += 52; //    "     y ��ǥ ��ġ
		int Count_Max = 0;

		for (int i = 0; i < ITEMS::IT_MAX_SiZE; i++){ //�������� ���

			//if (available_count[i]==1){

			HoldBit = (HBITMAP)SelectObject(sub_dc, (*imgVecInventory[INVENTORY::IN_ITEM])[i].bitmap); //������
			TransparentBlt(Mem_DC, Pos_x, Pos_y, +(*imgVecInventory[INVENTORY::IN_ITEM])[i].width, (*imgVecInventory[INVENTORY::IN_ITEM])[i].height, sub_dc, 0, 0, (*imgVecInventory[INVENTORY::IN_ITEM])[i].width, (*imgVecInventory[INVENTORY::IN_ITEM])[i].height, RGB(163, 73, 164));
				SelectObject(sub_dc, HoldBit);

				Count_Max++;
				
				if (Count_Max>3){ //���ٿ� 4ĭ�ΰ� ���� 4�� �Ѿ�� ���� ������ ���ֱ� ����

					Count_Max = 0;
					Pos_x -= 108;
					Pos_y += 35;
				}

				else
					Pos_x += 36;
			//}
		}
	}

};




