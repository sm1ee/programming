#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include "MyHeader.h"
#include "Fps.h"
#include "ObjManager.h"
#include "Obj.h"
#include "BackgroundManager.h"
#include "InputManager.h"
#include "ImgManager.h"

//프레임워크는 필요없기 떄문에 모르도록 수정할 예정
#include "InventoryManager.h"
#include "Effect.h"
#include "Bullet.h"

static HDC dc;
static HDC MemDC;
static HDC sub_dc;
static HWND hWnd;
static HBITMAP hBitmap;
static HBITMAP	OldBitmap;
static HINSTANCE hins;

static CFps MainFps(MAIN_DELAY);
static CFps InputFps(INPUT_DELAY);
//static Fps MouseFps(MOUSE_DELAY);

HDC Obj::dc;
HDC CEffect::Mem_DC;
ImgManager* ImgManager::Img_Obj;
ObjManager* ObjManager::Manager_Obj;
InputManager* InputManager::Input_Obj;
BackgroundManager* BackgroundManager::Background_Obj;
InventoryManager* InventoryManager::Inventory_Obj;
IMG_VEC* CEffect::ImgVecEffect[][CEffect::EFFECT::MAX_SIZE];
std::list<CBullect*> CBullect::BullectVec;
//std::list<Obj*> ObjManager::MyObject[];
//static 변수 선언 


static ObjManager* Obj_Manager = ObjManager::Get_Manager();
static InputManager* Input_Manager = InputManager::Get_InputManager();
static BackgroundManager* Back_Manager = BackgroundManager::Get_BackgroundManager();
static InventoryManager* Inven_Manager = InventoryManager::Get_InventoryManager();
//편하게 사용하기 위한 포인터 변수


void game_Init(MSG &Message){
	srand((unsigned)time((time_t)NULL));

	HBITMAP hOldBit = NULL;

	hWnd = GetFocus();
	dc = GetDC(hWnd);
	hins = GetModuleHandle(NULL);
	MemDC = CreateCompatibleDC(dc);
	sub_dc = CreateCompatibleDC(MemDC);
	hBitmap = CreateCompatibleBitmap(dc, WINSIZEX, WINSIZEY);
	OldBitmap = (HBITMAP)SelectObject(MemDC, hBitmap);

	Obj::Set_Dc(MemDC);
	Inven_Manager->Set_Dc(MemDC);
	Input_Manager->Set_DC(MemDC);
	Back_Manager->Set_Dc(MemDC);
	CEffect::Set_Init(MemDC);
	int SEX = 0;
	class Mouse{
	public:
		Mouse(){
			this->Key_Value = 0;
			this->_pt = { 0, 0 };
			memset(buf, 0, sizeof(buf));
		}
		SHORT Key_Value;
		POINT _pt; //클릭 시 마우스 x,y좌표 값
		char buf[20];

		void Set_MousePos(){
			GetCursorPos(&_pt);

			ScreenToClient(GetFocus(), &_pt); //윈도우 창에서 전체기준이 아니라 탭을 제외한 안쪽 윈도우 창 기준
		}
		void show_pos(){
			sprintf(buf, "X : %d, Y : %d", _pt.x, _pt.y);
			SetWindowTextA(GetFocus(), buf);
		}
	};


	enum BIT_TYPE{
		STORY1_PLAYER_BOY,
		STORY1_PLAYER_GIRL,
		STORY2_PLAYER_BOY,
		STORY2_PLAYER_GIRL,
		_NPC,
		TEXTBOX_LEFT,
		TEXTBOX_RIGHT,
		STORY1_BACK,
		STORY2_BACK,
		MOUSE_CLICK,
		MOUSE_DEFAULT,
		MAX_SIZE
	};

	enum IMG_STATE{
		PLAYER,
		NPC,
		TEXTBOX,
		BACKGROUND,
		MOUSE,
		MAX
	};

	Mouse mouse;
	HBITMAP Image_Bitmap[BIT_TYPE::MAX_SIZE] = { 0, };
	HBITMAP Image_State[IMG_STATE::MAX] = { 0, };

	[&](){
		char * path[BIT_TYPE::MAX_SIZE] = { "./Image/story1/man.bmp", "./Image/story1/woman.bmp", "./Image/story2/man.bmp", "./Image/story2/woman.bmp", "./Image/story2/npc.bmp", "./Image/story2/textleft.bmp", "./Image/story2/textright.bmp", "./Image/story1/background.bmp", "./Image/story2/background.bmp", "./Image/Mouse/click.bmp", "./Image/Mouse/default.bmp" };
		for (int i = BIT_TYPE::STORY1_PLAYER_BOY; i < BIT_TYPE::MAX_SIZE; i++){
			if (i == BIT_TYPE::STORY1_BACK || i == BIT_TYPE::STORY2_BACK)
				Image_Bitmap[i] = (HBITMAP)LoadImage(hins, path[i], IMAGE_BITMAP, WINSIZEX, WINSIZEY, LR_LOADFROMFILE | LR_COLOR);
			else
				Image_Bitmap[i] = (HBITMAP)LoadImage(hins, path[i], IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_COLOR);
		}
	}();



	/*--------------------------------초기값 셋팅-------------------------------------*/


	/*##########################남자, 여자 선택 후 okay버튼 누르면 해당 좌표값에 의해서 남자인지 여자인지 판단 #######################
	##########################그 후 해당 성별 캐릭터를 생성하고 NPC와 서로 대화를 주고받음 #######################*/

	//일단 하드코딩으로 구현

		RECT char_size[2] = { { 390, 305, 510, 340 }, { 540, 305, 655, 340 } };
		RECT okay_button = { 410, 570, 645, 620 };
	//	int SEX = 0;
	
		Image_State[IMG_STATE::MOUSE] = Image_Bitmap[BIT_TYPE::MOUSE_DEFAULT];
		Image_State[IMG_STATE::BACKGROUND] = Image_Bitmap[BIT_TYPE::STORY1_BACK];
	
		while (Message.message != WM_QUIT){
			if (PeekMessage(&Message, NULL, 0, 0, PM_REMOVE)){
				TranslateMessage(&Message);
				DispatchMessage(&Message);
			}
	
			//Rectangle(dc, char_size[0].left, char_size[0].top, char_size[0].right, char_size[0].bottom);
			//Rectangle(dc, char_size[1].left, char_size[1].top, char_size[1].right, char_size[1].bottom);
			//Rectangle(dc, okay_button.left, okay_button.top, okay_button.right, okay_button.bottom);
	
			mouse.Set_MousePos();
			//mouse.show_pos();
			
			hOldBit = (HBITMAP)SelectObject(sub_dc, Image_State[IMG_STATE::BACKGROUND]);
			BitBlt(MemDC, 0, 0, WINSIZEX, WINSIZEY, sub_dc, 0, 0, SRCCOPY);
			SelectObject(sub_dc, hOldBit);
	
			if (Image_State[IMG_STATE::PLAYER]){
				hOldBit = (HBITMAP)SelectObject(sub_dc, Image_State[IMG_STATE::PLAYER]);
				TransparentBlt(MemDC, 470, 390, 84, 97, sub_dc, 0, 0, 76, 87, RGB(163, 73, 164));
				SelectObject(sub_dc, hOldBit);
			}
	
			mouse.Key_Value = GetAsyncKeyState(VK_LBUTTON);
			
			Image_State[IMG_STATE::MOUSE] = (mouse.Key_Value & 0x8001) ? Image_Bitmap[BIT_TYPE::MOUSE_CLICK] : Image_Bitmap[BIT_TYPE::MOUSE_DEFAULT];
	
			hOldBit = (HBITMAP)SelectObject(sub_dc, Image_State[IMG_STATE::MOUSE]);
			TransparentBlt(MemDC, mouse._pt.x - 12, mouse._pt.y - 12, 48, 48, sub_dc, 0, 0, 48, 48, RGB(163, 73, 164));
			SelectObject(sub_dc, hOldBit);
	
			if (mouse.Key_Value & 0x0001){
				mouse.Set_MousePos();
				RECT pt_to_rect = { mouse._pt.x, mouse._pt.y, mouse._pt.x + 1, mouse._pt.y + 1 };
				RECT tmp;
				auto tmpfunc = [&]()->bool{
					for (int i = 0; i < 2; i++){
	
						if (IntersectRect(&tmp, &pt_to_rect, &char_size[i])){
							SEX = i + 1;
							Image_State[IMG_STATE::PLAYER] = (SEX == 1) ? Image_Bitmap[BIT_TYPE::STORY1_PLAYER_BOY] : Image_Bitmap[BIT_TYPE::STORY1_PLAYER_GIRL];
	
						}
					}
					if (IntersectRect(&tmp, &pt_to_rect, &okay_button)){
						if (SEX)
							return true;
						return false;
					}
					return false;
				};
				if (tmpfunc())
					break;
			}
	
			BitBlt(dc, 0, 0, WINSIZEX, WINSIZEY, MemDC, 0, 0, SRCCOPY);
		} //while END
	
		
	
		/*########################################################################*/
	
	
	
	
		/*#####################NPC랑 대화하는 소스코드#########################
		  #####################대화가 끝나면 사냥하는 화면으로 이동#############*/
	
		//sub_dc = Back_Manager->get_subDC();	
		char strings[255] = { 0, };
		bool isDraw = false;
		int index = 0;
		int tmp2 = 0;
		const int Max = 5;
		Image_State[IMG_STATE::MOUSE] = Image_Bitmap[BIT_TYPE::MOUSE_DEFAULT];
		Image_State[IMG_STATE::BACKGROUND] = Image_Bitmap[BIT_TYPE::STORY2_BACK];
		Image_State[IMG_STATE::NPC] = Image_Bitmap[BIT_TYPE::_NPC];
		Image_State[IMG_STATE::TEXTBOX] = Image_Bitmap[BIT_TYPE::TEXTBOX_RIGHT];
		Image_State[IMG_STATE::PLAYER] = (SEX == 1) ? Image_Bitmap[BIT_TYPE::STORY2_PLAYER_BOY] : Image_Bitmap[BIT_TYPE::STORY2_PLAYER_GIRL];
		while (Message.message != WM_QUIT){
			if (PeekMessage(&Message, NULL, 0, 0, PM_REMOVE)){
				TranslateMessage(&Message);
				DispatchMessage(&Message);
			}
	
			
			mouse.Set_MousePos();
			mouse.show_pos();
	
			hOldBit = (HBITMAP)SelectObject(sub_dc, Image_State[IMG_STATE::BACKGROUND]);
			BitBlt(MemDC, 0, 0, WINSIZEX, WINSIZEY, sub_dc, 0, 0, SRCCOPY);
			SelectObject(sub_dc, hOldBit);
			
			
			hOldBit = (HBITMAP)SelectObject(sub_dc, Image_State[IMG_STATE::PLAYER]);
			TransparentBlt(MemDC, 825, 517, 84, 97, sub_dc, 0, 0, 76, 87, RGB(163, 73, 164));
			SelectObject(sub_dc, hOldBit);
			
			hOldBit = (HBITMAP)SelectObject(sub_dc, Image_State[IMG_STATE::TEXTBOX]);
			TransparentBlt(MemDC, 300, 200, 519, 190, sub_dc, 0, 0, 519, 190, RGB(163, 73, 164));
			SelectObject(sub_dc, hOldBit);
	
			hOldBit = (HBITMAP)SelectObject(sub_dc, Image_State[IMG_STATE::NPC]);
			TransparentBlt(MemDC, 115, 355, 60, 76, sub_dc, 0, 0, 51, 67, RGB(163, 73, 164)); //계단위
			SelectObject(sub_dc, hOldBit);
	
	
			if (index != 2 && index != 4)
			{
				hOldBit = (HBITMAP)SelectObject(sub_dc, Image_State[IMG_STATE::NPC]);
				TransparentBlt(MemDC, 350, 260, 60, 76, sub_dc, 0, 0, 51, 67, RGB(163, 73, 164)); //텍스트위
				SelectObject(sub_dc, hOldBit);
			}
	
			else
			{
				hOldBit = (HBITMAP)SelectObject(sub_dc, Image_State[IMG_STATE::PLAYER]);
				TransparentBlt(MemDC, 700, 245, 84, 97, sub_dc, 0, 0, 76, 87, RGB(163, 73, 164));
				SelectObject(sub_dc, hOldBit);
			}
	
	
			mouse.Key_Value = GetAsyncKeyState(VK_LBUTTON);
		
			Image_State[IMG_STATE::MOUSE] = (mouse.Key_Value & 0x8001 || GetAsyncKeyState(VK_RETURN) & 0x8000) ? Image_Bitmap[BIT_TYPE::MOUSE_CLICK] : Image_Bitmap[BIT_TYPE::MOUSE_DEFAULT];
	
			hOldBit = (HBITMAP)SelectObject(sub_dc, Image_State[IMG_STATE::MOUSE]);
			TransparentBlt(MemDC, mouse._pt.x - 12, mouse._pt.y - 12, 48, 48, sub_dc, 0, 0, 48, 48, RGB(163, 73, 164)); //마우스 버튼 이미지 띄움
			SelectObject(sub_dc, hOldBit);
	
	
			if (!isDraw){ //한번만 글자 출력해야하니까 출력했으면 true
				memset(strings, 0, sizeof(strings));
				char tmp;
				char filename[100] = { 0, };
				int fileIndex = 0;
				isDraw = true;
	
				sprintf(filename, "./Image/story2/comment/%d.txt", index);
				std::ifstream OpenFile;
				OpenFile.open(filename);
	
				if (index != 2 && index != 4){
					for (int j = 0; !OpenFile.eof(); j++){
						OpenFile.get(tmp);
						strings[j] = tmp;
						TextOutA(MemDC, 460, 230, strings, strlen(strings));
						BitBlt(dc, 0, 0, WINSIZEX, WINSIZEY, MemDC, 0, 0, SRCCOPY);
						Sleep(50);
					}
				}
	
				else{
					for (int j = 0; !OpenFile.eof(); j++){
						OpenFile.get(tmp);
						strings[j] = tmp;
						TextOutA(MemDC, 330, 230, strings, strlen(strings));
						BitBlt(dc, 0, 0, WINSIZEX, WINSIZEY, MemDC, 0, 0, SRCCOPY);
						Sleep(50);
					}
				}
			}
	
			if (index != 2 && index != 4) //text 최종출력 위치
				TextOutA(MemDC, 460, 230, strings, strlen(strings));
			else 
				TextOutA(MemDC, 330, 230, strings, strlen(strings)); 
	
			Image_State[IMG_STATE::MOUSE] = (mouse.Key_Value & 0x8001) ? Image_Bitmap[BIT_TYPE::MOUSE_CLICK] : Image_Bitmap[BIT_TYPE::MOUSE_DEFAULT];
	
			hOldBit = (HBITMAP)SelectObject(sub_dc, Image_State[IMG_STATE::MOUSE]);
			TransparentBlt(MemDC, mouse._pt.x - 12, mouse._pt.y - 12, 48, 48, sub_dc, 0, 0, 48, 48, RGB(163, 73, 164)); //마우스 버튼 이미지 띄움
			SelectObject(sub_dc, hOldBit);
	
			bool tmp = false;
			if (mouse.Key_Value & 0x0001){ //클릭했는지 
				std::ifstream OpenFile;
				mouse.Set_MousePos();
				RECT t_rect;
				RECT Next_button_1 = { 755, 330, 800, 345 };
				RECT Next_button_2 = { 620, 330, 665, 345 };
				RECT pt_to_rect = { mouse._pt.x, mouse._pt.y, mouse._pt.x + 1, mouse._pt.y + 1 };
	
				switch (index){
				case 1:
				case 3:
					if (IntersectRect(&t_rect, &Next_button_1, &pt_to_rect)){
						Image_State[IMG_STATE::TEXTBOX] = Image_Bitmap[BIT_TYPE::TEXTBOX_LEFT];
						isDraw = false;
						if (++index >= Max)
							tmp = true;
					}
	
					break;
				case 2:
				case 4:
					if (IntersectRect(&t_rect, &Next_button_2, &pt_to_rect)){ //다음버튼을 클릭했는지
						Image_State[IMG_STATE::TEXTBOX] = Image_Bitmap[BIT_TYPE::TEXTBOX_RIGHT];
						isDraw = false;
						if (++index >= Max)
							tmp = true;
					}
					break;
	
				default:
					if (IntersectRect(&t_rect, &Next_button_1, &pt_to_rect)){ //다음버튼을 클릭했는지
						Image_State[IMG_STATE::TEXTBOX] = Image_Bitmap[BIT_TYPE::TEXTBOX_RIGHT];
						isDraw = false;
						index++;
					}
				}
			}
			else if (GetAsyncKeyState(VK_RETURN) & 0x8000){
				switch (index){
				case 1:
				case 3:
					Image_State[IMG_STATE::TEXTBOX] = Image_Bitmap[BIT_TYPE::TEXTBOX_LEFT];
						isDraw = false;
						if (++index >= Max)
							tmp = true;
					break;
				case 2:
				case 4:
					 //다음버튼을 클릭했는지
					Image_State[IMG_STATE::TEXTBOX] = Image_Bitmap[BIT_TYPE::TEXTBOX_RIGHT];
						isDraw = false;
						if (++index >= Max)
							tmp = true;
					break;
	
				default:
					Image_State[IMG_STATE::TEXTBOX] = Image_Bitmap[BIT_TYPE::TEXTBOX_RIGHT];
					isDraw = false;
					index++;
				}
				
			}
			if (tmp)
				break;
			BitBlt(dc, 0, 0, WINSIZEX, WINSIZEY, MemDC, 0, 0, SRCCOPY);
	
		} //while

	/*####################################################################*/

	char * S_SEX = (SEX == 1) ? "Boy" : "Girl";
	Obj_Manager->Insert_Node(OBJ_TYPE::PLAYER, S_SEX); //캐릭터 생s성
	//Obj_Manager->Insert_Node(OBJ_TYPE::PLAYER, "Girl"); //캐릭터 생성
	Obj_Manager->Insert_Node(OBJ_TYPE::MONSTER, "Mushroom"); //default로 몬스터 몇마리 생성
	Obj_Manager->Insert_Node(OBJ_TYPE::MONSTER, "Slim");
	Obj_Manager->Insert_Node(OBJ_TYPE::MONSTER, "Mushroom"); //default로 몬스터 몇마리 생성
	Obj_Manager->Insert_Node(OBJ_TYPE::MONSTER, "Slim");
	Obj_Manager->Insert_Node(OBJ_TYPE::MONSTER, "Mushroom"); //default로 몬스터 몇마리 생성
	Obj_Manager->Insert_Node(OBJ_TYPE::MONSTER, "Slim");
	Obj_Manager->Insert_Node(OBJ_TYPE::MONSTER, "Mushroom"); //default로 몬스터 몇마리 생성
	Obj_Manager->Insert_Node(OBJ_TYPE::MONSTER, "Slim");
}


void game_Progress(){

	if (InputFps.getFps()){
		SHORT Key_Value = Input_Manager->Input_State();
		Obj_Manager->Request_Player_Key_State(Key_Value);
		BACK_STATE IsEndLine = Back_Manager->Get_IsEndLine();
		int scroll = Obj_Manager->Request_Get_Dir_State(IsEndLine);
		Input_Manager->UpdateMouseScroll(scroll);
		Back_Manager->Set_Scroll_Update(scroll);
	}

	
	if (MainFps.getFps()){
		Obj_Manager->Request_Update_Next_Image();
		Obj_Manager->Request_Update();
		CBullect::ObjectIsDeath();
		CBullect::CollisionControl(); //충돌관리
		Obj_Manager->Get_PlayerPos_For_Monster();

		Obj_Manager->ReleaseControl();
	}

	Back_Manager->Draw();
	Obj_Manager->Request_Draw();
	//Input_Manager->Draw();

	BitBlt(dc, 0, 0, WINSIZEX, WINSIZEY, MemDC, 0, 0, SRCCOPY);
}

void game_Release(){

	ImgManager::Get_ImgManager()->Release();

	DeleteObject(SelectObject(MemDC, OldBitmap));
	DeleteDC(sub_dc);
	DeleteDC(MemDC);
	ReleaseDC(hWnd, dc);
}