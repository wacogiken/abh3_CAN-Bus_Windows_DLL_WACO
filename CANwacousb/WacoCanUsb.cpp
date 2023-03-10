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
//�T�v
//========================================
//	WacoCanUsb�f�o�C�X���g���ׂ̃N���X
//
//========================================
//����
//========================================
//2023/03/02	yo0043		�V�K�쐬
//
//========================================
//�����ɕK�v�ȑO����
//========================================
//
//========================================
//�g�p���@
//========================================
//	��{�I�Ȏg�����́A�T���v���\�[�X�����ĉ�����
//
//========================================
///�f�o�C�X�ŗL
//========================================
//OnOpenInterface�ɓn���f�o�C�X�̔ԍ�
//	COM1��0�Ƃ����񋓔ԍ���n���ĉ�����
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

//�{�N���X�ň����f�o�C�X�ɕK�{�̃��C�u����
//#pragma comment(lib,"vciapi.lib")

//====================
//���̃\�[�X����
//====================

//HEX�����𐔒l�ɕϊ�
uint32_t hex2num(char* pChr,int nLength);
void num2hex(char* pDst,uint32_t nValue,int nChrLength);

//====================

//HEX�����𐔒l�ɕϊ�
uint32_t hex2num(char* pChr,int nLength)
	{
	//1�����ϊ��H
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
		nResult <<= 4;	//4bit�V�t�g
		nResult |= hex2num(pChr++,1);
		--nLength;
		}
	return(nResult);
	}

//���l��HEX�����ɕϊ�
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

//�R���X�g���N�^
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

	//��M�f�[�^�i�[�p�̈�̏�����
	m_pCanMsg = new RSCANMSG[256]();

	//m_pBalObject	= 0;	// bus access object
	//m_lSocketNo		= 0;	// socket number
	//m_pCanControl	= 0;	// control interface
	//m_pCanChn		= 0;	// channel interface
	//m_hEventReader	= 0;
	//m_pReader		= 0;
	//m_pWriter		= 0;
	}

//�f�X�g���N�^
CWacoCanUsb::~CWacoCanUsb()
	{
	//������J���Ă���ꍇ���L��̂ŕ��鏈�����s��
	OnCloseInterface();

	//
	SafeDelete(&m_pCanMsg);
	}

//================================================================================
//�g�pCAN�C���^�[�t�F�[�X���Ɏ������K�{�̉��z�֐�
//================================================================================

//���p�\��CAN�C���^�[�t�F�[�X�����擾����ꍇ�ɌĂяo����܂�
int32_t CWacoCanUsb::OnGetInterfaceCount()
	{
	//�T�v
	//	�w��CAN�C���^�[�t�F�[�X�̗��p�\�����擾����ꍇ�ɌĂяo����܂�
	//�p�����[�^
	//	����
	//�߂�l
	//	0			���p�\�C���^�[�t�F�[�X������܂���
	//	-1			�������ɃG���[���������܂���
	//	��L�ȊO	���p�\��CAN�C���^�[�t�F�[�X��
	//

	//todo:CAN�C���^�[�t�F�[�X�̐���߂��������������ĉ�����

	//COM�|�[�g���
	CEnumComPortEx ECP;
	int32_t nResult = (int32_t)	ECP.UpdateInformation();

	return(nResult);
	}

//�w��CAN�C���^�[�t�F�[�X���J���ꍇ�ɌĂяo����܂�
int32_t CWacoCanUsb::OnOpenInterface(int32_t nDeviceNum)
	{
	//�T�v
	//	�w��CAN�C���^�[�t�F�[�X���J���ꍇ�ɌĂяo����܂�
	//�p�����[�^
	//	nDeviceNum		0�J�n�̃C���^�[�t�F�[�X�ԍ�
	//�߂�l
	//	0				����I��
	//	0�ȊO			�ُ�I��
	//���ӓ_
	//	�^�C���A�E�g���Ԃ́AGetTimeout,GetTimeout_Open�֐��Ŏ擾�o���܂�

	//todo:CAN�C���^�[�t�F�[�X���J���������������ĉ�����

	//nDeviceNum
	//	0	COM1

	int32_t nResult = 0;
	do
		{
		//�ʐM�|�[�g���J���ׂ̕����񏀔�
		TCHAR chPort[16];
		::_stprintf(chPort,_T("\\\\.\\COM%d"),nDeviceNum + 1);

		//�ʐM������I�[�v������
		m_hComm = CreateFile(chPort,GENERIC_READ|GENERIC_WRITE,0,0,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,0);
		if(m_hComm == INVALID_HANDLE_VALUE)
			{
			//�I�[�v�����s
			m_hComm = NULL;
			nResult = -1;
			break;
			}

		//I/O�o�b�t�@�T�C�Y�̐ݒ�
		int nTxBufSize = 256;
		int nRxBufSize = 256;
		if(SetupComm(m_hComm,nTxBufSize,nRxBufSize) == 0)
			{
			nResult = -2;
			break;
			}

		//�^�C���A�E�g���̃Z�b�g
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

		//���݂̒ʐM�f�o�C�X���擾�y�ю��s���̏���
		else if(GetCommState(m_hComm,&m_dcb) == 0)
			{
			nResult = -4;
			break;
			}

		//�ʐM�f�o�C�X���̐ݒ�
		m_dcb.DCBlength			= sizeof(DCB);			//���̍\���̂̃T�C�Y
		m_dcb.fBinary			= TRUE;					//�o�C�i���f�[�^�̑���M
		m_dcb.BaudRate			= CBR_115200;			//�ʐM���x�i115200bps�j
		m_dcb.ByteSize			= DATABITS_8;			//�f�[�^�r�b�g���i8bits�j
		m_dcb.Parity			= NOPARITY;				//�p���e�B�i�����j
		m_dcb.StopBits			= ONESTOPBIT;			//�X�g�b�v�r�b�g���i1bit�j
		m_dcb.fRtsControl		= RTS_CONTROL_DISABLE;	//RTS������͐��䖳��
		m_dcb.fDsrSensitivity	= FALSE;				//DSR��OFF�̎��Ɏ�M�f�[�^�𖳎����܂���
		m_dcb.fDtrControl		= DTR_CONTROL_DISABLE;	//DTR������͐��䖳��
		m_dcb.fOutxCtsFlow		= FALSE;				//�n�[�h�t���[���䖳��
		m_dcb.fOutxDsrFlow		= FALSE;				//DSR���䖳��
		m_dcb.fInX				= FALSE;				//XON/XOFF��M����͖���
		m_dcb.fOutX				= FALSE;				//XON/XOFF���M����͖���
		m_dcb.XonChar			= CMD_DC1;				//XON���䎞����M�����L�����N�^�R�[�h
		m_dcb.XoffChar			= CMD_DC3;				//XOFF���䎞�ɑ���M�����L�����N�^�R�[�h
		m_dcb.XonLim			= 100;					//�o�b�t�@�ɓ����Ă���f�[�^��100�o�C�g�������Α��M�ĊJ
		m_dcb.XoffLim			= 1500;					//�o�b�t�@�ɓ����Ă���f�[�^��1500�o�C�g�𒴂���Ƒ��M��~
		m_dcb.fParity			= FALSE;				//�p���e�B�`�F�b�N
		m_dcb.fAbortOnError		= FALSE;				//�G���[���������Ă��������Ȃ�
		if(SetCommState(m_hComm,&m_dcb) == 0)
			{
			nResult = -5;
			break;
			}

		//���̑��ݒ�
		EscapeCommFunction(m_hComm,SETDTR);				//DTR�������ON
		EscapeCommFunction(m_hComm,SETRTS);				//RTS�������ON

		//����
		break;
		} while(false);
	//
	if(nResult)
		{
		//�I�[�v�����s�Ȃ̂ŃN���[�Y�������Ăяo��
		OnCloseInterface();
		return(nResult);
		}

	//��M�f�[�^�r������p�Z�}�t�H
	m_hSema = ::CreateSemaphore(NULL,1,1,NULL);

	//��M�X���b�h�J�n
	m_hReadThread = (HANDLE)_beginthreadex(NULL,0,CWacoCanUsb::ReceiveThread,this,0,&m_nReadThreadUid);


	//��������
	return(0);
	}

//�J����CAN�C���^�[�t�F�[�X�����ꍇ�ɌĂяo����܂�
void CWacoCanUsb::OnCloseInterface()
	{
	//�T�v
	//	�J����CAN�C���^�[�t�F�[�X�����ꍇ�ɌĂяo����܂�
	//�p�����[�^
	//	����
	//�߂�l
	//	����

	//todo:CAN�C���^�[�t�F�[�X���J���Ă���ꍇ�ɕ��鏈�����������ĉ�����


	//��M�X���b�h���쒆�H
	if (m_hReadThread)
		{
		//��M�X���b�h��~
		m_lMustQuit = 1;
		::WaitForSingleObject(m_hReadThread,INFINITE);
		m_lMustQuit = 0;
//		::CloseHandle(m_hReadThread);	//����Ǝ���X���b�h�N���o���Ȃ����ۊm�F
		m_hReadThread = NULL;
		}

	//�G���[���L��ꍇ�͉�������
	OnCanClearError();

	//CAN����i�ɑ�������COM�|�[�g�j�����
	SafeCloseHandle(&m_hComm);

	//��M�o�b�t�@�r������p�Z�}�t�H�J��
	SafeCloseHandle(&m_hSema);
	}

//CAN�C���^�[�t�F�[�X�����M����ꍇ�ɌĂяo����܂�
int32_t CWacoCanUsb::OnCanRecv(uint32_t* pCanID,uint8_t* pData8,bool bNoWait /* false */)
	{
	//�T�v
	//	CAN�C���^�[�t�F�[�X����1���̃p�P�b�g����M���܂�
	//�p�����[�^
	//	pCanID			��M�p�P�b�g��ID���i�[����̈�ւ̃|�C���^
	//	pData8			��M�p�P�b�g���i�[����8[bytes]�̈�ւ̃|�C���^
	//	bNoWait			true�̏ꍇ�A���Ɏ�M�ς݃p�P�b�g�̂ݑΏۂƂ��Ď�M�҂����܂���
	//					false�̏ꍇ�A��M�ς݃p�P�b�g�������ꍇ�́AGetTimeoutRecv�Ŏ擾�������Ԗ���M��҂��܂�
	//�߂�l
	//	0				����I��
	//	0�ȊO			�ُ�I��

	//todo:CAN�C���^�[�t�F�[�X����1�񕪂̎�M���s���������������ĉ�����

	//�߂�l
	int32_t nResult = -1;

	//��M�^�C���A�E�g��Ύ��Ԃ��Z�o
	uint32_t nLimitTime = GetTm() + GetTimeout_Recv();

	//�������ԓ��̂ݏ�������i�A���Œ�1��͏�������j
	do
		{
		RSCANMSG msg;
		if (GetCanMsg(&msg) == 0)
			{
			//����擾
			*pCanID = uint32_t(msg.nCANid);
			::memcpy(pData8,msg.nRaw8,sizeof(uint8_t) * 8);
			nResult = 0;
			break;
			}
		//�҂����Ԗ����H
		if(bNoWait)
			break;
		//���ԑ҂�
		Sleep(1);
		} while(GetTm() < nLimitTime);

	//
	return(nResult);
	}

//CAN�C���^�[�t�F�[�X�ɑ��M����ꍇ�ɌĂяo����܂�
int32_t CWacoCanUsb::OnCanSend(uint32_t nCanID,uint8_t* pData8,uint8_t nLength)
	{
	//�T�v
	//	CAN�C���^�[�t�F�[�X��1���̃p�P�b�g�𑗐M���܂�
	//�p�����[�^
	//	nCanID			���M�p�P�b�g��ID
	//	pData8			���M�p�P�b�g���i�[����Ă���8[bytes]�̈�ւ̃|�C���^
	//	nTimeoutMS		���M���e����[ms]
	//�߂�l
	//	0				����I��
	//	0�ȊO			�ُ�I��

	//todo:CAN�C���^�[�t�F�[�X�ɑ��M���鏈�����������ĉ�����

	//�p�P�b�g�\�z
	uint32_t nPacketLength = 0;
	uint8_t* pPacket = CreatePacket(nPacketLength,nCanID,pData8,nLength);

	//�߂�l
	int32_t nResult = 0;

	//���M
	DWORD nWritesize = 0;
	if(WriteFile(m_hComm,pPacket,(DWORD)nPacketLength,&nWritesize,NULL) == 0)
		{
		//Err
		nResult = 1;
		}
	else
		{
		//�J�E���^�̒l�ɑ��M�T�C�Y(�f�[�^��)�����Z
		AddCounter((uint32_t)nLength);
		}

	//���M�p�ꎞ�o�b�t�@�J��
	SafeDelete(&pPacket);

	//
	return(nResult);
	}

//CAN�C���^�[�t�F�[�X�ɃG���[���L�邩���ׂ�ꍇ�ɌĂяo����܂�
int32_t CWacoCanUsb::OnCanGetError()
	{
	//�T�v
	//	CAN�C���^�[�t�F�[�X�ɃG���[���L�邩���ׂ܂�
	//�p�����[�^
	//	����
	//�߂�l
	//	0				����
	//	0�ȊO			�ُ�
	//���ӓ_
	//	���̃f�o�C�X�ł͔��������G���[���S�Ď�M�f�[�^�Ƃ��Ēʒm�����

	//todo:CAN�C���^�[�t�F�[�X�ɃG���[���L�邩���ׂ鏈�����������ĉ�����

	//�����Ɋi�[�����G���[���L�邩�H
	if(m_status.err.nRaw)
		return(1);

	return(0);
	}

//CAN�C���^�[�t�F�[�X�̃G���[����������ꍇ�ɌĂяo����܂�
int32_t CWacoCanUsb::OnCanClearError()
	{
	//�T�v
	//	CAN�C���^�[�t�F�[�X�̃G���[���������܂�
	//�p�����[�^
	//	����
	//�߂�l
	//	0				����
	//	0�ȊO			�ُ�

	//todo:CAN�C���^�[�t�F�[�X�̃G���[���������鏈�����������ĉ�����

	//�r�����䐬���H
	if(LockCanMsg())
		{
		//�����Ɋi�[�����G���[�l������
		m_status.err.nRaw = 0;
		//�r���������
		UnlockCanMsg();
		}
	//�f�o�C�X�̃G���[������
	::ClearCommError(m_hComm,NULL,NULL);

	return(0);
	}

//��M�o�b�t�@���N���A����ꍇ�ɌĂяo����܂�
int32_t CWacoCanUsb::OnCanRecvClear()
	{
	//�T�v
	//	��M�o�b�t�@���N���A����ꍇ�ɌĂяo����܂�
	//�p�����[�^
	//	����
	//�߂�l
	//	0				����
	//	0�ȊO			�ُ�

	//todo:��M�o�b�t�@���N���A���鏈�����������ĉ�����

	//�r�����䐬���H
	if(LockCanMsg())
		{
		m_nCanReadPt = 0;
		m_nCanWritePt = 0;
		//�r���������
		UnlockCanMsg();
		}
	return(0);
	}

//================================================================================
//�{�N���X��p�̗v�f
//================================================================================

//�񓯊���M�X���b�h
unsigned __stdcall CWacoCanUsb::ReceiveThread(void* Param)
	{
	CWacoCanUsb* pClass = (CWacoCanUsb*)Param;

	//
	uint32_t	nStage = 0;				//��M�҂��X�e�[�W����J�n
	RSCANMSG	canMsg;					//�o�^�p�o�b�t�@
	DWORD		nMaxRecv = 1024;
	uint8_t		nStockPt = 0;
	char*		pTmpRecv = new char[nMaxRecv]();

	//��M�X���b�h
	while (pClass->m_lMustQuit == 0)
		{
		//������
		if(nStage == 0)
			{

			//���X�e�[�W
			nStage = 1000;
			}

		//[STX]�҂�
		if(nStage == 1000)
			{
			//
			DWORD nReadsize = 0;
			char nChr = 0;
			while(-1)
				{
				//��M���ɃG���[�H
				if(ReadFile(pClass->m_hComm,&nChr,1,&nReadsize,NULL) == 0)
					{
					//�G���[����
					nStage = 1050;
					break;
					}
				else
					{
					//������M�����H
					if(nReadsize)
						{
						//[STX]���擾�H
						if(nChr == ANSICHR::CMD_STX)
							{
							//���X�e�[�W�ɑJ��
							nStage = 1050;
							break;
							}
						else
							{
							//���̕������擾
							//	�҂����Ԗ����Ŏ擾���p��
							}
						}
					else
						{
						//������M���ĂȂ��̂ŁA��莞�ԑ҂��ē����X�e�[�W����蒼��
						Sleep(1);
						break;
						}
					}
				//�擾���[�v
				}
			}

		//��M�o�b�t�@������
		else if(nStage == 1050)
			{
			//�i�[�̈�ƈꎞ�o�b�t�@�y�ѕۑ��ʒu������
			::ZeroMemory(&canMsg,sizeof(RSCANMSG));
			::ZeroMemory(pTmpRecv,nMaxRecv);

			//[STX]�ۑ�
			pTmpRecv[0] = ANSICHR::CMD_STX;
			nStockPt = 1;

			//���X�e�[�W�ɑJ��
			nStage = 1100;
			}

		//[ETX]�҂�
		else if(nStage == 1100)
			{
			DWORD nReadsize = 0;
			char nChr = 0;
			while(-1)
				{
				//��M���ɃG���[�H
				if(ReadFile(pClass->m_hComm,&nChr,1,&nReadsize,NULL) == 0)
					{
					//�G���[����
					nStage = 1050;
					break;
					}
				else
					{
					//������M�����H
					if(nReadsize)
						{
						//�i�[
						pTmpRecv[nStockPt++] = nChr;

						//�o�b�t�@��t�H
						if(nStockPt == nMaxRecv)
							{
							//�o�b�t�@�擪�ɂ��čŏ������蒼��
							nStage = 1050;
							break;
							}
						//[STX]���擾�H
						else if(nChr == ANSICHR::CMD_STX)
							{
							//[ETX]��ǂݑ��˂������������̉\�����L��ׁA�o�b�t�@���������Đ擪�����蒼��
							nStage = 1050;
							break;
							}
						//[ETX]���擾�H
						else if(nChr == ANSICHR::CMD_ETX)
							{
							nStage = 1150;
							break;
							}
						}
					else
						{
						//������M���ĂȂ��̂ŁA��莞�ԑ҂��ē����X�e�[�W����蒼��
						Sleep(1);
						break;
						}
					}
				//�擾���[�v
				}
			}

		//
		else if(nStage == 1150)
			{
			//���݂͏�������

			//���X�e�[�W
			nStage = 1200;
			}

		//[STX]...[ETX]����������̂ŉ��
		else if(nStage == 1200)
			{
			//pTmpRecv�ɂ�[STX]��[ETX]���܂߂���M�f�[�^���i�[����Ă���

			//
			if(StockPacket(&canMsg,pTmpRecv,nStockPt) == 0)
				{
				//�J�E���^�̒l�Ɏ�M�T�C�Y(dlc)�����Z
				pClass->AddCounter((uint32_t)canMsg.nDLC);

				//���X�e�[�W
				nStage = 1300;
				}
			else
				{
				//�p�P�b�g�ɃG���[���L��̂ō���̃p�P�b�g�͖������Ă�蒼��
				nStage = 1050;
				}
			}
		//��͌��ʂ���薳�������̂œo�^
		else if(nStage == 1300)
			{
			//�o�^
			pClass->AddCanMsg(&canMsg,1);

			//�������ɖ߂�
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

//��M�o�b�t�@�r������v��
bool CWacoCanUsb::LockCanMsg(DWORD nTimeoutMS /* INFINITE */)
	{
	//�����m�ۂɎ��s�H
	if(::WaitForSingleObject(m_hSema,nTimeoutMS) != WAIT_OBJECT_0)
		return(false);	//�m�ێ��s
	//�m�ې���
	return(true);
	}

//��M�o�b�t�@�r���������
void CWacoCanUsb::UnlockCanMsg()
	{
	//�m�ۂ����������J��
	::ReleaseSemaphore(m_hSema,1,NULL);
	}

//��M�f�[�^�̓o�^����
uint32_t CWacoCanUsb::AddCanMsg(PRSCANMSG pMsg,int nCount /* 1 */)
	{
	//������L
	if(!LockCanMsg())
		return(0);	//������L���s���́A�o�^��0�Ƃ��Ė߂�

	//
	uint32_t nResult = 0;
	while(nCount)
		{
		//���̃p�P�b�g�͖������Ȃ��Ώۂ��H
		if(!IsIgnoreID(pMsg->nCANid))
			{
			//�o�b�t�@FULL�ł͖����H
			if(m_nCanWritePt < 255)
				{
				//�i�[
				::CopyMemory(&m_pCanMsg[m_nCanWritePt],pMsg,sizeof(RSCANMSG));
				//�i�[��X�V
				++m_nCanWritePt;
				//�i�[���X�V
				++nResult;
				}
			else
				{
				//�o�b�t�@����t
				Sleep(0);
				}
			}
		else
			{
			//��������Ώ�
			Sleep(0);
			}

		//�o�^�v�f�̎��ʒu
		++pMsg;

		//�o�^�v�f�̎c�����X�V
		--nCount;
		}
	//�����J��
	UnlockCanMsg();
	//
	return(nResult);
	}

//�o�^���ꂽCAN���b�Z�[�W��1�擾
uint32_t CWacoCanUsb::GetCanMsg(PRSCANMSG pMsg)
	{
	//
	uint32_t nResult = 0;
	//������L
	if(!LockCanMsg())
		return(2);	//������L���s���́A���̑��G���[�Ƃ��Ė߂�
	//���ǂݏo���̗v�f���L��H
	else if(m_nCanReadPt < m_nCanWritePt)
		{
		//�Y���ӏ�����擾
		::CopyMemory(pMsg,&m_pCanMsg[m_nCanReadPt],sizeof(RSCANMSG));
		//�ǂݏo���ʒu�̍X�V
		++m_nCanReadPt;
		//�ǂݏo���c�肪�����H
		if(m_nCanReadPt >= m_nCanWritePt)
			{
			//�������i�S�N���A�����Ńo�b�t�@��擪�ɖ߂����A�r������ς݂Ȃ̂�ClearCanMsg���g��Ȃ����j
			m_nCanReadPt = 0;
			m_nCanWritePt = 0;
			}
		nResult = 0;
		}
	else
		nResult = 1;

	//�����J��
	UnlockCanMsg();
	//
	return(nResult);
	}

//�p�P�b�g�\�z
uint8_t* CWacoCanUsb::CreatePacket(uint32_t& nPacketLength,uint32_t nCanID,uint8_t* pData8,uint8_t nLength)
	{
	//�\��
	//	[STX.1][Flag.2][CANid.8][Data.nLength][CRC.w][ETX.1]
	//��
	//	 02 31 38 30 30 45 46 30 31 30 32 43 33 30 39 43 33 30 39 30 30 30 30 30 30 30 30 43 43 34 42 03
	//
	//�z�肳���S�̂̃f�[�^��
	uint8_t nDataLength = 1 + 2 + 8 + (nLength * 2) + (2 * 2) + 1;	//max:32
	//�o�b�t�@�\�z
	char* pBuffer = new char[nDataLength]();
	//�\�z
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

	//�p�P�b�g��
	nPacketLength = nPt;

	//
	return((uint8_t*)pBuffer);
	}

//�p�P�b�g�`�F�b�N�Ɗi�[
uint8_t CWacoCanUsb::StockPacket(PRSCANMSG pDst,char* pSrc,uint8_t nSrcLen)
	{
	//�v�f�ԍ�[1-2]����t���O�ނ𒊏o���A�\��̃p�P�b�g�S�̃T�C�Y���쐬
	uint8_t nFlag = (uint8_t)hex2num(&pSrc[1],2);
	uint8_t nExtendID = (nFlag >> 4) & 1;
	uint8_t nDLC = nFlag & 0x0f;
	uint8_t nLength = 1 + 2 + 8 + (nDLC * 2) + (2 * 2) + 1;	//max:32

	//�S�̃T�C�Y�ƈ�v���Ȃ��H
	if(nSrcLen != nLength)
		return(1);	//�S�̃T�C�Y�ƈ�v���Ȃ�


	//�i�[:�g��ID
	pDst->nExtendID = nExtendID;

	//�i�[:CANID[3-10]
	pDst->nCANid = hex2num(&pSrc[3],8);

	//�i�[:�f�[�^��
	pDst->nDLC = nDLC;

	//�i�[:DATA[11-X]
	for(uint8_t nLoop = 0;nLoop < nDLC;nLoop++)
		pDst->nRaw8[nLoop] = (uint8_t)hex2num(&pSrc[11 + (nLoop * 2)],2);

	//�i�[:CRC[X+1 - X+5]
	pDst->nCRC = hex2num(&pSrc[11 + (nDLC * 2)],4);

	//
	return(0);
	}
