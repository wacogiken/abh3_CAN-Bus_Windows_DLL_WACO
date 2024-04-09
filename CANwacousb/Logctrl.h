//ログ制御クラス
//
//概要
//	CANのデータをテキストファイルで残す為のクラス
//	記録する要素は固定なので注意。
//
//使い方
//	CAN回線を制御しているクラスに、本クラスの変数を作る。(この例ではm_logとする）
//	ログファイルの保存先をInitialize関数で指定する。
//	CAN回線を開く時、Control関数を呼び出してから、Open関数を呼び出す
//	CAN回線を閉じた時に、Close関数を呼び出す
//	CAN回線への送受信時、Add関数を呼び出す
//
//保存仕様
//	指定したフォルダの下に、開始時の日付と時間でフォルダを作る
//	中に入るファイル名は、6桁の数値.txtとなる
//
//機能設定
//	以下の設定は、Open関数を呼び出す前に行う必要が有る事に注意
//		Control関数で、有効/無効、ログ対象を制御する
//		SetLimitRow関数で、自動的に次のファイルに移行する行数を指定する
//
//注意点
//	全てのエラーは基本的に無視される為、以下の様な動作となる。。
//	・ファイルの追記に失敗しても、ファイルはクローズされない
//	・ファイルのオープンに失敗すると、ログは記録されない
//
//本クラスの利用について
//	GetTM関数の為、"winmm.lib"のリンクが必要
//

#pragma once

#include <Windows.h>
#include <stdint.h>
#include <stdio.h>
#include <timeapi.h>	//Windows.hより後になる様にインクルードする

//
class CLogctrl
{
public:

	//時間保存用構造体
	typedef struct _LOGCTRL_TIME
		{
		uint32_t	nTick;				//現在時刻[ms]
		SYSTEMTIME	sysTime;			//ローカル時間用構造体
		} LOGCTRL_TIME,*pLOGCTRL_TIME;

protected:
	//内部変数
	typedef struct _LOGCTRL_VAR
		{
		uint32_t		nLineLimit;			//次のファイルに移行するログ行数
		uint8_t			nControl;			//ログ制御フラグ（bit毎の意味は、Control関数を参照）
		wchar_t			logFolder[1024];	//ログフォルダ（末尾にパス記号有り）
		DWORD			nFileCount;			//ファイル自動更新時に加算される
		LOGCTRL_TIME	firstTime;			//最初に回線を開いた時の時間
		DWORD			nLogCount;			//ログ行数（一定行数で次のファイルを作る）
		FILE*			hLog;				//ファイルオープンハンドル（有効な場合はファイル保存中）
		HANDLE			hSema;				//ログファイル追記時の排他制御用
		} LOGCTRL_VAR,*pLOGCTRL_VAR;
	LOGCTRL_VAR m_var;

public:
	//ログ機能初期化及びログフォルダ設定
	int32_t Initialize(uint8_t nUtf16,void* pLogFolder)
		{
		//概要
		//	ログ機能初期化及びログフォルダ設定
		//パラメータ
		//	nUtf16			0..pLogFolderはAnsi  0以外..pLogFolderはUTF-16
		//	pLogFolder		ログフォルダ指定（末尾にパス記号を付けない事）
		//戻り値
		//	常に0が戻ります
		//

		//ログを開いている場合は閉じる
		Close();

		//ログフォルダ指定をwide文字で保存
		::ZeroMemory(m_var.logFolder,sizeof(wchar_t) * 1024);
		if(nUtf16 == 0)
			{
			::MultiByteToWideChar(CP_ACP,0,(LPSTR)pLogFolder,-1,(LPWSTR)m_var.logFolder,1024);
			}
		else
			::wcscpy_s(m_var.logFolder,1024,(wchar_t*)pLogFolder);
		//
		return(0);
		}

	//ログファイルを開く
	int32_t Open(bool bFirstTime = true)
		{
		//概要
		//	ログファイルを開く
		//パラメータ
		//	bFirstTime		true..最初のファイルとして開く  false..継続するファイルとして開く
		//戻り値
		//	0				正常にファイルを開きました
		//	0以外			ファイルを開くときにエラーが発生しました
		//

		//既に開いている場合が有るのでログファイルを閉じる
		Close();

		//ログ行数のリセット
		m_var.nLogCount = 0;

		//機能有効？
		if(m_var.nControl & 1)
			{
			//保存ファイル名を構築
			if(bFirstTime)
				{
				//現在の時間を開始時間として保存し、最初のファイル扱いとする
				GetTm(&m_var.firstTime);				
				m_var.nFileCount = 1;
				}
			else
				{
				//2つ目（又はそれ以降）のファイルとして扱う
				++m_var.nFileCount;
				}
			//バッファ
			wchar_t* pTmpBuffer = new wchar_t[2048]();

			//フォルダ名構築
			wsprintf(pTmpBuffer,L"%s\\%04d%02d%02d-%02d%02d%02d"
				,m_var.logFolder
				,m_var.firstTime.sysTime.wYear
				,m_var.firstTime.sysTime.wMonth
				,m_var.firstTime.sysTime.wDay
				,m_var.firstTime.sysTime.wHour
				,m_var.firstTime.sysTime.wMinute
				,m_var.firstTime.sysTime.wSecond
				);
			::CreateDirectoryW(pTmpBuffer,NULL);

			//ファイル名構築
			wsprintf(pTmpBuffer,L"%s\\%04d%02d%02d-%02d%02d%02d\\%06d.txt"
				,m_var.logFolder
				,m_var.firstTime.sysTime.wYear
				,m_var.firstTime.sysTime.wMonth
				,m_var.firstTime.sysTime.wDay
				,m_var.firstTime.sysTime.wHour
				,m_var.firstTime.sysTime.wMinute
				,m_var.firstTime.sysTime.wSecond
				,m_var.nFileCount
				);

			//ファイルを開く
			FILE* fp = NULL;
			::_wfopen_s(&fp,pTmpBuffer,L"wb");
			delete [] pTmpBuffer;

			//
			if(fp == NULL)
				return(-1);

			//ファイルハンドラの保存
			m_var.hLog = fp;
			}
		//
		return(0);
		}

	//ログファイルを閉じる
	int32_t Close(void)
		{
		//概要
		//	ログファイルを閉じる
		//パラメータ
		//	無し
		//戻り値
		//	常に0が戻ります
		//

		//ログ有効ならファイルを閉じる
		if(m_var.hLog)
			{
			::fclose(m_var.hLog);
			m_var.hLog = NULL;
			}
		return(0);
		}

	//ログ機能制御
	int32_t Control(uint8_t nCmd)
		{
		//概要
		//	ログ機能の制御を行う
		//パラメータ
		//	nCmd	bit0..1でログ機能有効（ファイルを開く前に要設定）
		//			bit1..1で送信ログを記録しない（受信のみとなる）
		//			bit2..1で受信ログを記録しない（送信のみとなる）
		//戻り値
		//	常に0が戻ります
		//


		m_var.nControl = nCmd;
		return(0);
		}

	//ログ追加
	int32_t Add(bool bRecv,uint32_t nTimeU32,uint32_t nTimeL32,uint32_t nCanID,char* pSrc,uint8_t nSrcLen)
		{
		//概要
		//	ログファイルにログを追記する
		//パラメータ
		//	bRecv			true:受信データ扱い  false..送信データ扱い
		//	uTimeU32		タイムスタンプの上位32bit値
		//	uTimeL32		タイムスタンプの下位32bit値
		//	nCanID			データに付随するCanID（受信なら発信元、送信なら送信先）
		//	pSrc			データが入ったポインタ
		//	nSrcLen			データの数
		//戻り値
		//	常に0が戻ります
		//注意点
		//	逐次保存の為、ファイルシステムはある程度高い性能が要求されます
		//	制御設定次第で、記録しない場合が有ります
		//	設定行数を超過すると、次のファイルが作られます
		//

		//固定領域としてバッファを確保
		static char tmpBuffer[256];

		//送信指定で、送信ログを記録しない設定？
		if((bRecv == false) && (m_var.nControl & 0x02)) 
			return(0);	//何もしないで戻る
		//受信指定で、受信ログを記録しない設定？
		else if((bRecv != false) && (m_var.nControl & 0x04)) 
			return(0);	//何もしないで戻る

		//排他制御(100ms迄待つ)
		if(::WaitForSingleObject(m_var.hSema,100) == WAIT_OBJECT_0)
			{
			//排他制御獲得

			//ファイルを開いているか？（この確認はファイル更新が絡む為、資源確保してから行う事）
			FILE* hFile = m_var.hLog;
			if(hFile)
				{
				//最初のファイルで最初の行？
				if((m_var.nFileCount == 1) && (m_var.nLogCount == 0))
					{
					//ヘッダ行を送信
					::sprintf_s(tmpBuffer,256,"LocalTimeMS,Dir,Timestamp,CanID,Data\r\n");
					fputs(tmpBuffer,hFile);
					}

				//送信又は受信の文字列
				const char* pDir = "send";
				if(bRecv)
					pDir = "recv";

				//時間、方向、Timestamp,canID迄を作成して送信
				::sprintf_s(tmpBuffer,256,"%d,\"%s\",%08X%08Xh,%08Xh"
					,GetTm() - m_var.firstTime.nTick
					,pDir
					,nTimeU32,nTimeL32
					,nCanID
					);
				fputs(tmpBuffer,hFile);

				//データ保存
				for(uint8_t nLoop = 0;nLoop < nSrcLen;nLoop++)
					{
					sprintf_s(tmpBuffer,256,",%02Xh",pSrc[nLoop] & 0xff);
					fputs(tmpBuffer,hFile);
					}
				fputs("\r\n",hFile);

				//ログ行カウンタ加算したら制限超過？
				if(++m_var.nLogCount >= m_var.nLineLimit)
					{
					//行数超過なので新しいファイルを継続扱いで開く
					Close();
					Open(false);
					}
				}
			else
				{
				//ファイルが開かれていないので、何も記録しない
				}

			//排他制御解放
			::ReleaseSemaphore(m_var.hSema,1,NULL);
			}
		else
			{
			//排他制御獲得失敗したので、何も記録しない
			}

		return(0);
		}

	//次のファイルに移行するログ行数を設定
	int32_t SetLimitRow(uint32_t nLimit = 100000)
		{
		//概要
		//	ログファイルの、最大行数を設定
		//パラメータ
		//	nLimit			行数を指定
		//戻り値
		//	常に0が戻ります
		//解説
		//	ログを追記（Add関数）した時、記録行数がこの設定値以上になると
		//	新しいログファイルを作成する。（次の追記はそのファイルになる）
		//	最初のファイルには項目名が入った先頭行が有るが、2つ目以降の
		//	ファイルには記録されないので、ファイルを全部連結する事が可能となる。
		//	初期値は本クラス構築時に、デフォルトの値が入る。
		//

		//保険
		if(nLimit == 0)
			nLimit = 100000;
		//設定
		m_var.nLineLimit = nLimit;
		//
		return(0);
		}

	//現在時刻を取得
	uint32_t GetTm(pLOGCTRL_TIME pTime = NULL)
		{
		//概要
		//	現在時刻を取得
		//パラメータ
		//	pTime			格納先を指定（指定時はローカル時間も入る）
		//戻り値
		//	[ms]単位の現在時間が戻ります
		//解説
		//	格納先を指定しない場合は、[ms]単位の時間が戻り値となります。
		//	格納先を指定した場合は、格納先の時間要素全てが格納され、
		//	格納した[ms]単位の時間が戻り値となります。
		//

		//現在時間を[ms]単位で取得
		MMTIME x;
		timeGetSystemTime(&x,sizeof(MMTIME));
		uint32_t nTick = uint32_t(x.u.ticks);

		//格納先指定有り？
		if(pTime)
			{
			//ローカル時間を取得して保存
			::GetLocalTime(&pTime->sysTime);
			//現在時間も保存
			pTime->nTick = nTick;
			}
		//[ms]単位の時間が戻り値となる
		return(nTick);
		}

public:
	//コンストラクタ
	CLogctrl()
		{
		//内部変数領域初期化
		::ZeroMemory(&m_var,sizeof(LOGCTRL_VAR));

		//ログ用セマフォ構築
		m_var.hSema = ::CreateSemaphore(NULL,1,1,NULL);

		//ログ最大行数はデフォルト値を設定
		SetLimitRow();
		}

	//デストラクタ
	virtual ~CLogctrl()
		{
		//ログ用セマフォ廃棄
		::DeleteObject(m_var.hSema);
		m_var.hSema = NULL;
		}
};
