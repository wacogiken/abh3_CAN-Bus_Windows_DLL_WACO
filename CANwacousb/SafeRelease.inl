//安全な解放・削除を行うテンプレート
//
//履歴
//	2014.xx.xx	Y.OGAWA		1st release
//	2014.08.27	Y.OGAWA		2nd release
//							SafeDeleteObjectを追加
//	2015.10.29	Y.OGAWA		3rd release
//							SafeCloseHandleを追加
//	2018.02.02	Y.OGAWA		4th release
//							SafeCloseSocketを追加
//
//注意点
//	SafeCloseSocketのみポインタでは無く実体を扱う


//2重インクルード防止（*.h/*.cppどちら側からでも使用可能）
#pragma once

//安全な解放
template <class T> void SafeRelease(T **ppT)
	{
    if (*ppT)
	    {
        (*ppT)->Release();
        *ppT = NULL;
	    }
	}

//安全な削除
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
