#pragma once
#include "MyHeader.h"
#include "ImgManager.h"
// I 좌 우 스페이스 Z 마우스 왼쪽
class InputManager {
private:
	enum nput{
		I,
		Z,
		Left,
		Right,
		Space,
		L_mouse,
		MAX
	};
	static InputManager* Input_Obj;
	IMG_VEC *imgVecInput;
	bool click;
	HDC sub_dc;
	HDC Mem_DC;
	int MouseScroll;
	bool click_check = false;
	bool draw_check = false;
	bool rbotton = false;
	POINT pt1, pt2;
	RECT rc;
	InputManager(){
		imgVecInput = ImgManager::Get_ImgManager()->setSprite("./Image/Mouse");
		this->click = false;
		sub_dc = NULL;
		Mem_DC = NULL;
		ShowCursor(false); //마우스 커서 off
		MouseScroll = 0;
		pt1.x = 0;
		pt1.y = 0;
		pt2.x = 0;
		pt2.y = 0;
	}
	~InputManager(){
		DeleteDC(this->sub_dc);
	}

public:
	static InputManager* Get_InputManager(){
		if (!Input_Obj)
			Input_Obj = new InputManager();
		return Input_Obj;
	}

	POINT Get_MousePos(){
		POINT _pt;
		GetCursorPos(&_pt);
		ScreenToClient(GetFocus(), &_pt); //윈도우 창에서 전체기준이 아니라 탭을 제외한 안쪽 윈도우 창 기준
		return _pt;
	}

	void UpdateMouseScroll(int _posX){
		MouseScroll += _posX;

	}
	void Set_DC(HDC _dc){
		this->Mem_DC = _dc;
		this->sub_dc = CreateCompatibleDC(_dc);
	}
	void Draw(){
		
		int state = click ^ 1;
		
		POINT _pt = Get_MousePos();


		HBITMAP hOldBit = (HBITMAP)SelectObject(sub_dc, (*imgVecInput)[state].bitmap);

		TransparentBlt(Mem_DC, _pt.x, _pt.y, (*imgVecInput)[state].width, (*imgVecInput)[state].height, sub_dc, 0, 0, (*imgVecInput)[state].width, (*imgVecInput)[state].height, RGB(163, 73, 164));
		SelectObject(sub_dc, hOldBit);

		char arr[255] = { 0 };
		sprintf(arr, "X : %d, Y : %d", _pt.x + this->MouseScroll, _pt.y);
		SetWindowTextA(GetFocus(), arr);
	}


	SHORT Input_State(){
		SHORT Key_Value = 0;
		SHORT temp = GetAsyncKeyState('I');
		SHORT Q_temp = GetAsyncKeyState('Q');
		if (GetAsyncKeyState(VK_LBUTTON) & 0x8001){ //마우스 좌클릭
			click = true;
			Key_Value = VK_LBUTTON;
		}
		else if (GetAsyncKeyState(VK_RBUTTON) & 0x8001){
			Key_Value = VK_LBUTTON;
		}
		else if (GetAsyncKeyState(VK_SHIFT) & 0x8001){
			Key_Value = VK_SHIFT;
		}
		else if (GetAsyncKeyState('Z')) {
			Key_Value = 'Z';
		}
		else if ((temp & 0x8000) && (temp & 0x0001)) {
			Key_Value = 'I';
		}
		else if ((Q_temp & 0x8000) && (Q_temp & 0x0001)){
			Key_Value = 'Q';
		}
		else if (GetAsyncKeyState(VK_SPACE) & 0x8001){
			Key_Value = VK_SPACE;
		} //스페이스바
		else if (GetAsyncKeyState(VK_LEFT)){
			Key_Value = VK_LEFT;
		} //왼쪽 키
		else if (GetAsyncKeyState(VK_RIGHT)){
 			Key_Value = VK_RIGHT;
		} //오른쪽 키

		else{
			click = false;
		}

		return Key_Value;
	}
	void Rec_Draw(){
		if (click_check){
			MoveToEx(Mem_DC, pt1.x, pt1.y, NULL);
			LineTo(Mem_DC, pt2.x, pt1.y);
			LineTo(Mem_DC, pt2.x, pt2.y);
			LineTo(Mem_DC, pt1.x, pt2.y);
			LineTo(Mem_DC, pt1.x, pt1.y);
		}
	}
	bool check_draw(){
		return draw_check;
	}
	void MouseDown(){
		if (GetAsyncKeyState(VK_LBUTTON) || GetAsyncKeyState(VK_RBUTTON)){
			if (!click_check) {
				click_check = true;
				GetCursorPos(&pt1);
				ScreenToClient(GetFocus(), &pt1);
				rbotton = false;
				if (GetAsyncKeyState(VK_RBUTTON)) 
					rbotton = true;
			}
		}
		else click_check = false;
	}
	void MouseUp(){
		if (click_check) {
			GetCursorPos(&pt2);
			ScreenToClient(GetFocus(), &pt2);
		}
		else if (pt1.x || pt1.y) {
			//렉트리턴
			draw_check = true;

			rc.left = pt1.x;
			rc.top = pt1.y;
			rc.right = pt2.x;
			rc.bottom = pt2.y;
			pt2 = pt1 = { 0, 0 };
		}
	}
	bool return_rbotton(){
		if (rbotton) {
			rbotton = false;
			return true;
		}
		return false;
	}
	RECT return_RC(){
		draw_check = false;
		return this->rc;
	}
	//void Draw_Rec(BackgroundManager* back_manager, ObjManager* Obj_Manager){
	//	RECT _rc;
	//	MouseDown();
	//	MouseUp();
	//	if (check_draw()){
	//		_rc = return_RC();
	//		if (return_rbotton())
	//			Obj_Manager->compareInRect(_rc, Mem_DC);
	//		else
	//			Obj_Manager->InsertBlockRect(_rc.left, _rc.top, _rc.right, _rc.bottom);
	//	}
	//	Rec_Draw();
	//	Obj_Manager->Request_Draw();
	//	Obj_Manager->requestDrawRect(back_manager);
	//	
	//}
};
