#pragma once
#include "MyHeader.h"
#include <io.h>
#define _CRT_SECURE_NO_WARNINGS
class ImgManager {


	// ������ �̹��� �����
	std::list<IMG_VEC*> listImgVec;


public:
	// �������̽�

	IMG_VEC* setSprite(const char* path){

		IMG_VEC *_tmpImgVec = NULL; // �ӽ� 
		_tmpImgVec = new IMG_VEC();

		char buf_path[MAX_PATH] = { 0, };

		sprintf_s(buf_path, sizeof(buf_path), "%s/*.*", path);

		_finddata_t filedat;
		intptr_t  hFile = _findfirst(buf_path, &filedat);

		do {
			if (hFile == -1)
				break;
			if (filedat.attrib & (_A_SUBDIR | _A_SYSTEM | _A_HIDDEN)) {
				continue; // . .. ó�� 
			}
			else  {
				// ���� �б� �۾�
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

		listImgVec.push_back(_tmpImgVec); // ���� ������

		return   _tmpImgVec;
	}


	// ���� ó��
	void Release(){
		// ���� �ܰ迡�� �ϰ� ����

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
