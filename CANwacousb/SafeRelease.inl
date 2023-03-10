//���S�ȉ���E�폜���s���e���v���[�g
//
//����
//	2014.xx.xx	Y.OGAWA		1st release
//	2014.08.27	Y.OGAWA		2nd release
//							SafeDeleteObject��ǉ�
//	2015.10.29	Y.OGAWA		3rd release
//							SafeCloseHandle��ǉ�
//	2018.02.02	Y.OGAWA		4th release
//							SafeCloseSocket��ǉ�
//
//���ӓ_
//	SafeCloseSocket�̂݃|�C���^�ł͖������̂�����


//2�d�C���N���[�h�h�~�i*.h/*.cpp�ǂ��瑤����ł��g�p�\�j
#pragma once

//���S�ȉ��
template <class T> void SafeRelease(T **ppT)
	{
    if (*ppT)
	    {
        (*ppT)->Release();
        *ppT = NULL;
	    }
	}

//���S�ȍ폜
template <class T> void SafeDelete(T **ppT)
	{
	if (*ppT)
		{
		delete *ppT;
		*ppT = NULL;
		}
	}

template <class T> void SafeDeleteObject(T **ppT)
	{
	if (*ppT)
		{
		(*ppT)->DeleteObject();
		delete *ppT;
		*ppT = NULL;
		}
	}

template <class T> void SafeCloseHandle(T **ppT)
	{
	if(*ppT)
		{
		::CloseHandle(*ppT);
		*ppT = NULL;
		}
	}

template <class T> void SafeCloseSocket(T *pT)
	{
	if(*pT)
		{
		::closesocket(*pT);
		*pT = NULL;
		}
	}
