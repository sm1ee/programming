#pragma once
#include "MyHeader.h"
#include <io.h>
#define _CRT_SECURE_NO_WARNINGS
class ImgManager {


	// 삭제용 이미지 저장소
	std::list<IMG_VEC*> listImgVec;


public:
	// 인터페이스

	IMG_VEC* setSprite(const char* path){

		IMG_VEC *_tmpImgVec = NULL; // 임시 
		_tmpImgVec = new IMG_VEC();

		char buf_path[MAX_PATH] = { 0, };

		sprintf_s(buf_path, sizeof(buf_path), "%s/*.*", path);

		_finddata_t filedat;
		intptr_t  hFile = _findfirst(buf_path, &filedat);

		do {
			if (hFile == -1)
				break;
			if (filedat.attrib & (_A_SUBDIR | _A_SYSTEM | _A_HIDDEN)) {
				continue; // . .. 처리 
			}
			else  {
				// 파일 읽기 작업
				// HBITMAP;
				HINSTANCE hins = GetModuleHandle(NULL);

				sprintf_s(buf_path, sizeof(buf_path), "%s/%s", path, filedat.name);
				tagBITMAP _data;


				ImageData _imgdat;
				_imgdat.bitmap = (HBITMAP)LoadImage(hins, buf_path, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_COLOR);

				GetObject(_imgdat.bitmap, sizeof(_data), &_data);

				_imgdat.height = _data.bmHeight;
				_imgdat.width = _data.bmWidth;
				sprintf_s(_imgdat.name, sizeof(_imgdat.name), "%s", filedat.name);

				(*_tmpImgVec).push_back(_imgdat);
			}

		} while (_findnext(hFile, &filedat) == 0);

		_findclose(hFile);

		listImgVec.push_back(_tmpImgVec); // 삭제 관리용

		return   _tmpImgVec;
	}


	// 삭제 처리
	void Release(){
		// 개발 단계에서 일괄 삭제

		for each (IMG_VEC* _temp in listImgVec){

			delete _temp;

		}

		listImgVec.clear();

		delete Img_Obj;
		Img_Obj = NULL;
	}


	// singleton
private:
	static ImgManager  *Img_Obj;

public:
	static ImgManager* Get_ImgManager(){

		if (Img_Obj == NULL)
			Img_Obj = new ImgManager();

		return Img_Obj;
	}

private:
	ImgManager(){}
	~ImgManager(){

	}
};
