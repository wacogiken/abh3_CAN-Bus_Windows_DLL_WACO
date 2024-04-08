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

//�W���I�ȃC���N���[�h
#include <Windows.h>
#include <stdint.h>

//�p�����N���X
#include "CanIF.h"

//���O����p
#include "Logctrl.h"

//�{�N���X�Ŏg�p����f�o�C�X�p�̃C���N���[�h

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


//�{�N���X��`
class CWacoCanUsb : public CCanIF
{
public:
//================================================================================
//�g�pCAN�C���^�[�t�F�[�X���Ɏ������K�{�̉��z�֐�
//================================================================================
public:

	//���p�\��CAN�C���^�[�t�F�[�X�����擾����ꍇ�ɌĂяo����܂�
	virtual int32_t OnGetInterfaceCount(void);

	//�w��CAN�C���^�[�t�F�[�X���J���ꍇ�ɌĂяo����܂�
	virtual int32_t OnOpenInterface(int32_t nDeviceNum);

	//�J����CAN�C���^�[�t�F�[�X�����ꍇ�ɌĂяo����܂�
	virtual void OnCloseInterface(void);

	//CAN�C���^�[�t�F�[�X�����M����ꍇ�ɌĂяo����܂�
	virtual int32_t OnCanRecv(uint32_t* pCanID,uint8_t* pData8,bool bNoWait = false);

	//CAN�C���^�[�t�F�[�X�ɑ��M����ꍇ�ɌĂяo����܂�
	virtual int32_t OnCanSend(uint32_t nCanID,uint8_t* pData8,uint8_t nLength);

	//CAN�C���^�[�t�F�[�X�ɃG���[���L�邩���ׂ�ꍇ�ɌĂяo����܂�
	virtual int32_t OnCanGetError(void);

	//CAN�C���^�[�t�F�[�X�̃G���[����������ꍇ�ɌĂяo����܂�
	virtual int32_t OnCanClearError(void);

	//��M�o�b�t�@���N���A����ꍇ�ɌĂяo����܂�
	virtual int32_t OnCanRecvClear(void);

	//���O�@�\�������y�у��O�t�H���_�ݒ�
	virtual int32_t canLogInit(uint8_t nUtf16,void* pLogFolder)
		{
		//���O���J���Ă���ꍇ�͕���
		m_log.Close();

		//���O�t�H���_�w���wide�����ŕۑ�
		m_log.Initialize(nUtf16,pLogFolder);
		//
		return(0);
		}

	//���O�@�\����
	virtual int32_t canLogCtrl(uint8_t nCmd)
		{
		//
		return(m_log.Control(nCmd));
		}


//================================================================================
//�����p�i�X���b�h���痘�p�ׂ�public�����j
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
		uint32_t	nTimeU32;			//Timestamp(���)�A�����ꍇ��0������
		uint32_t	nTimeL32;			//Timestamp(����)�A�����ꍇ��0������
		uint8_t		nExtendID;			//�g��ID�t���O�̏ꍇ��1������
		uint8_t		nDLC;				//�f�[�^���̒���
		uint16_t	nCRC;				//CRC
		uint8_t		nRaw8[8];
		} RSCANMSG,*PRSCANMSG;

	//�ϐ���
	WACOCANUSB_STATUS	m_status;		//�{�N���X�̕ϐ���
	CLogctrl		m_log;				//���O����N���X

	//�{�N���X�Ǝ��̗v�f
	HANDLE			m_hSema;			//��M�o�b�t�@�r������p�Z�}�t�H
	HANDLE			m_hReadThread;		//��M�X���b�h�n���h��
	UINT			m_nReadThreadUid;
	uint32_t		m_nCanReadPt;
	uint32_t		m_nCanWritePt;
	HANDLE			m_hComm;
	COMMTIMEOUTS	m_timeout;
	DCB				m_dcb;
	LONG			m_lMustQuit;		// quit flag for the receive thread
	PRSCANMSG		m_pCanMsg;

	//�񓯊���M�X���b�h
	static unsigned __stdcall ReceiveThread(void* pParam);

	//��M�o�b�t�@�r������v��
	bool LockCanMsg(DWORD nTimeoutMS = INFINITE);

	//��M�o�b�t�@�r���������
	void UnlockCanMsg(void);

	//��M�f�[�^�̓o�^����
	uint32_t AddCanMsg(PRSCANMSG pMsg,int nCount = 1);

	//�o�^���ꂽCAN���b�Z�[�W��1�擾
	uint32_t GetCanMsg(PRSCANMSG pMsg);

//================================================================================
//�����p
//================================================================================
protected:

	//�p�P�b�g�\�z
	uint8_t* CreatePacket(uint32_t& nPacketLength,uint32_t nCanID,uint8_t* pData8,uint8_t nLength);

	//�p�P�b�g�`�F�b�N�Ɗi�[
	static uint8_t StockPacket(PRSCANMSG pDst,char* pSrc,uint8_t nSrcLen);

public:
	CWacoCanUsb();
	virtual ~CWacoCanUsb();
};
