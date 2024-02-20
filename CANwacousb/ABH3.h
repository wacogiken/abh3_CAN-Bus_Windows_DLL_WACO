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

//���d�C���N���[�h�h�~
#ifndef _ABH3_H_INCLUDED_
#define _ABH3_H_INCLUDED_

//�v���b�g�t�H�[���ˑ�
#include <Windows.h>
#include <stdint.h>
#include <timeapi.h>
#include <stdio.h>

//CAN�֘A
#include "Can1939.h"
#include "CanIF.h"

//�^��`
#include "typedef.h"

//�N���X��`
class CAbh3
{
//================================================================================
//���[�U�[�����p�\�Ȍ^
//================================================================================
public:

//================================================================================
//�����Ŏg�p�����^
//================================================================================
protected:

//================================================================================
//���[�U�[�����p�\�Ȋ֐��iWindows�ŗL�j
//================================================================================
public:
	//�g�p�\��CAN�C���^�[�t�F�[�X�����擾
	int32_t GetInterfaceCount(void);
	//�J���C���^�[�t�F�[�X���w��
	int32_t SetInterface(int32_t nDeviceNum);
	//�C���^�[�t�F�[�X���J��
	int32_t OpenInterface(void);
	//�J�����C���^�[�t�F�[�X�����
	void CloseInterface(void);
	//���݊J���Ă���C���^�[�t�F�[�X�ԍ����擾
	int32_t GetCurrentInterface(void);
	//���݃C���^�[�t�F�[�X���J���Ă��邩�H
	int32_t IsOpenInterface(void);
	//�C���^�[�t�F�[�X���J���^�C���A�E�g���Ԃ�ݒ�
	void SetOpenTimeout(uint32_t nTimeoutMS);
	//���M�^�C���A�E�g���Ԃ�ݒ�
	void SetSendTimeout(uint32_t nTimeoutMS);
	//��M�^�C���A�E�g���Ԃ�ݒ�
	void SetRecvTimeout(uint32_t nTimeoutMS);
	//�ʐM�z�X�g�̃A�h���X��ݒ�
	void SetHostID(uint8_t nAdrs);
	//�ʐM�z�X�g�̃A�h���X���擾
	uint8_t GetHostID(void);
	//�ʐM���x���w��
	void SetBaudrate(uint32_t nBaudrateKbps);
	//�ݒ肵���ʐM���x���擾
	uint32_t GetBaudrate(void);
	//PC���N�����Ă���̎��Ԃ�[ms]�P�ʂŎ擾
	uint32_t GetTm(void)
		{
		uint32_t nResult = 0;
		if(m_pVar)
			{
			if(IsValidDevice())
				nResult = m_pVar->pDeviceClass->GetTm();
			}
		return(nResult);
		}	

	//1�b�Ԃɑ���M�����f�[�^�ʂ��擾���܂�
	uint32_t GetCounter(void)
		{
		uint32_t nResult = 0;
		if(m_pVar)
			{
			if(IsValidDevice())
				nResult = m_pVar->pDeviceClass->GetCounter();
			}
		return(nResult);
		}

	//����M�J�E���^���擾���܂�
	uint32_t GetTransmitCounter(uint8_t nRecv)
		{
		if(m_pVar)
			{
			if(IsValidDevice())
				return(m_pVar->pDeviceClass->GetTransmitCounter(nRecv));
			}
		return(0);
		}

	//CAN�o�X�ɃG���[���������Ă��邩���ׂ܂�
	uint32_t GetCANerror(void);
	//CAN�o�X�̃G���[�������s���܂�
	 uint32_t ResetCANerror(void);

	//�V���O���p�P�b�g�̑��MPDU�R�[�h��ݒ�
	void SetSinglePacketCode(uint8_t nTargetID,uint8_t nCode)
		{
		if(m_pVar)
			m_pVar->config.target[nTargetID].nPDU_single = nCode;
		}
	//�V���O���p�P�b�g�̑��MPDU�R�[�h���擾
	uint8_t GetSinglePacketCode(uint8_t nTargetID)
		{
		uint8_t nResult = 0;
		if(m_pVar)
			nResult = m_pVar->config.target[nTargetID].nPDU_single;
		if(nResult == 0)
			nResult = 0xEF;
		return(nResult);
		}
	//�u���[�h�L���X�g�p�P�b�g�̑��MPDU�R�[�h��ݒ�
	void SetBroadcastPacketCode(uint8_t nTargetID,uint8_t nCode)
		{
		if(m_pVar)
			m_pVar->config.target[nTargetID].nPDU_broadcast = nCode;
		}
	//�u���[�h�L���X�g�p�P�b�g�̑��MPDU�R�[�h���擾
	uint8_t GetBroadcastPacketCode(uint8_t nTargetID)
		{
		uint8_t nResult = 0;
		if(m_pVar)
			nResult = m_pVar->config.target[nTargetID].nPDU_broadcast;
		if(nResult == 0)
			nResult = 0xEA;
		return(nResult);
		}

//================================================================================
//���[�U�[�����p�\�Ȋ֐��iABH3�ŗL�j
//================================================================================
public:
	//�w�߂̏������i�������t���j
	int32_t abh3_can_init(uint8_t nTargetID,pCANABH3_RESULT pResult);
	int32_t abh3_can_port_init(void);
	int32_t abh3_can_cmd_init(uint8_t nTargetID,pCANABH3_RESULT pResult);
	//�w�߂̑��M�i���ʁj
	int32_t abh3_can_cmdAY(uint8_t nTargetID,int16_t cmd,pCANABH3_RESULT pPtr);
	int32_t abh3_can_cmdBX(uint8_t nTargetID,int16_t cmd,pCANABH3_RESULT pPtr);
	//�w�߂̑��M�i�����j
	int32_t abh3_can_cmd(uint8_t nTargetID,int16_t cmdAY,int16_t cmdBX,pCANABH3_RESULT pPtr);
	//���͂̑��M�i�ꊇ�j
	int32_t abh3_can_opSet(uint8_t nTargetID,int32_t data,int32_t mask,pCANABH3_RESULT pPtr);
	//���͂̑��M�i�r�b�g�j
	int32_t abh3_can_opBitSet(uint8_t nTargetID,int8_t num,int8_t data,pCANABH3_RESULT pPtr);
	//�w�߂Ɠ��͂̑��M�i�����j
	int32_t abh3_can_cmdAndopSet(uint8_t nTargetID,int16_t cmdAY,int16_t cmdBX,int32_t data,int32_t mask,pCANABH3_RESULT pPtr);
	//�u���[�h�L���X�g�p�P�b�g�̃��N�G�X�g
	int32_t abh3_can_reqBRD(uint8_t nTargetID,uint8_t num,uint8_t nBroadcast,pCANABH3_RESULT pPtr);
	//�}���`�p�P�b�g�ɂ��TelABH3�p�P�b�g�̑���M
	int32_t abh3_can_trans(uint8_t nTargetID,char* sbuf,char*& rbuf,size_t& rbuflen);
	//���݂̎�M�f�[�^�󋵂��ꊇ�擾
	void abh3_can_copylastdata(uint8_t nTargetID,pCANABH3_LASTDATA pPtr);
	//��M�f�[�^�󋵂̎w��ӏ��̍X�V�t���O������
	void abh3_can_resetlastdata(uint8_t nTargetID,int32_t nAdrs);
	//��M(�O���p�E�r�����䂠��)
	int32_t abh3_can_read(uint8_t nTargetID,pCANABH3_RESULT pPtr,PACKETTYPE nType,uint8_t nAdrs);
	//��M(�O���p�E�r�����䂠��)
	int32_t abh3_can_flush(void);
	//��������
	int32_t abh3_can_finish(void);

//================================================================================
//���[�U�[�p�֐�(�P�ʕϊ��n)
//================================================================================
public:
	//���x
	int16_t	cnvVel2CAN(float vel)
		{
		int16_t nResult = int16_t(vel / 0.2f);
		return(nResult);
		}
	float	cnvCAN2Vel(int16_t vel)
		{
		float nResult = float(vel) * 0.2f;
		return(nResult);
		}

	//���ח�
	float	cnvCAN2Load(int16_t load)
		{
		float nResult = float(load) * 1.0f;
		return(nResult);
		}

	//�A�i���O����
	float	cnvCAN2Analog(int16_t analog)
		{
		float nResult = float(analog) * 0.01f;
		return(nResult);
		}

	//�d���d��
	float	cnvCAN2Volt(int16_t volt)
		{
		float nResult = float(volt) * 0.1f;
		return(nResult);
		}

	//�d��
	float	cnvCAN2Cur(int16_t cur)
		{
		float nResult = float(cur) * 0.01f;
		return(nResult);
		}
	int16_t cnvCur2CAN(float cur)
		{
		int16_t nResult = int16_t(cur / 0.01f);
		return(nResult);
		}


//================================================================================
//�����p
//================================================================================
protected:


	//�ݒ�
	typedef struct _CANABH3_CONFIG
		{
		//���ʍ���
		uint8_t			nHostAdrs;		//�z�X�g���iPC�j�A�h���X
		uint8_t			nSingleCode;	//EFh	�V���O���p�P�b�g�̎��ʃR�[�h
		//�ڑ��斈�ɈقȂ�v�f
		struct _CONFIG_TARGET
			{
			uint8_t		nPDU_single;		//�V���O���p�P�b�g����PDU�i���w���0xEF�j
			uint8_t		nPDU_broadcast;		//�u���[�h�L���X�g�p�P�b�g��PDU�i���w���0xEA�j
			} target[256];
		} CANABH3_CONFIG,*pCANABH3_CONFIG;

	//�����p�ϐ�
	typedef struct _CANABH3_VAR
		{
		CCanIF*				pDeviceClass;	//�o�͐�̃N���X
		int32_t				nReqDevice;		//�J���\��̃f�o�C�X�ԍ�
		int32_t				nOpenDevice;	//0..�J���Ă��Ȃ�  1�ȏ�..�J�����C���^�[�t�F�[�X�ԍ�+1������
		HANDLE				hTerm;			//�ʐM�r���p�̃Z�}�t�H
		CANABH3_CONFIG		config;			//�ݒ�
		CANABH3_LASTDATA	lastdata[256];	//�ŏI�f�[�^�֘A�i���M�E��M�j
		} CANABH3_VAR,*pCANABH3_VAR;

	//�����ϐ��i���I�m�ۂ��Ďg���j
	pCANABH3_VAR m_pVar;

	//��MID���环�ʂɕK�v�ȗv�f�𒊏o
	PACKETTYPE recvid2any(uint32_t nCANID,uint8_t& nSenderID,uint8_t& nTargetID,uint8_t& nGroup,uint8_t& nAdrs,uint8_t* pRecvData);

	//�ŏI��M�f�[�^�̈���擾�j
	pCANABH3_LASTDATA GetLastData(uint8_t nID)
		{
		pCANABH3_LASTDATA pResult = NULL;
		if(m_pVar)
			pResult = &m_pVar->lastdata[nID];
		return(pResult);
		}

	//�ŏI���M�f�[�^�̈���擾�iCAN�ɑ��M�����f�[�^�̗̈������j
	pCANABH3_LASTSEND GetLastSendData(uint8_t nID)
		{
		pCANABH3_LASTDATA pResult = GetLastData(nID);
		if(pResult)
			return(&pResult->send);
		return(NULL);
		}

	//�ŏI��M�f�[�^�̈���擾�iCAN�����M�����f�[�^�Ɠ����j
	pCANABH3_LASTRECV GetLastRecvData(uint8_t nID)
		{
		pCANABH3_LASTDATA pResult = GetLastData(nID);
		if(pResult)
			return(&pResult->recv);
		return(NULL);
		}

	//�f�o�C�X�N���X���o�^����Ă��邩�H
	bool IsValidDevice(void)
		{
		if(m_pVar == NULL)
			return(false);
		//�o�^����Ă����true���߂�
		return((bool)(m_pVar->pDeviceClass != 0));
		}

	//CANID���\�z
	uint32_t CreateSinglePacketID(uint8_t nTargetID)
		{
		//����16bit�ɑ��M��Ƒ��M����ID������

		//�V���O���p�P�b�g�̎��ʃR�[�h
		uint8_t nCode = GetSinglePacketCode(nTargetID);

		//CAN-ID�\�z�i0x00XXYYZZ�j
		//	XX..�V���O���p�P�b�g�̎��ʃR�[�h
		//	YY..���M��ID
		//	ZZ..�z�X�gID
		uint32_t nResult = ((uint32_t)nCode) << 16 | ((uint32_t)nTargetID) << 8 | GetHostID();
		return(nResult);
		}

	//CANID���\�z
	uint32_t CreateBroadcastPacketID(uint8_t nTargetID,uint8_t num,uint8_t nAdrs,uint8_t nBroadcast = 0)
		{
		//�p�����[�^
		//	nTargetID		���M��ID
		//	num				�u���[�h�L���X�g�ԍ��i���5bit���O���[�v�ԍ��A����3bit���A�h���X�j
		//	nGroup			���M��O���[�v�ԍ�
		//	nAdrs			���M��A�h���X
		//	nBroadcast		0�ȊO�őS�̑��M�i���M�悪0xFF�Ƃ��Ĉ�����j

		//�S�̑��M�H
		if(nBroadcast)
			nTargetID = 0xff;

		//�u���[�h�L���X�g�p�P�b�g�̎��ʃR�[�h
		uint8_t nCode = GetBroadcastPacketCode(nTargetID);

		//CAN-ID�\�z�i0x00XXYYZZ�j
		//	XX..�u���[�h�L���X�g�p�P�b�g�̎��ʃR�[�h
		//	YY..���M��ID
		//	ZZ..�z�X�gID
		uint32_t nResult = ((uint32_t)nCode) << 16 | ((uint32_t)nTargetID) << 8 | GetHostID();
		return(nResult);
		}

	//�u���[�h�L���X�g�̃V�O�l�`���i�f�[�^�̐擪3�o�C�g�ɓ��镨�j���\�z
	uint32_t CreateBrdID(uint8_t nBrdNum)
		{
		//����8bit�Ƀu���[�h�L���X�g�̃O���[�v�ƃA�h���X����Z�o�����ԍ�������
		uint32_t nResult = 0x00ff00 | nBrdNum;
		return(nResult);
		}

	//�r������v��
	int32_t Lock(void)
		{
		//�����ϐ��̊m�F
		if(m_pVar == NULL)
			return(-1);	//�����ϐ����p�ӏo���ĂȂ�
		//�f�o�C�X�N���X�̊m�F
		else if(!IsValidDevice())
			return(-1);	//�f�o�C�X�N���X���o�^����Ă��Ȃ�
		//���M�^�C���A�E�g���Ԃ��擾
		uint32_t nTimeoutMS = m_pVar->pDeviceClass->GetTimeout_Send();

		//���b�N���鎞�͑��M���e���Ԃ𗘗p����
		if(::WaitForSingleObject(m_pVar->hTerm,nTimeoutMS) != WAIT_OBJECT_0)
			return(-1);	//������L���s
		//������L����
		return(0);
		}

	//�r������J��
	void Unlock(void)
		{
		if(m_pVar == NULL)
			return;
		//���ӓ_
		//	�m�F���Ȃ��ŊJ������ׁA2�d�J���̗ނɒ���
		::ReleaseSemaphore(m_pVar->hTerm,1,NULL);
		}

	//��M�o�b�t�@�N���A
	void ClearRecvBuffer(void)
		{
		if(m_pVar == NULL)
			return;
		else if(IsValidDevice())
			m_pVar->pDeviceClass->OnCanRecvClear();
		}

	//��M(�����p�E�r�����䖳��)
	int32_t abh3_can_recv(uint8_t nTargetID,pCANABH3_RESULT pPtr,PACKETTYPE nType,uint8_t nAdrs = 0);

	//CAN���M
	int32_t CanSend8(uint32_t nSendID,uint8_t* pSendData,uint8_t nLength);

	//CAN��M
	int32_t CanRecv8(uint32_t* pRecvID,uint8_t* pRecvData); 

	//�}���`�p�P�b�g���M
	int32_t CanTermSendMulti(uint8_t nTargetID,uint8_t* pSendData,uint32_t nSendDataSize,uint8_t*& pRecvData,uint32_t& nRecvDataSize);

	//��M�����f�[�^���ŏI�f�[�^�Ƃ��Ċi�[
	void StockLastRecvData(uint32_t nRecvID,uint8_t* pRecvData);

	//���M����p�P�b�g�f�[�^���ŏI�f�[�^�Ƃ��Ċi�[
	void StockLastSendData(uint8_t nSendID,uint8_t* pSendData,uint8_t nLength,bool bSinglePacket);


	//CANID���甭�M�������擾
	uint8_t canid2fromid(uint32_t nID)
		{
		//���M����bit[0-7]
		return((uint8_t)(nID));
		}

	//CANID���瑗�M����擾
	uint8_t canid2toid(uint32_t nID)
		{
		//���M���bit[8-15]
		return((uint8_t)(nID >> 8));
		}

public:
	//�R���X�g���N�^
	CAbh3(CCanIF* pDeviceClass = NULL);
	//�f�X�g���N�^
	virtual ~CAbh3();
};

#endif
