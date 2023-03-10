/*
 ******************************************************************************
 * Software License Agreement (BSD License)
 *
 *  Copyright (c) 2021-2023, Waco Giken Co., Ltd.
 *  All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions
 *  are met:
 *
 *   * Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above
 *     copyright notice, this list of conditions and the following
 *     disclaimer in the documentation and/o2r other materials provided
 *     with the distribution.
 *   * Neither the name of the Waco Giken nor the names of its
 *     contributors may be used to endorse or promote products derived
 *     from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 *  FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 *  COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 *  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 *  BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 *  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 *  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 *  LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 *  ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 *  POSSIBILITY OF SUCH DAMAGE.
 ******************************************************************************
*/
//========================================
//概要
//========================================
//	WacoCanUsbデバイスを使う為のクラス
//
//========================================
//履歴
//========================================
//2023/03/02	yo0043		新規作成
//
//========================================
//実装に必要な前処理
//========================================
//
//========================================
//使用方法
//========================================
//	基本的な使い方は、サンプルソースを見て下さい
//
//========================================
///デバイス固有
//========================================
//OnOpenInterfaceに渡すデバイスの番号
//	COM1を0とした列挙番号を渡して下さい
//
//========================================

//
#include "pch.h"
#include <tchar.h>
#include <stdint.h>
#include <process.h>
#include "WacoCanUsb.h"
#include "EnumComPortEx.h"
#include "SafeRelease.inl"
#include "Crc.h"

//
#pragma warning(disable : 4996)
#pragma warning(disable : 6385)

//本クラスで扱うデバイスに必須のライブラリ
//#pragma comment(lib,"vciapi.lib")

//====================
//このソース限定
//====================

//HEX文字を数値に変換
uint32_t hex2num(char* pChr,int nLength);
void num2hex(char* pDst,uint32_t nValue,int nChrLength);

//====================

//HEX文字を数値に変換
uint32_t hex2num(char* pChr,int nLength)
	{
	//1文字変換？
	if(nLength == 1)
		{
		uint8_t nChr = *pChr;
		if((nChr >= '0') && (nChr <= '9'))
			return(nChr - '0');
		else if((nChr >= 'a') && (nChr <= 'f'))
			return(nChr - 'a' + 10);
		else if((nChr >= 'A') && (nChr <= 'F'))
			return(nChr - 'A' + 10);
		return(0);
		}
	//
	uint32_t nResult = 0;
	while(nLength)
		{
		nResult <<= 4;	//4bitシフト
		nResult |= hex2num(pChr++,1);
		--nLength;
		}
	return(nResult);
	}

//数値をHEX文字に変換
void num2hex(char* pDst,uint32_t nValue,int nChrLength)
	{
	if(nChrLength == 1)
		{
		uint8_t nValue8 = (uint8_t)(nValue & 0xf);
		if((nValue8 >= 0) && (nValue8 <= 9))
			*pDst = '0' + nValue8;
		else if((nValue8 >= 10) && (nValue8 <= 15))
			*pDst = 'A' + nValue8 - 10;
		else
			*pDst = '0';
		}
	else
		{
		for(int nLoop = 0;nLoop < nChrLength;nLoop++)
			{
			num2hex(pDst,(nValue >> ((nChrLength - nLoop - 1) * 4)),1);
			++pDst;
			}
		}
	}

//コンストラクタ
CWacoCanUsb::CWacoCanUsb()
	{
	//
	::ZeroMemory(&m_status,sizeof(WACOCANUSB_STATUS));
	m_hSema			= NULL;
	m_pCanMsg		= NULL;
	m_hReadThread	= 0;
	m_lMustQuit		= 0;	// quit flag for the receive thread
	m_nCanReadPt	= 0;
	m_nCanWritePt	= 0;

	//受信データ格納用領域の初期化
	m_pCanMsg = new RSCANMSG[256]();

	//m_pBalObject	= 0;	// bus access object
	//m_lSocketNo		= 0;	// socket number
	//m_pCanControl	= 0;	// control interface
	//m_pCanChn		= 0;	// channel interface
	//m_hEventReader	= 0;
	//m_pReader		= 0;
	//m_pWriter		= 0;
	}

//デストラクタ
CWacoCanUsb::~CWacoCanUsb()
	{
	//回線を開いている場合が有るので閉じる処理を行う
	OnCloseInterface();

	//
	SafeDelete(&m_pCanMsg);
	}

//================================================================================
//使用CANインターフェース毎に実装が必須の仮想関数
//================================================================================

//利用可能なCANインターフェース数を取得する場合に呼び出されます
int32_t CWacoCanUsb::OnGetInterfaceCount()
	{
	//概要
	//	指定CANインターフェースの利用可能数を取得する場合に呼び出されます
	//パラメータ
	//	無し
	//戻り値
	//	0			利用可能インターフェースがありません
	//	-1			調査時にエラーが発生しました
	//	上記以外	利用可能なCANインターフェース数
	//

	//todo:CANインターフェースの数を戻す処理を実装して下さい

	//COMポートを列挙
	CEnumComPortEx ECP;
	int32_t nResult = (int32_t)	ECP.UpdateInformation();

	return(nResult);
	}

//指定CANインターフェースを開く場合に呼び出されます
int32_t CWacoCanUsb::OnOpenInterface(int32_t nDeviceNum)
	{
	//概要
	//	指定CANインターフェースを開く場合に呼び出されます
	//パラメータ
	//	nDeviceNum		0開始のインターフェース番号
	//戻り値
	//	0				正常終了
	//	0以外			異常終了
	//注意点
	//	タイムアウト時間は、GetTimeout,GetTimeout_Open関数で取得出来ます

	//todo:CANインターフェースを開く処理を実装して下さい

	//nDeviceNum
	//	0	COM1

	int32_t nResult = 0;
	do
		{
		//通信ポートを開く為の文字列準備
		TCHAR chPort[16];
		::_stprintf(chPort,_T("\\\\.\\COM%d"),nDeviceNum + 1);

		//通信回線をオープンする
		m_hComm = CreateFile(chPort,GENERIC_READ|GENERIC_WRITE,0,0,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,0);
		if(m_hComm == INVALID_HANDLE_VALUE)
			{
			//オープン失敗
			m_hComm = NULL;
			nResult = -1;
			break;
			}

		//I/Oバッファサイズの設定
		int nTxBufSize = 256;
		int nRxBufSize = 256;
		if(SetupComm(m_hComm,nTxBufSize,nRxBufSize) == 0)
			{
			nResult = -2;
			break;
			}

		//タイムアウト情報のセット
		m_timeout.ReadIntervalTimeout			= 0;
		m_timeout.ReadTotalTimeoutMultiplier	= 1;
		m_timeout.ReadTotalTimeoutConstant		= 0;
		m_timeout.WriteTotalTimeoutMultiplier	= 0;
		m_timeout.WriteTotalTimeoutConstant		= 0;
		if(SetCommTimeouts(m_hComm,&m_timeout) == 0)
			{
			nResult = -3;
			break;
			}

		//現在の通信デバイス情報取得及び失敗時の処理
		else if(GetCommState(m_hComm,&m_dcb) == 0)
			{
			nResult = -4;
			break;
			}

		//通信デバイス情報の設定
		m_dcb.DCBlength			= sizeof(DCB);			//この構造体のサイズ
		m_dcb.fBinary			= TRUE;					//バイナリデータの送受信
		m_dcb.BaudRate			= CBR_115200;			//通信速度（115200bps）
		m_dcb.ByteSize			= DATABITS_8;			//データビット数（8bits）
		m_dcb.Parity			= NOPARITY;				//パリティ（無し）
		m_dcb.StopBits			= ONESTOPBIT;			//ストップビット数（1bit）
		m_dcb.fRtsControl		= RTS_CONTROL_DISABLE;	//RTS制御線は制御無し
		m_dcb.fDsrSensitivity	= FALSE;				//DSRがOFFの時に受信データを無視しません
		m_dcb.fDtrControl		= DTR_CONTROL_DISABLE;	//DTR制御線は制御無し
		m_dcb.fOutxCtsFlow		= FALSE;				//ハードフロー制御無し
		m_dcb.fOutxDsrFlow		= FALSE;				//DSR制御無し
		m_dcb.fInX				= FALSE;				//XON/XOFF受信制御は無し
		m_dcb.fOutX				= FALSE;				//XON/XOFF送信制御は無し
		m_dcb.XonChar			= CMD_DC1;				//XON制御時送受信されるキャラクタコード
		m_dcb.XoffChar			= CMD_DC3;				//XOFF制御時に送受信されるキャラクタコード
		m_dcb.XonLim			= 100;					//バッファに入っているデータが100バイトを下回れば送信再開
		m_dcb.XoffLim			= 1500;					//バッファに入っているデータが1500バイトを超えると送信停止
		m_dcb.fParity			= FALSE;				//パリティチェック
		m_dcb.fAbortOnError		= FALSE;				//エラーが発生しても何もしない
		if(SetCommState(m_hComm,&m_dcb) == 0)
			{
			nResult = -5;
			break;
			}

		//その他設定
		EscapeCommFunction(m_hComm,SETDTR);				//DTR制御線をON
		EscapeCommFunction(m_hComm,SETRTS);				//RTS制御線をON

		//完了
		break;
		} while(false);
	//
	if(nResult)
		{
		//オープン失敗なのでクローズ処理を呼び出す
		OnCloseInterface();
		return(nResult);
		}

	//受信データ排他制御用セマフォ
	m_hSema = ::CreateSemaphore(NULL,1,1,NULL);

	//受信スレッド開始
	m_hReadThread = (HANDLE)_beginthreadex(NULL,0,CWacoCanUsb::ReceiveThread,this,0,&m_nReadThreadUid);


	//処理完了
	return(0);
	}

//開いたCANインターフェースを閉じる場合に呼び出されます
void CWacoCanUsb::OnCloseInterface()
	{
	//概要
	//	開いたCANインターフェースを閉じる場合に呼び出されます
	//パラメータ
	//	無し
	//戻り値
	//	無し

	//todo:CANインターフェースを開いている場合に閉じる処理を実装して下さい


	//受信スレッド動作中？
	if (m_hReadThread)
		{
		//受信スレッド停止
		m_lMustQuit = 1;
		::WaitForSingleObject(m_hReadThread,INFINITE);
		m_lMustQuit = 0;
//		::CloseHandle(m_hReadThread);	//閉じると次回スレッド起動出来ない現象確認
		m_hReadThread = NULL;
		}

	//エラーが有る場合は解除する
	OnCanClearError();

	//CAN回線（に相当するCOMポート）を閉じる
	SafeCloseHandle(&m_hComm);

	//受信バッファ排他制御用セマフォ開放
	SafeCloseHandle(&m_hSema);
	}

//CANインターフェースから受信する場合に呼び出されます
int32_t CWacoCanUsb::OnCanRecv(uint32_t* pCanID,uint8_t* pData8,bool bNoWait /* false */)
	{
	//概要
	//	CANインターフェースから1つ分のパケットを受信します
	//パラメータ
	//	pCanID			受信パケットのIDを格納する領域へのポインタ
	//	pData8			受信パケットを格納する8[bytes]領域へのポインタ
	//	bNoWait			trueの場合、既に受信済みパケットのみ対象として受信待ちしません
	//					falseの場合、受信済みパケットが無い場合は、GetTimeoutRecvで取得した時間迄受信を待ちます
	//戻り値
	//	0				正常終了
	//	0以外			異常終了

	//todo:CANインターフェースから1回分の受信を行う処理を実装して下さい

	//戻り値
	int32_t nResult = -1;

	//受信タイムアウト絶対時間を算出
	uint32_t nLimitTime = GetTm() + GetTimeout_Recv();

	//制限時間内のみ処理する（但し最低1回は処理する）
	do
		{
		RSCANMSG msg;
		if (GetCanMsg(&msg) == 0)
			{
			//正常取得
			*pCanID = uint32_t(msg.nCANid);
			::memcpy(pData8,msg.nRaw8,sizeof(uint8_t) * 8);
			nResult = 0;
			break;
			}
		//待ち時間無し？
		if(bNoWait)
			break;
		//時間待ち
		Sleep(1);
		} while(GetTm() < nLimitTime);

	//
	return(nResult);
	}

//CANインターフェースに送信する場合に呼び出されます
int32_t CWacoCanUsb::OnCanSend(uint32_t nCanID,uint8_t* pData8,uint8_t nLength)
	{
	//概要
	//	CANインターフェースに1つ分のパケットを送信します
	//パラメータ
	//	nCanID			送信パケットのID
	//	pData8			送信パケットが格納されている8[bytes]領域へのポインタ
	//	nTimeoutMS		送信許容時間[ms]
	//戻り値
	//	0				正常終了
	//	0以外			異常終了

	//todo:CANインターフェースに送信する処理を実装して下さい

	//パケット構築
	uint32_t nPacketLength = 0;
	uint8_t* pPacket = CreatePacket(nPacketLength,nCanID,pData8,nLength);

	//戻り値
	int32_t nResult = 0;

	//送信
	DWORD nWritesize = 0;
	if(WriteFile(m_hComm,pPacket,(DWORD)nPacketLength,&nWritesize,NULL) == 0)
		{
		//Err
		nResult = 1;
		}
	else
		{
		//カウンタの値に送信サイズ(データ長)を加算
		AddCounter((uint32_t)nLength);
		}

	//送信用一時バッファ開放
	SafeDelete(&pPacket);

	//
	return(nResult);
	}

//CANインターフェースにエラーが有るか調べる場合に呼び出されます
int32_t CWacoCanUsb::OnCanGetError()
	{
	//概要
	//	CANインターフェースにエラーが有るか調べます
	//パラメータ
	//	無し
	//戻り値
	//	0				正常
	//	0以外			異常
	//注意点
	//	このデバイスでは発生したエラーが全て受信データとして通知される

	//todo:CANインターフェースにエラーが有るか調べる処理を実装して下さい

	//内部に格納したエラーが有るか？
	if(m_status.err.nRaw)
		return(1);

	return(0);
	}

//CANインターフェースのエラーを解除する場合に呼び出されます
int32_t CWacoCanUsb::OnCanClearError()
	{
	//概要
	//	CANインターフェースのエラーを解除します
	//パラメータ
	//	無し
	//戻り値
	//	0				正常
	//	0以外			異常

	//todo:CANインターフェースのエラーを解除する処理を実装して下さい

	//排他制御成功？
	if(LockCanMsg())
		{
		//内部に格納したエラー値を解除
		m_status.err.nRaw = 0;
		//排他制御解除
		UnlockCanMsg();
		}
	//デバイスのエラーを解除
	::ClearCommError(m_hComm,NULL,NULL);

	return(0);
	}

//受信バッファをクリアする場合に呼び出されます
int32_t CWacoCanUsb::OnCanRecvClear()
	{
	//概要
	//	受信バッファをクリアする場合に呼び出されます
	//パラメータ
	//	無し
	//戻り値
	//	0				正常
	//	0以外			異常

	//todo:受信バッファをクリアする処理を実装して下さい

	//排他制御成功？
	if(LockCanMsg())
		{
		m_nCanReadPt = 0;
		m_nCanWritePt = 0;
		//排他制御解除
		UnlockCanMsg();
		}
	return(0);
	}

//================================================================================
//本クラス専用の要素
//================================================================================

//非同期受信スレッド
unsigned __stdcall CWacoCanUsb::ReceiveThread(void* Param)
	{
	CWacoCanUsb* pClass = (CWacoCanUsb*)Param;

	//
	uint32_t	nStage = 0;				//受信待ちステージから開始
	RSCANMSG	canMsg;					//登録用バッファ
	DWORD		nMaxRecv = 1024;
	uint8_t		nStockPt = 0;
	char*		pTmpRecv = new char[nMaxRecv]();

	//受信スレッド
	while (pClass->m_lMustQuit == 0)
		{
		//初期化
		if(nStage == 0)
			{

			//次ステージ
			nStage = 1000;
			}

		//[STX]待ち
		if(nStage == 1000)
			{
			//
			DWORD nReadsize = 0;
			char nChr = 0;
			while(-1)
				{
				//受信時にエラー？
				if(ReadFile(pClass->m_hComm,&nChr,1,&nReadsize,NULL) == 0)
					{
					//エラー発生
					nStage = 1050;
					break;
					}
				else
					{
					//何か受信した？
					if(nReadsize)
						{
						//[STX]を取得？
						if(nChr == ANSICHR::CMD_STX)
							{
							//次ステージに遷移
							nStage = 1050;
							break;
							}
						else
							{
							//他の文字を取得
							//	待ち時間無しで取得を継続
							}
						}
					else
						{
						//何も受信してないので、一定時間待って同じステージをやり直し
						Sleep(1);
						break;
						}
					}
				//取得ループ
				}
			}

		//受信バッファ初期化
		else if(nStage == 1050)
			{
			//格納領域と一時バッファ及び保存位置初期化
			::ZeroMemory(&canMsg,sizeof(RSCANMSG));
			::ZeroMemory(pTmpRecv,nMaxRecv);

			//[STX]保存
			pTmpRecv[0] = ANSICHR::CMD_STX;
			nStockPt = 1;

			//次ステージに遷移
			nStage = 1100;
			}

		//[ETX]待ち
		else if(nStage == 1100)
			{
			DWORD nReadsize = 0;
			char nChr = 0;
			while(-1)
				{
				//受信時にエラー？
				if(ReadFile(pClass->m_hComm,&nChr,1,&nReadsize,NULL) == 0)
					{
					//エラー発生
					nStage = 1050;
					break;
					}
				else
					{
					//何か受信した？
					if(nReadsize)
						{
						//格納
						pTmpRecv[nStockPt++] = nChr;

						//バッファ一杯？
						if(nStockPt == nMaxRecv)
							{
							//バッファ先頭にして最初からやり直し
							nStage = 1050;
							break;
							}
						//[STX]を取得？
						else if(nChr == ANSICHR::CMD_STX)
							{
							//[ETX]を読み損ねたか文字化けの可能性が有る為、バッファ初期化して先頭からやり直し
							nStage = 1050;
							break;
							}
						//[ETX]を取得？
						else if(nChr == ANSICHR::CMD_ETX)
							{
							nStage = 1150;
							break;
							}
						}
					else
						{
						//何も受信してないので、一定時間待って同じステージをやり直し
						Sleep(1);
						break;
						}
					}
				//取得ループ
				}
			}

		//
		else if(nStage == 1150)
			{
			//現在は処理無し

			//次ステージ
			nStage = 1200;
			}

		//[STX]...[ETX]がそろったので解析
		else if(nStage == 1200)
			{
			//pTmpRecvには[STX]と[ETX]を含めた受信データが格納されている

			//
			if(StockPacket(&canMsg,pTmpRecv,nStockPt) == 0)
				{
				//カウンタの値に受信サイズ(dlc)を加算
				pClass->AddCounter((uint32_t)canMsg.nDLC);

				//次ステージ
				nStage = 1300;
				}
			else
				{
				//パケットにエラーが有るので今回のパケットは無視してやり直す
				nStage = 1050;
				}
			}
		//解析結果が問題無かったので登録
		else if(nStage == 1300)
			{
			//登録
			pClass->AddCanMsg(&canMsg,1);

			//初期化に戻る
			nStage = 0;
			}
		Sleep(0);
		}
	//
	SafeDelete(&pTmpRecv);
	//
	_endthreadex(0);
	return(0);
	}

//受信バッファ排他制御要求
bool CWacoCanUsb::LockCanMsg(DWORD nTimeoutMS /* INFINITE */)
	{
	//資源確保に失敗？
	if(::WaitForSingleObject(m_hSema,nTimeoutMS) != WAIT_OBJECT_0)
		return(false);	//確保失敗
	//確保成功
	return(true);
	}

//受信バッファ排他制御解除
void CWacoCanUsb::UnlockCanMsg()
	{
	//確保した資源を開放
	::ReleaseSemaphore(m_hSema,1,NULL);
	}

//受信データの登録処理
uint32_t CWacoCanUsb::AddCanMsg(PRSCANMSG pMsg,int nCount /* 1 */)
	{
	//資源占有
	if(!LockCanMsg())
		return(0);	//資源占有失敗時は、登録数0として戻る

	//
	uint32_t nResult = 0;
	while(nCount)
		{
		//このパケットは無視しない対象か？
		if(!IsIgnoreID(pMsg->nCANid))
			{
			//バッファFULLでは無い？
			if(m_nCanWritePt < 255)
				{
				//格納
				::CopyMemory(&m_pCanMsg[m_nCanWritePt],pMsg,sizeof(RSCANMSG));
				//格納先更新
				++m_nCanWritePt;
				//格納数更新
				++nResult;
				}
			else
				{
				//バッファが一杯
				Sleep(0);
				}
			}
		else
			{
			//無視する対象
			Sleep(0);
			}

		//登録要素の次位置
		++pMsg;

		//登録要素の残数を更新
		--nCount;
		}
	//資源開放
	UnlockCanMsg();
	//
	return(nResult);
	}

//登録されたCANメッセージを1つ取得
uint32_t CWacoCanUsb::GetCanMsg(PRSCANMSG pMsg)
	{
	//
	uint32_t nResult = 0;
	//資源占有
	if(!LockCanMsg())
		return(2);	//資源占有失敗時は、その他エラーとして戻る
	//未読み出しの要素が有る？
	else if(m_nCanReadPt < m_nCanWritePt)
		{
		//該当箇所から取得
		::CopyMemory(pMsg,&m_pCanMsg[m_nCanReadPt],sizeof(RSCANMSG));
		//読み出し位置の更新
		++m_nCanReadPt;
		//読み出し残りが無い？
		if(m_nCanReadPt >= m_nCanWritePt)
			{
			//初期化（全クリア扱いでバッファを先頭に戻すが、排他制御済みなのでClearCanMsgを使わない事）
			m_nCanReadPt = 0;
			m_nCanWritePt = 0;
			}
		nResult = 0;
		}
	else
		nResult = 1;

	//資源開放
	UnlockCanMsg();
	//
	return(nResult);
	}

//パケット構築
uint8_t* CWacoCanUsb::CreatePacket(uint32_t& nPacketLength,uint32_t nCanID,uint8_t* pData8,uint8_t nLength)
	{
	//構造
	//	[STX.1][Flag.2][CANid.8][Data.nLength][CRC.w][ETX.1]
	//例
	//	 02 31 38 30 30 45 46 30 31 30 32 43 33 30 39 43 33 30 39 30 30 30 30 30 30 30 30 43 43 34 42 03
	//
	//想定される全体のデータ長
	uint8_t nDataLength = 1 + 2 + 8 + (nLength * 2) + (2 * 2) + 1;	//max:32
	//バッファ構築
	char* pBuffer = new char[nDataLength]();
	//構築
	int nPt = 0;
	pBuffer[nPt++] = ANSICHR::CMD_STX;
	num2hex(&pBuffer[nPt],0x10 + nLength,2);
	nPt += 2;
	num2hex(&pBuffer[nPt],nCanID,8);
	nPt += 8;
	for(uint8_t nLoop = 0;nLoop < nLength;nLoop++)
		{
		num2hex(&pBuffer[nPt],pData8[nLoop],2);
		nPt += 2;
		}
	CCrc16 CRC(0x1021);
	uint16_t nCRC = (uint16_t)CRC.CalcCRC(0xffff,(PBYTE)&pBuffer[1],nPt - 1);
	num2hex(&pBuffer[nPt],nCRC,4);
	nPt += 4;
	pBuffer[nPt++] = ANSICHR::CMD_ETX;

	//パケット長
	nPacketLength = nPt;

	//
	return((uint8_t*)pBuffer);
	}

//パケットチェックと格納
uint8_t CWacoCanUsb::StockPacket(PRSCANMSG pDst,char* pSrc,uint8_t nSrcLen)
	{
	//要素番号[1-2]からフラグ類を抽出し、予定のパケット全体サイズを作成
	uint8_t nFlag = (uint8_t)hex2num(&pSrc[1],2);
	uint8_t nExtendID = (nFlag >> 4) & 1;
	uint8_t nDLC = nFlag & 0x0f;
	uint8_t nLength = 1 + 2 + 8 + (nDLC * 2) + (2 * 2) + 1;	//max:32

	//全体サイズと一致しない？
	if(nSrcLen != nLength)
		return(1);	//全体サイズと一致しない


	//格納:拡張ID
	pDst->nExtendID = nExtendID;

	//格納:CANID[3-10]
	pDst->nCANid = hex2num(&pSrc[3],8);

	//格納:データ長
	pDst->nDLC = nDLC;

	//格納:DATA[11-X]
	for(uint8_t nLoop = 0;nLoop < nDLC;nLoop++)
		pDst->nRaw8[nLoop] = (uint8_t)hex2num(&pSrc[11 + (nLoop * 2)],2);

	//格納:CRC[X+1 - X+5]
	pDst->nCRC = hex2num(&pSrc[11 + (nDLC * 2)],4);

	//
	return(0);
	}
