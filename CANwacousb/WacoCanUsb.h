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

#pragma once

//標準的なインクルード
#include <Windows.h>
#include <stdint.h>

//継承元クラス
#include "CanIF.h"

//ログ制御用
#include "Logctrl.h"

//本クラスで使用するデバイス用のインクルード

typedef enum
	{
	CMD_NUL = 0x00,	//<nul>
	CMD_SOH = 0x01,	//<soh>
	CMD_STX = 0x02,	//<stx>
	CMD_ETX = 0x03,	//<etx>
	CMD_EOT = 0x04,	//<eot>
	CMD_ENQ = 0x05,	//<enq>
	CMD_ACK = 0x06,	//<ack>
	CMD_BEL = 0x07,	//<bel>
	CMD_BS	= 0x08,	//<bs>
	CMD_HT	= 0x09,	//<ht>
	CMD_NL	= 0x0a,	//<nl>
	CMD_LF	= 0x0a,	//<lf>
	CMD_VT	= 0x0b,	//<vt>
	CMD_NP	= 0x0c,	//<np> 
	CMD_CR	= 0x0d,	//<cr>
	CMD_SO	= 0x0e,	//<so>
	CMD_SI	= 0x0f,	//<si>
	CMD_DLE = 0x10,	//<dle>
	CMD_DC1 = 0x11,	//<dc1>
	CMD_DC2 = 0x12,	//<dc2>
	CMD_DC3 = 0x13,	//<dc3>
	CMD_DC4 = 0x14,	//<dc4>
	CMD_NAK = 0x15,	//<nak>
	CMD_SYN = 0x16,	//<syn>
	CMD_ETB = 0x17,	//<etb>
	CMD_CAN = 0x18,	//<can>
	CMD_EM	= 0x19,	//<em>
	CMD_SUB = 0x1a,	//<sub>
	CMD_EOF	= 0x1a,	//<eof>
	CMD_ESC = 0x1b,	//<esc>
	CMD_FS	= 0x1c,	//<fs>
	CMD_GS	= 0x1d,	//<gs>
	CMD_RS	= 0x1e,	//<rs>
	CMD_US	= 0x1f,	//<us>
	} ANSICHR;


//本クラス定義
class CWacoCanUsb : public CCanIF
{
public:
//================================================================================
//使用CANインターフェース毎に実装が必須の仮想関数
//================================================================================
public:

	//利用可能なCANインターフェース数を取得する場合に呼び出されます
	virtual int32_t OnGetInterfaceCount(void);

	//指定CANインターフェースを開く場合に呼び出されます
	virtual int32_t OnOpenInterface(int32_t nDeviceNum);

	//開いたCANインターフェースを閉じる場合に呼び出されます
	virtual void OnCloseInterface(void);

	//CANインターフェースから受信する場合に呼び出されます
	virtual int32_t OnCanRecv(uint32_t* pCanID,uint8_t* pData8,bool bNoWait = false);

	//CANインターフェースに送信する場合に呼び出されます
	virtual int32_t OnCanSend(uint32_t nCanID,uint8_t* pData8,uint8_t nLength);

	//CANインターフェースにエラーが有るか調べる場合に呼び出されます
	virtual int32_t OnCanGetError(void);

	//CANインターフェースのエラーを解除する場合に呼び出されます
	virtual int32_t OnCanClearError(void);

	//受信バッファをクリアする場合に呼び出されます
	virtual int32_t OnCanRecvClear(void);

	//ログ機能初期化及びログフォルダ設定
	virtual int32_t canLogInit(uint8_t nUtf16,void* pLogFolder)
		{
		//ログを開いている場合は閉じる
		m_log.Close();

		//ログフォルダ指定をwide文字で保存
		m_log.Initialize(nUtf16,pLogFolder);
		//
		return(0);
		}

	//ログ機能制御
	virtual int32_t canLogCtrl(uint8_t nCmd)
		{
		//
		return(m_log.Control(nCmd));
		}


//================================================================================
//内部用（スレッドから利用為にpublic扱い）
//================================================================================
public:
	//
	typedef struct _WACOCANUSB_STATUS
		{
		bool	bCanStart;	//false..stopped or reseted  true..started
		union
			{
			DWORD nRaw;
			struct _errbit_info
				{
				DWORD	nStuff:1;
				DWORD	nForm:1;
				DWORD	nACK:1;
				DWORD	nBit:1;
				DWORD	nCRC:1;
				DWORD	nOther:1;
				DWORD	nDummy:26;
				} info;
			} err;
		} WACOCANUSB_STATUS,*pWACOCANUSB_STATUS;
	//
	typedef struct _RSCANMSG
		{
		uint32_t	nCANid;				//CANID
		uint32_t	nTimeU32;			//Timestamp(上位)、無い場合は0が入る
		uint32_t	nTimeL32;			//Timestamp(下位)、無い場合は0が入る
		uint8_t		nExtendID;			//拡張IDフラグの場合は1が入る
		uint8_t		nDLC;				//データ部の長さ
		uint16_t	nCRC;				//CRC
		uint8_t		nRaw8[8];
		} RSCANMSG,*PRSCANMSG;

	//変数類
	WACOCANUSB_STATUS	m_status;		//本クラスの変数類
	CLogctrl		m_log;				//ログ制御クラス

	//本クラス独自の要素
	HANDLE			m_hSema;			//受信バッファ排他制御用セマフォ
	HANDLE			m_hReadThread;		//受信スレッドハンドル
	UINT			m_nReadThreadUid;
	uint32_t		m_nCanReadPt;
	uint32_t		m_nCanWritePt;
	HANDLE			m_hComm;
	COMMTIMEOUTS	m_timeout;
	DCB				m_dcb;
	LONG			m_lMustQuit;		// quit flag for the receive thread
	PRSCANMSG		m_pCanMsg;

	//非同期受信スレッド
	static unsigned __stdcall ReceiveThread(void* pParam);

	//受信バッファ排他制御要求
	bool LockCanMsg(DWORD nTimeoutMS = INFINITE);

	//受信バッファ排他制御解除
	void UnlockCanMsg(void);

	//受信データの登録処理
	uint32_t AddCanMsg(PRSCANMSG pMsg,int nCount = 1);

	//登録されたCANメッセージを1つ取得
	uint32_t GetCanMsg(PRSCANMSG pMsg);

//================================================================================
//内部用
//================================================================================
protected:

	//パケット構築
	uint8_t* CreatePacket(uint32_t& nPacketLength,uint32_t nCanID,uint8_t* pData8,uint8_t nLength);

	//パケットチェックと格納
	static uint8_t StockPacket(PRSCANMSG pDst,char* pSrc,uint8_t nSrcLen);

public:
	CWacoCanUsb();
	virtual ~CWacoCanUsb();
};
