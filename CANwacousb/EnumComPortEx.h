#pragma once

#include "pch.h"
//#include <afxwin.h>
#include <setupapi.h>
#include <winioctl.h>
#include <wchar.h>
#include <shlobj.h>

class CEnumComPortEx
{
//================================================================================
//ユーザーが使用可能な要素
//================================================================================

public:
	//定義
	typedef enum
		{
		//
		 MAX_PORT	= 256
		,MAX_STRLEN	= 256
		
		//エラー関連
		,ERR_NOERROR	= 0
		,ERR_ERROR		= 1		//詳細不明なエラー、又は詳細を知らせる必要が無いエラー
		,ERR_SYSTEM		= 2		//内部エラー（機能が使えない場合等）


		} ENUM;

	//
	typedef struct ECPEX_INFO
		{
		bool	bAlive;
		PTCHAR	pName;
		} ECPEX_INFO,*pECPEX_INFO;

protected:
	//
	typedef struct _ECPEX_VAR
		{
		HANDLE		hSema;
		ECPEX_INFO	info[MAX_PORT];
		} ECPEX_VAR,*pECPEX_VAR;
	ECPEX_VAR m_var;

public:
	//COMポート情報を取得して格納する
	DWORD UpdateInformation(void);

	//格納されている情報へのポインタを取得する
	pECPEX_INFO GetPortInfo(int nCOMport)
		{
		if((nCOMport < 1) || (nCOMport >= MAX_PORT))
			return(NULL);
		else if(!m_var.info[nCOMport].bAlive)
			return(NULL);
		return(&m_var.info[nCOMport]);
		}

	//COMポート番号を指定値から検索
	int GetFirstPort(int nBeginPort = 1)
		{
		//概要
		//	システムに搭載されたCOMポートの内、指定番号又はそれより大きいポート番号を取得
		//パラメータ
		//	nBeginPort	検索を開始するCOMポート番号
		//戻り値
		//	1以上		COMポート番号
		//	0			COMポートが存在しない

		for(int nCOMport = nBeginPort;nCOMport < MAX_PORT;nCOMport++)
			{
			pECPEX_INFO pInfo = GetPortInfo(nCOMport);
			if(pInfo)
				return(nCOMport);
			}
		return(0);
		}

	//COMポート番号を指定値より大きい物を検索
	int GetNextPort(int& nCOMport)
		{
		//概要
		//	システムに搭載されたCOMポートの内、指定番号より大きいポート番号を取得
		//パラメータ
		//	nCOMport	COMポート番号（これより大きい値が検索対象となる）
		//戻り値
		//	1以上		次に発見したCOMポート番号
		//	0			指定値より大きいCOMポートが存在しない

		nCOMport = GetFirstPort(nCOMport + 1);
		return(nCOMport);
		}


//================================================================================
//内部用
//================================================================================
private:

	//資源占有
	bool Lock(DWORD nTimeoutMS = INFINITE)
		{
		if(::WaitForSingleObject(m_var.hSema,nTimeoutMS) != WAIT_OBJECT_0)
			return(false);
		return(true);
		}

	//資源開放
	void Unlock(void)
		{
		::ReleaseSemaphore(m_var.hSema,1,NULL);
		}

	//資源初期化
	void Clear(void)
		{
		if(Lock())
			{
			for(int nCOMport = 1;nCOMport < MAX_PORT;nCOMport++)
				{
				m_var.info[nCOMport].bAlive = false;
				if(m_var.info[nCOMport].pName)
					{
					delete m_var.info[nCOMport].pName;
					m_var.info[nCOMport].pName = NULL;
					}
				}
			Unlock();
			}
		}

public:
	//コンストラクタ
	CEnumComPortEx();
	//デストラクタ
	virtual ~CEnumComPortEx();

};
