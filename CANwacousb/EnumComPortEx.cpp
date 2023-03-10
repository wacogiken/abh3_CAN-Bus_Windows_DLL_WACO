//CEnumComPortExEx(COMポートを列挙するクラス・デバイス名付き)
// Copyright(C)2020 Y.OGAWA
//
//  Date		Auth		Ver		Comment
// -----------------------------------------------------------------
//	2020.04.16	Y.OGAWA		1.00	1st release
//									CEnumComPortExをベースに作成
//
//・使用に関する許諾条件
//　ソースコード内著作権表示の削除禁止
//
//使用方法
//	ターゲットプロジェクトにこのファイル(*.cpp/*.h)のリンクを追加して使用
//
//	(1)本クラスを1つ作成する
//	(2)UpdateInformation関数を呼び出す
//	(3)GetPortInfo関数をCOMポート番号を指定して呼び出し、情報を取得する
//

#include <pch.h>
#include <tchar.h>
#include "EnumComPortEx.h"
#include "SafeRelease.inl"

//自動ライブラリリンク
#pragma comment(lib,"setupapi.lib")

//コンストラクタ
CEnumComPortEx::CEnumComPortEx()
	{
	//内部変数の初期化
	::ZeroMemory(&m_var,sizeof(ECPEX_VAR));
	//セマフォ構築
	m_var.hSema = ::CreateSemaphore(NULL,1,1,NULL);
	}

//デストラクタ
CEnumComPortEx::~CEnumComPortEx()
	{
	//全開放
	Clear();
	//セマフォ開放
	::CloseHandle(m_var.hSema);

	}

//COMポート情報を取得して格納する
DWORD CEnumComPortEx::UpdateInformation()
	{
	//戻り値
	//	列挙されたCOMポートの数が戻ります
	//

	//初期化（自動資源ロックに注意）
	Clear();

	//資源占有
	if(!Lock())
		return(ERR_ERROR);	//資源占有失敗

	//デバイス情報格納先
	SP_DEVINFO_DATA DeviceInfoData;
	ZeroMemory(&DeviceInfoData,sizeof(SP_DEVINFO_DATA));
	DeviceInfoData.cbSize = sizeof(SP_DEVINFO_DATA);

	//一時的なバッファ
	BYTE tmpBuffer[MAX_STRLEN * sizeof(TCHAR)];
	PTCHAR pTmpBuffer = (PTCHAR)&tmpBuffer;
	DWORD nValue = 0;

	//デバイス情報を扱う為のハンドルを取得
	HDEVINFO hDevInfo = SetupDiGetClassDevs(&GUID_DEVINTERFACE_COMPORT,NULL,NULL,(DIGCF_PRESENT | DIGCF_DEVICEINTERFACE));
	if(hDevInfo == INVALID_HANDLE_VALUE)
		return(ERR_SYSTEM);	//取得失敗（エラー時は全デバイス使用不可扱い）

	// 列挙の終わりまでループ
	DWORD nDeviceNum = 0;
	while(SetupDiEnumDeviceInfo(hDevInfo,nDeviceNum,&DeviceInfoData))
		{
		// デバイスインスタンスIDを取得
		SetupDiGetDeviceInstanceId(hDevInfo,&DeviceInfoData,(PTSTR)pTmpBuffer,sizeof(tmpBuffer),&nValue);
		//デバイスIDを元にデバイスの情報を取得
		HKEY hKey = SetupDiOpenDevRegKey(hDevInfo,&DeviceInfoData,DICS_FLAG_GLOBAL,0,DIREG_DEV,KEY_QUERY_VALUE);
		if(hKey)
			{
			DWORD nType = 0;
			nValue = MAX_STRLEN;
			RegQueryValueEx(hKey,_T("PortName"),NULL,&nType,(LPBYTE)&tmpBuffer,&nValue);
			//デバイス名の最初が"COM"か？
			if((pTmpBuffer[0] == 'C') && (pTmpBuffer[1] == 'O') && (pTmpBuffer[2] == 'M'))
				{
				//"COM"から始まるデバイス名

				//COMポート番号を格納（4文字目から数字が入っているという前提）
				int nCOMport = (int)::_tcstoul((PTCHAR)&pTmpBuffer[3],NULL,10);

				//格納先ポインタを取得
				//pECP_PORTINFO pInfo = GetInfo(m_var.nEnablePortCount);
				//デバイス名を取得
				nValue = 0;
                SetupDiGetDeviceRegistryProperty(hDevInfo,&DeviceInfoData,SPDRP_FRIENDLYNAME,NULL,NULL,0,&nValue);
                if(nValue)
	                {
					//デバイス名が取得出来たので格納する

					//デバイス名の領域を構築
					m_var.info[nCOMport].pName = new TCHAR[nValue + 1]();
					//格納
					SetupDiGetDeviceRegistryProperty(hDevInfo,&DeviceInfoData,SPDRP_FRIENDLYNAME,NULL,(LPBYTE)m_var.info[nCOMport].pName,nValue,NULL);
					m_var.info[nCOMport].pName[nValue] = 0;	//終端コード
					//使用フラグ
					m_var.info[nCOMport].bAlive = true;
	                }
				}
			}
		//次のデバイス
		++nDeviceNum;
		} 
	//占有開放
	Unlock();

	//完了
	return(ERR_NOERROR);
	}

