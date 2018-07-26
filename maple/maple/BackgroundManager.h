#pragma once
#include "MyHeader.h"
#include "ImgManager.h"
class BackgroundManager {
private:
	static BackgroundManager *Background_Obj;

	POINT _pt;
	IMG_VEC *imgVecBack;
	HINSTANCE hins;
	HDC sub_dc;
	HDC Mem_DC;
	STAGE_TYPE stage;
	int scroll;
	BACK_STATE IsEndLine;
	bool EditMode = false;
	BackgroundManager(){
		_pt = { 0, 0 };
		imgVecBack = ImgManager::Get_ImgManager()->setSprite("./Image/background");
		sub_dc = NULL;
		Mem_DC = NULL;
		stage = STAGE_TYPE::STAGE1;
		scroll = 0;
		IsEndLine = BACK_STATE::NOT_ENDLINE;
	}
	~BackgroundManager(){
		DeleteDC(this->sub_dc);
	}
public:
	static BackgroundManager * Get_BackgroundManager(){
		if (BackgroundManager::Background_Obj == NULL){
			BackgroundManager::Background_Obj = new BackgroundManager();
		}
		return BackgroundManager::Background_Obj;
	}

	void Set_Dc(HDC _dc){
		this->Mem_DC = _dc;
		this->sub_dc = CreateCompatibleDC(_dc);
	}

	void Set_Stage(STAGE_TYPE _stage){
		this->stage = _stage;
	}
	BACK_STATE Get_IsEndLine(){
		return IsEndLine;
	}
	void Set_Scroll_Update(int _speed){
		scroll += _speed;
		IsEndLine = BACK_STATE::NOT_ENDLINE;
		if (scroll <= 0){
			scroll = 0;
			IsEndLine = BACK_STATE::LEFT_ENDLINE;
		}
		else if (scroll >= (*imgVecBack)[stage].width - WINSIZEX){
			scroll = (*imgVecBack)[stage].width - WINSIZEX;
			IsEndLine = BACK_STATE::RIGHT_ENDLINE;
		}
	}
	void Draw(){

		HBITMAP hOldBit = (HBITMAP)SelectObject(sub_dc, (*imgVecBack)[stage].bitmap);

		BitBlt(Mem_DC, 0, 0, (*imgVecBack)[stage].width - scroll, WINSIZEY, sub_dc, scroll, 0,  SRCCOPY);
		BitBlt(Mem_DC, (*imgVecBack)[stage].width - scroll, 0, scroll, WINSIZEY, sub_dc, 0, 0,  SRCCOPY);

		SelectObject(sub_dc, hOldBit);
	}
	//배경을 띄움
	//일단은 배경하나
	bool isEditMode(){
		return this->EditMode;
	}
	void Set_Edit(int state){
		if (state == 81)
			EditMode = !EditMode;
	}
	bool Check_Q(){
		bool _Q_check = this->EditMode;
		return _Q_check;
	}
	void DrawRect(RECT _rc){
		MoveToEx(Mem_DC, _rc.left, _rc.top, NULL);
		LineTo(Mem_DC, _rc.right, _rc.top);
		LineTo(Mem_DC, _rc.right, _rc.bottom);
		LineTo(Mem_DC, _rc.left, _rc.bottom);
		LineTo(Mem_DC, _rc.left, _rc.top);
	}
};


