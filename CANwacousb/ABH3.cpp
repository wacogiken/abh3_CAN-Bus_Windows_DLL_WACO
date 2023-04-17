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

//�T�v
//	ABH3�pCAN����N���X
//
//����
//	2023/03/31	yo0043		1th release
//							�O��CAbh3�N���X����啝�ύX�ׁ̈A�ʕ��Ƃ��Ĉ���
//							��M�o�b�t�@���w�肵�Ȃ��ʐM�v���́A���M�̂ݍs��
//							�C���^�[�t�F�[�X�̃I�[�v���N���[�Y�ɂ��r�����������

#include "pch.h"
#include <memory.h>
#include "Abh3.h"

//���S�ȍ폜
template <class T> void SafeDelete(T** ppT)
	{
	if (*ppT)
		{
		delete *ppT;
		*ppT = NULL;
		}
	}

//�R���X�g���N�^
CAbh3::CAbh3(CCanIF* pDeviceClass /* NULL */)
	{
	//�����ϐ�����������ԂŊm��
	m_pVar = new CANABH3_VAR();

	//�ʐM����p�Z�}�t�H�\�z
	m_pVar->hTerm = ::CreateSemaphore(NULL,1,1,NULL);

	//�o�͒i�̃N���X��ۑ�
	m_pVar->pDeviceClass = pDeviceClass;
	}

//�f�X�g���N�^
CAbh3::~CAbh3()
	{
	//�C���^�[�t�F�[�X�����i�J���Ă���ꍇ���L��ׁj
	CloseInterface();


	//�ʐM����p�Z�}�t�H�̊J��
	::CloseHandle(m_pVar->hTerm);
	m_pVar->hTerm = NULL;

	//�����ϐ����
	SafeDelete(&m_pVar);
	}

//================================================================================
//���[�U�[�p�֐�(Windows�ŗL�n)
//================================================================================

//�g�p�\��CAN�C���^�[�t�F�[�X�����擾
int32_t CAbh3::GetInterfaceCount(void)
	{
	//�f�o�C�X�����o�^�H
	if(!IsValidDevice())
		return(0);	//�f�o�C�X��������
	return(m_pVar->pDeviceClass->OnGetInterfaceCount());
	}

//�C���^�[�t�F�[�X���w�肷��
int32_t CAbh3::SetInterface(int32_t nDeviceNum)
	{
	m_pVar->nReqDevice = nDeviceNum;
	return(0);
	}

//�w��C���^�[�t�F�[�X���J��
int32_t CAbh3::OpenInterface()
	{
	//�J���\��̃f�o�C�X�ԍ����擾
	int32_t nDeviceNum = m_pVar->nReqDevice;

	//CAN�C���^�[�t�F�[�X���J���Ă���ꍇ���L��̂ŕ���
	CloseInterface();

	//�f�o�C�X�����o�^�H
	if(!IsValidDevice())
		return(-1);	//�G���[

	//CAN�C���^�[�t�F�[�X���J��
	int32_t nResult = m_pVar->pDeviceClass->OnOpenInterface(nDeviceNum);

	//�ُ�I���H
	if(nResult)
		return(nResult);

	//�J����CAN�C���^�[�t�F�[�X����ۑ�
	m_pVar->nOpenDevice = nDeviceNum + 1;

	//�f�o�C�X�Ƀz�X�g��ID��n���āA��������ΏۂƂ�����i���M����M�ɏ��ׁj
	m_pVar->pDeviceClass->SetIgnoreID(GetHostID());

	//
	return(0);
	} 

//�J�����C���^�[�t�F�[�X�����
void CAbh3::CloseInterface()
	{
	//�J���Ă���CAN�C���^�[�t�F�[�X��������
	m_pVar->nOpenDevice = 0;

	//�f�o�C�X�o�^�L��H
	if(IsValidDevice())
		{
		//CAN�C���^�[�t�F�[�X�����
		m_pVar->pDeviceClass->OnCloseInterface();
		}
	}

//���݊J���Ă���C���^�[�t�F�[�X�ԍ����擾
int32_t CAbh3::GetCurrentInterface()
	{
	return(m_pVar->nOpenDevice - 1);
	}

//���݃C���^�[�t�F�[�X���J���Ă��邩�H
int32_t CAbh3::IsOpenInterface()
	{
	if(GetCurrentInterface() >= 0)
		return(1);
	return(0);
	}

//�C���^�[�t�F�[�X���J���^�C���A�E�g���Ԃ�ݒ�
void CAbh3::SetOpenTimeout(uint32_t nTimeoutMS)
	{
	if(IsValidDevice())
		m_pVar->pDeviceClass->SetTimeout_Open(nTimeoutMS);
	}

//���M�^�C���A�E�g���Ԃ�ݒ�
void CAbh3::SetSendTimeout(uint32_t nTimeoutMS)
	{
	if(IsValidDevice())
		m_pVar->pDeviceClass->SetTimeout_Send(nTimeoutMS);
	}

//��M�^�C���A�E�g���Ԃ�ݒ�
void CAbh3::SetRecvTimeout(uint32_t nTimeoutMS)
	{
	if(IsValidDevice())
		m_pVar->pDeviceClass->SetTimeout_Recv(nTimeoutMS);
	}

//�ʐM�z�X�g�̃A�h���X��ݒ�
void CAbh3::SetHostID(uint8_t nAdrs)
	{
	m_pVar->config.nHostAdrs = nAdrs;
	}

//�ʐM�z�X�g�̃A�h���X���擾
uint8_t CAbh3::GetHostID()
	{
	return(m_pVar->config.nHostAdrs);
	}

//�ʐM���x���w��
void CAbh3::SetBaudrate(uint32_t nBaudrateKbps)
	{
	//�T�v
	//	�ʐM�{�[���[�g�w��
	//�p�����[�^
	//	nBaudrateKbps	�{�[���[�g��[kbps]�P�ʂŎw��i�f�o�C�X�ˑ��Ȃ̂Œ��Ӂj

	//�f�o�C�X���o�^�ς݂Ȃ�{�[���[�g�ݒ�
	if(IsValidDevice())
		m_pVar->pDeviceClass->SetBaudrate(nBaudrateKbps);
	}

//�ݒ肵���ʐM���x���擾
uint32_t CAbh3::GetBaudrate(void)
	{
	//�f�o�C�X�����o�^�H
	if(!IsValidDevice())
		return(0);
	return(m_pVar->pDeviceClass->GetBaudrate());
	}

//CAN�o�X�ɃG���[���������Ă��邩���ׂ܂�
uint32_t CAbh3::GetCANerror()
	{
	//�T�v
	//	CAN�o�X�ɃG���[���������Ă��邩���ׂ܂�
	//�p�����[�^
	//	����
	//�߂�l
	//	0			�G���[����
	//	2			�C���^�[�t�F�[�X�̃A�N�Z�X���̂ɖ�肪�L��
	//	��L�ȊO	�G���[�L��

	if(!IsValidDevice())
		return(2);
	return((uint32_t)m_pVar->pDeviceClass->OnCanGetError());
	}

//CAN�o�X�̃G���[�������s���܂�
 uint32_t CAbh3::ResetCANerror()
	{
	//�T�v
	//	CAN�o�X�̃G���[�������s���܂�
	//�p�����[�^
	//	����
	//�߂�l
	//	0			����I��
	//	2			�C���^�[�t�F�[�X�̃A�N�Z�X���̂ɖ�肪�L��
	//	��L�ȊO	�G���[�����Ɏ��s
	//���ӓ_
	//	�{�֐��ł̓G���[���畜�A�o���Ȃ��ꍇ���L�鎖�ɒ���
	//	���̏ꍇ�́A�C���^�[�t�F�[�X����x�Ԃ��A�ēx�J������
	//	�菇�ő��̏ꍇ�͕��A����

	if(!IsValidDevice())
		return(2);
	return((uint32_t)m_pVar->pDeviceClass->OnCanClearError());
	}

//================================================================================
//���[�U�[�p�֐�(�ʐM�n)
//================================================================================

//
int32_t CAbh3::abh3_can_init(uint8_t nTargetID,pCANABH3_RESULT pPtr)
	{
	int32_t nResult = this->abh3_can_port_init();
	if(nResult == 0)
		nResult = this->abh3_can_cmd_init(nTargetID,pPtr);
	return(nResult);
	}

//
int32_t CAbh3::abh3_can_port_init()
	{
	int32_t nResult = 0;

	//�ʐM�r������p�̃Z�}�t�H���擾
	if(Lock() == 0)
		{
		nResult = OpenInterface();
		
		Unlock();
		}
	else
		nResult = -1;

	return(nResult);
	}

//�w�߂̏�����
int32_t CAbh3::abh3_can_cmd_init(uint8_t nTargetID,pCANABH3_RESULT pPtr)
	{
	//���s
	return(
		abh3_can_cmdAndopSet(
			nTargetID,
			0,
			0,
			0,
			0,
			pPtr
			)
		);
	}

//�w�߂̑��M�i���ʁj
int32_t CAbh3::abh3_can_cmdAY(uint8_t nTargetID,int16_t cmd,pCANABH3_RESULT pPtr)
	{
	//�ڑ���ʂ̍ŏI���M�f�[�^�̈���擾
	pCANABH3_LASTSEND pLastsend = GetLastSendData(nTargetID);
	//���s
	return(
		abh3_can_cmdAndopSet(
			nTargetID,
			cmd,
			pLastsend->nOrderBX,
			pLastsend->nInputBit,
			0,
			pPtr
			)
		);
	}
int32_t CAbh3::abh3_can_cmdBX(uint8_t nTargetID,int16_t cmd,pCANABH3_RESULT pPtr)
	{
	//�ڑ���ʂ̍ŏI���M�f�[�^�̈���擾
	pCANABH3_LASTSEND pLastsend = GetLastSendData(nTargetID);
	//���s
	return(
		abh3_can_cmdAndopSet(
			nTargetID,
			pLastsend->nOrderAY,
			cmd,
			pLastsend->nInputBit,
			0,
			pPtr
			)
		);
	}

//�w�߂̑��M�i�����j
int32_t CAbh3::abh3_can_cmd(uint8_t nTargetID,int16_t cmdAY,int16_t cmdBX,pCANABH3_RESULT pPtr)
	{
	//�ڑ���ʂ̍ŏI���M�f�[�^�̈���擾
	pCANABH3_LASTSEND pLastsend = GetLastSendData(nTargetID);
	//���s
	return(
		abh3_can_cmdAndopSet(
			nTargetID,
			cmdAY,
			cmdBX,
			pLastsend->nInputBit,
			0,
			pPtr
			)
		);
	}

//���͂̑��M�i�ꊇ�j
int32_t CAbh3::abh3_can_opSet(uint8_t nTargetID,int32_t data,int32_t mask,pCANABH3_RESULT pPtr)
	{
	//�ڑ���ʂ̍ŏI���M�f�[�^�̈���擾
	pCANABH3_LASTSEND pLastsend = GetLastSendData(nTargetID);

	return(
		abh3_can_cmdAndopSet(
			nTargetID,
			pLastsend->nOrderAY,
			pLastsend->nOrderBX,
			data,
			mask,
			pPtr
			)
		);
	}

//���͂̑��M�i�r�b�g�j
int32_t CAbh3::abh3_can_opBitSet(uint8_t nTargetID,int8_t num,int8_t data,pCANABH3_RESULT pPtr)
	{
	//�ڑ���ʂ̍ŏI���M�f�[�^�̈���擾
	pCANABH3_LASTSEND pLastsend = GetLastSendData(nTargetID);

	//
	int32_t nData = pLastsend->nInputBit;
	if(data)
		nData |= (1 << num);
	else
		nData &= ~(1 << num);

	//
	return(
		abh3_can_cmdAndopSet(
			nTargetID,
			pLastsend->nOrderAY,
			pLastsend->nOrderBX,
			nData,
			0,
			pPtr
			)
		);
	}

//�w�߂Ɠ��͂̑��M�i�����j
int32_t CAbh3::abh3_can_cmdAndopSet(uint8_t nTargetID,int16_t cmdAY,int16_t cmdBX,int32_t data,int32_t mask,pCANABH3_RESULT pPtr)
	{
	//�߂�l
	int32_t nResult = -1;

	//�ʐM�r������p�̃Z�}�t�H���擾
	if(Lock() == 0)
		{
		//�C���^�[�t�F�[�X���J���Ă���H
		if(IsOpenInterface())
			{
			//�ڑ���ʂ̍ŏI���M�f�[�^�̈���擾
			pCANABH3_LASTSEND pLastsend = GetLastSendData(nTargetID);

			//�ݒ�l���i�[
			pLastsend->nOrderAY = cmdAY;
			pLastsend->nOrderBX = cmdBX;
			pLastsend->nInputBit = (data & ~mask) | (data & mask);

			//�V���O���p�P�b�g(DP0)���\�z
			uint8_t* pPacket = CCan1939::CreateSGL0(
				 pLastsend->nOrderAY
				,pLastsend->nOrderBX
				,pLastsend->nInputBit
				);

			//���M��ID���쐬
			uint32_t nSendID = CreateSinglePacketID(nTargetID);

			//�Œ蒷�ő��M
			nResult = CanSend8(nSendID,pPacket,8);

			//��M�o�b�t�@�w��L��ł������G���[�����Ȃ�V���O���p�P�b�gDP0����M
			if(pPtr && (nResult == 0))
				nResult = abh3_can_recv(nTargetID,pPtr,PACKETTYPE::SINGLE_PACKET);

			//�J��
			CCan1939::FreeBuffer(pPacket);
			}
		//�ʐM�r������p�̃Z�}�t�H���J��
		Unlock();
		}

	//����
	return(nResult);
	}


//�u���[�h�L���X�g�p�P�b�g���M
int32_t CAbh3::abh3_can_reqBRD(uint8_t nTargetID,uint8_t num,uint8_t nBroadcast,pCANABH3_RESULT pPtr)
	{
	//�T�v
	//	�u���[�h�L���X�g�p�P�b�g�̑��M
	//�p�����[�^
	//	nTargetID	���M��ID
	//	num			�O���[�v�ԍ��ƃA�h���X����v�Z�����l�i��F�O���[�v5�ŃA�h���X0�Ȃ�0x28�j
	//	nBoradcast	0..���݂̐ڑ��悪���M��  0�ȊO..�SABH3�����M��
	//	pPtr		��M�p�̈�ANULL�w��Ŏ�M���Ȃ�
	//�߂�l
	//	0			����I��
	//	-1			CAN������g�p��
	//	��L�ȊO	���̑��̃G���[
	//

	//�߂�l
	int32_t nResult = -1;

	//�ʐM�r������p�̃Z�}�t�H���擾
	if(Lock() == 0)
		{
		//�C���^�[�t�F�[�X���J���Ă���H
		if(IsOpenInterface())
			{
			//�f�[�^���擪3�o�C�g�̃V�O�l�`�����\�z
			uint32_t nSign = CreateBrdID(num);

			//�u���[�h�L���X�g�p�P�b�g(num)���\�z
			uint8_t* pPacket = CCan1939::CreateBRD(nSign);

			//���M��CANID���쐬
			uint32_t nSendID = CreateBroadcastPacketID(nTargetID,num,nBroadcast);

			//�Œ蒷�ő��M
			nResult = CanSend8(nSendID,pPacket,3);

			//��M�o�b�t�@�w��L��ł������G���[�����Ȃ�V���O���p�P�b�gDP0����M
			if(pPtr && (nResult == 0))
				{
				int nAdrs = num & 0x07;	//����3�r�b�g���A�h���X
				nResult = abh3_can_recv(nTargetID,pPtr,PACKETTYPE::BROADCAST_PACKET,nAdrs);
				}

			//�J��
			CCan1939::FreeBuffer(pPacket);
			}
		//�ʐM�r������p�̃Z�}�t�H���J��
		Unlock();
		}

	//����
	return(nResult);
	}

//�}���`�p�P�b�g�ɂ��TelABH3�p�P�b�g�̑���M
int32_t CAbh3::abh3_can_trans(uint8_t nTargetID,char* sbuf,char*& rbuf,size_t& rbuflen)
	{
	//�T�v
	//	sbuf		���M�f�[�^���i�[���ꂽ�|�C���^
	//	rbuf		��M�f�[�^���i�[����|�C���^�ϐ��ւ̎Q��
	//	rbuflen		��M�f�[�^�o�b�t�@��
	//�߂�l
	//	0			����
	//	0�ȊO		�ُ�
	//���ӓ_
	//	rbuf�͌Ăяo�����ŊJ��(delete)����K�v������܂�
	//	�ʐM���������͎��s����܂ŏ������߂�܂���
	//�g�p��
	//	char* sbuf = "CP000095";
	//	char* rbuf = NULL;
	//	size_t rbuflen = 0;
	//	if(abh3_can_trans(sbuf,rbuf,rbuflen) == 0)
	//		{
	//		//����ȏꍇ�̏���
	//		printf("%s\n",rbuf);
	//		}
	//	else
	//		{
	//		//�ُ�ȏꍇ�̏���
	//		printf("Error\n");
	//		}
	//	if(rbuf)
	//		delete rbuf;

	//�߂�l
	int32_t nResult = -1;

	//�ʐM�r������p�̃Z�}�t�H���擾
	if(Lock() == 0)
		{
		//�C���^�[�t�F�[�X���J���Ă���H
		if(IsOpenInterface())
			{
			//�ꎞ�o�b�t�@�\�z
			//2020.10.09 yo0043 �ȉ��d�l�ɕύX
			//	�f�[�^�������Œ�9�o�C�g�ɂȂ�l��1�ȏ��SYN��ǉ�

			//������̃T�C�Y�����ɁA�o�b�t�@�T�C�Y���Z�o
			size_t nSrcLen = ::strlen(sbuf);
			size_t nBufLen = 0;
			if(nSrcLen < 9)
				nBufLen = 9;
			else
				nBufLen = nSrcLen + 1;

			//�I�[�����l�����ăo�b�t�@���\�z
			char* pTmp = new char[nBufLen + 1]();	//�������t���Ŋm�ہi�I�[�ׁ̈j

			//�I�[��������SYN(0x16)�Ŗ��߂�
			::memset(pTmp,0x16,nBufLen);

			//���������擪���當�����������R�s�[����
			::memcpy(pTmp,sbuf,nSrcLen);

			//�^�ϊ�
			uint8_t* pSrc = (uint8_t*)pTmp;
			uint8_t* pDst = 0;
			uint32_t nDstSize = 0;

			//����M
			nResult = CanTermSendMulti(nTargetID,pSrc,uint32_t(nBufLen),pDst,nDstSize);
			if(nResult == 0)
				{
				rbuf = (char*)pDst;
				rbuflen = (size_t)nDstSize;
				}
			//�ꎞ�o�b�t�@�J��
			delete [] pTmp;
			}
		//�ʐM�r������p�̃Z�}�t�H���J��
		Unlock();
		}

	//����
	return(nResult);
	}

//���݂̎�M�f�[�^�󋵂��ꊇ�擾
void CAbh3::abh3_can_copylastdata(uint8_t nTargetID,pCANABH3_LASTRECV pDst)
	{
	pCANABH3_LASTRECV pSrc = GetLastRecvData(nTargetID);
	if(pSrc)
		::CopyMemory(pDst,pSrc,sizeof(CANABH3_LASTRECV));
	}

//��M�f�[�^�󋵂̎w��ӏ��̍X�V�t���O������
void CAbh3::abh3_can_resetlastdata(uint8_t nTargetID,int32_t nAdrs)
	{
	if(nAdrs < 0)
		{
		for(nAdrs = 0;nAdrs < 8;nAdrs++)
			abh3_can_resetlastdata(nTargetID,nAdrs);
		}
	else
		m_pVar->lastdata.recv[nTargetID].update[nAdrs & 0x7].nUpdate = 0;
	}

//��M(�O���p�E�r�����䂠��)
int32_t CAbh3::abh3_can_read(uint8_t nTargetID,pCANABH3_RESULT pPtr,PACKETTYPE nType,uint8_t nAdrs)
	{
	//�߂�l
	int32_t nResult = -1;

	//�ʐM�r������p�̃Z�}�t�H���擾
	if(Lock() == 0)
		{
		nResult =  abh3_can_recv(nTargetID,pPtr,nType,nAdrs);
		Unlock();
		}
	//
	return(nResult);
	}

//��M(�O���p�E�r�����䂠��)
int32_t CAbh3::abh3_can_flush()
	{
	//�T�v
	//	��M�o�b�t�@�ɂ�����e��S�Ď擾���A�ŏI��M�f�[�^�ɔ��f������
	//�p�����[�^
	//	����
	//�߂�l
	//	��M�o�b�t�@����擾���ď���������
	//

	//�߂�l
	int32_t nCount = 0;

	//�_�~�[���[�h�p�o�b�t�@
	uint32_t nCanID;
	uint8_t data8[8];

	//�f�o�C�X�N���X���o�^�ς݂Ȃ��M
	if(m_pVar->pDeviceClass)
		{
		//���ʑw�̃o�b�t�@����ǂݏo���i�o�b�t�@����Ŏ��s���閘�J��Ԃ��j
		int32_t nResult = 0;
		while(nResult == 0)
			{
			//1�ǂݏo��
			nResult = m_pVar->pDeviceClass->OnCanRecv(&nCanID,data8,true);
			if(nResult == 0)
				{
				//�ŏI��M�f�[�^�֔��f������
				StockLastRecvData(nCanID,data8);
				++nCount;
				}
			}
		}

	//����
	return(nCount);
	}

//��������
int32_t CAbh3::abh3_can_finish()
	{
	//�ʐM�r������p�̃Z�}�t�H���擾
	if(Lock() == 0)
		{
		CloseInterface();
		Unlock();
		}

	return(0);
	}

//================================================================================
//�����p
//================================================================================

//��M�̂݁i�����p�j
int32_t CAbh3::abh3_can_recv(uint8_t nTargetID,pCANABH3_RESULT pPtr,PACKETTYPE nType,uint8_t nAdrs /* 0 */)
	{
	//�T�v
	//	CAN�C���^�[�t�F�[�X����f�[�^����M
	//�p�����[�^
	//	pPtr			��M�f�[�^�i�[��ւ̃|�C���^
	//	nType			��M���I������p�P�b�g���
	//					PACKETTYPE::SINGLE_PACLET ���� PACKETTYPE::BROADCAST_PACKET ���w�肷��
	//	nAdrs			�@��A�h���X���w��i�u���[�h�L���X�g�p�P�b�g�w�莞�̂ݗ��p�����j
	//
	//�߂�l
	//	0			����I��
	//	0�ȊO		�ُ�I��
	//
	//�I���������
	//	nType == PACKETTYPE::ANY_PACKET
	//		������M������߂�B�inTargetID,nAdrs�͖��������j
	//	nType == PACKETTYPE::SINGLE_PACKET
	//		nTargetID���甭�M���ꂽ�V���O���p�P�b�g����M������߂�B�inAdrs�͖��������j
	//	nType == PACKETTYPE::BROADCAST_PACKET
	//		nTargetID���甭�M���ꂽ�u���[�h�L���X�g�p�P�b�g�ŁAnAdrs�̃A�h���X����v������߂�B
	//		�O���[�v�ԍ��͖��������
	//
	//�{�֐����O��Ƃ��Ă������
	//	todo
	//
	//���ӓ_
	//	���̊֐��͑��̒ʐM�֐�����Ăяo�����ׁA���b�N�֎~
	//	�w�肵����ނ̃p�P�b�g�����Ȃ���ԂŁA�^�C���A�E�g�ɂȂ�Ȃ���������
	//	�p�P�b�g����M�����ԂɂȂ�Ə������甲�����Ȃ��ꍇ���L�鎖�ɒ��ӁB
	//

	//��M
	int32_t nResult = 0;
	while(nResult == 0)
		{
		//��M
		nResult = CanRecv8(&pPtr->nID,pPtr->u.raw);

		//�������H
		if(nType == PACKETTYPE::ANY_PACKET)
			break;

		//�����M�H
		else if(nResult == 0)
			{
			//�p�P�b�g����
			uint8_t nPacketSenderID = 0;
			uint8_t nPacketTargetID = 0;
			uint8_t nPacketGroup = 0;
			uint8_t nPacketAdrs = 0;
			PACKETTYPE nPacketType = recvid2any(pPtr->nID,nPacketSenderID,nPacketTargetID,nPacketGroup,nPacketAdrs,NULL);

			//����
			if(nTargetID == nPacketTargetID)
				{
				//�V���O���p�P�b�g�w��H
				if((nType == PACKETTYPE::SINGLE_PACKET) && (nPacketType == PACKETTYPE::SINGLE_PACKET))
					break;
				else if(nType == PACKETTYPE::BROADCAST_PACKET)
					{
					if((nPacketType == PACKETTYPE::BROADCAST_PACKET) && (nAdrs == nPacketAdrs))
						break;
					}
				}
			else
				{
				//�w�肵������ł͂Ȃ������甭�M���ꂽ�p�P�b�g����M
				}
			}
		else
			{
			//��M���̂����s
			}
		}
	//
	return(nResult);
	}

//8[bytes]����M
int32_t CAbh3::CanSend8(uint32_t nSendID,uint8_t* pSendData,uint8_t nLength)
	{
	//�T�v
	//	CAN�C���^�[�t�F�[�X�ɑ΂���ID���w�肵�ăf�[�^�𑗐M
	//�p�����[�^
	//	nSendID		CAN-ID(29bit)
	//	pSendData	���M�f�[�^�ւ̃|�C���^
	//�߂�l
	//	0			����I��
	//	0�ȊO		�ُ�I��
	//���ӓ_
	//	�r������͂��̊֐��̌Ăяo�������s����

	//�߂�l
	int32_t nResult = -1;

	//�f�o�C�X�N���X���o�^�ς݂Ȃ瑗�M
	if(m_pVar->pDeviceClass)
		{
		nResult = m_pVar->pDeviceClass->OnCanSend(nSendID,pSendData,nLength);
		}

	//����
	return(nResult);
	}

//��M
int32_t CAbh3::CanRecv8(uint32_t* pRecvID,uint8_t* pRecvData)
	{
	//�T�v
	//	CAN�C���^�[�t�F�[�X����f�[�^����M
	//�p�����[�^
	//	pRecvID		��M����CAN-ID(29bit)�i�[��ւ̃|�C���^
	//	pRecvData	��M�f�[�^�i�[��ւ̃|�C���^
	//�߂�l
	//	0			����I��
	//	0�ȊO		�ُ�I��
	//���ӓ_
	//	�r������͂��̊֐��̌Ăяo�������s����

	//�߂�l
	int32_t nResult = -1;

	//�f�o�C�X�N���X���o�^�ς݂Ȃ��M
	if(m_pVar->pDeviceClass)
		{
		//���ʑw�̃o�b�t�@����1�����Ă���
		nResult = m_pVar->pDeviceClass->OnCanRecv(pRecvID,pRecvData);

		//��M�����H
		if(nResult == 0)
			{
			//�ŏI�f�[�^�i�[�����ɓn���i�i�[����邩�͒��g����j
			StockLastRecvData(*pRecvID,pRecvData);
			}
		}

	//����
	return(nResult);
	}


//��MID���环�ʂɕK�v�ȗv�f�𒊏o
PACKETTYPE CAbh3::recvid2any(uint32_t nCANID,uint8_t& nSenderID,uint8_t& nTargetID,uint8_t& nGroup,uint8_t& nAdrs,uint8_t* pRecvData)
	{
	//�p�����[�^
	//	nCANID		��M�����p�P�b�g��CANID�i���M�p�P�b�g�͎̂g���Ȃ��̂Œ��Ӂj
	//	nSenderID	�p�P�b�g�𑗐M�����@���ID�̊i�[��
	//	nTargetID	��M��ɐݒ肳��Ă���ID�̊i�[��
	//	nGroup		�u���[�h�L���X�g�p�P�b�g�̏ꍇ�A�O���[�v�ԍ��̊i�[��
	//	nAdrs		�u���[�h�L���X�g�p�P�b�g�̏ꍇ�A�@��A�h���X�̊i�[��
	//�߂�l
	//	PACKETTYPE::SINGLE_PACKET		�V���O���p�P�b�g
	//	PACKETTYPE::BROADCAST_PACKET	�u���[�h�L���X�g�p�P�b�g
	//	PACKETTYPE::UNKNOWN_PACKET		����s��
	//

	//�߂�l
	PACKETTYPE nResult = PACKETTYPE::UNKNOWN_PACKET;

	uint8_t nD2 = uint8_t(nCANID >> 16);	//xxXXxxxx
	uint8_t nD1 = uint8_t(nCANID >> 8);		//xxxxXXxx
	uint8_t nD0 = uint8_t(nCANID);			//xxxxxxXX


	//(�m��)���M��
	nSenderID = nD0;

	//�u���[�h�L���X�g�p�P�b�g�̕ԓ��H(xxFFxxxx)
	if(nD2 == 0xff)
		{
		//(�m��)�u���[�h�L���X�g�p�P�b�g
		nResult = PACKETTYPE::BROADCAST_PACKET;
		//(�m��)�O���[�v�ԍ��ƃA�h���X
		nAdrs = nD1 & 0x07;				//(bit:xxxxxXXX)
		nGroup = (nD1 >> 3) & 0x1f;		//(bit:XXXXXxxx)
		//��M��͊܂܂�Ȃ��ׁA�z�X�gID��ݒ肷��
		nTargetID = GetHostID();
		}
	//�V���O���p�P�b�g�̕ԓ����͗v���H(xxEFxxxx)
	else if(nD2 == GetSinglePacketCode(nSenderID))
		{
		//(�m��)�V���O���p�P�b�g
		nResult = PACKETTYPE::SINGLE_PACKET;
		//(�m��)��M��
		nTargetID = nD1;
		//�O���[�v�ƃA�h���X�͊܂܂�Ȃ��ׁA0��ݒ肷��
		nGroup = 0;
		nAdrs = 0;
		}
	//��M�f�[�^�w��L��H�i�u���[�h�L���X�g�v���𔻒f����ꍇ�͎w��K�{�j
	else if(pRecvData)
		{
		//�u���[�h�L���X�g�p�P�b�g�̗v���H(xxEAxxxx)
		if(nD2 == GetBroadcastPacketCode(nSenderID))
			{
			//(�m��)�u���[�h�L���X�g�p�P�b�g
			nResult = PACKETTYPE::BROADCAST_PACKET;
			//(�m��)�O���[�v�ԍ��ƃA�h���X
			nAdrs = pRecvData[0] & 0x07;	//����3bit
			nGroup = (pRecvData[0] >> 3) & 0x1f;		//���5bit
			//(�m��)��M��
			nTargetID = nD1;
			}
		}

	//
	return(nResult);
	}


//��M�����f�[�^���ŏI�f�[�^�Ƃ��Ċi�[
void CAbh3::StockLastRecvData(uint32_t nRecvID,uint8_t* pRecvData)
	{
	//���ӓ_
	//	���ʂő��M�����������Ŋm�肷��
	//	�u���[�h�L���X�g�p�P�b�g�̏ꍇ�A�O���[�v�ԍ��͊i�[�Ɏg��Ȃ�
	//	�i���M��ID�ƃA�h���X�Ŋi�[�悪���肷��ׁj
	//

	//�p�P�b�g����
	uint8_t nSenderID = 0;
	uint8_t nTargetID = 0;
	uint8_t nGroup = 0;
	uint8_t nAdrs = 0;
	PACKETTYPE nType = recvid2any(nRecvID,nSenderID,nTargetID,nGroup,nAdrs,pRecvData);

	//�i�[��
	pCANABH3_LASTRECV pLastdata = GetLastRecvData(nSenderID);

	//�V���O���p�P�b�g�H
	if(nType == PACKETTYPE::SINGLE_PACKET)
		{
		//�i�[��ɃR�s�[
		::CopyMemory(&pLastdata->DP0R,pRecvData,8);
		//�X�V�t���O
		pLastdata->update[0].nUpdate = 1;
		}
	//�u���[�h�L���X�g�p�P�b�g�H
	else if(nType == PACKETTYPE::BROADCAST_PACKET)
		{
		//�e�A�h���X�ɕۑ�
		if(nAdrs == 0)
			::CopyMemory(&pLastdata->BR0,pRecvData,8);
		else if(nAdrs == 1)
			::CopyMemory(&pLastdata->BR1,pRecvData,8);
		else if(nAdrs == 2)
			::CopyMemory(&pLastdata->BR2,pRecvData,8);
		else if(nAdrs == 3)
			::CopyMemory(&pLastdata->BR3,pRecvData,8);
		else if(nAdrs == 4)
			::CopyMemory(&pLastdata->BR4,pRecvData,8);
		else if(nAdrs == 5)
			::CopyMemory(&pLastdata->BR5,pRecvData,8);
		else if(nAdrs == 6)
			::CopyMemory(&pLastdata->BR6,pRecvData,8);
		//�X�V�t���O
		pLastdata->update[nAdrs + 1].nUpdate = 1;
		}
	else
		{
		//�s���ȃp�P�b�g����M
		}
	}

//�}���`�p�P�b�g���M
int32_t CAbh3::CanTermSendMulti(uint8_t nTargetID,uint8_t* pSendData,uint32_t nSendDataSize,uint8_t*& pRecvData,uint32_t& nRecvDataSize)
	{
	//�T�v
	//	�}���`�p�P�b�g���M�iPC�����N�_�Ƃ��鑗��M�j���s��
	//�p�����[�^
	//	pSendData		���M�f�[�^�ւ̃|�C���^
	//	nSendDataSize	���M�f�[�^��[bytes]
	//	pRecvData		��M�f�[�^�������|�C���^�ւ̎Q��
	//	nRecvDataSize	��M�f�[�^�����i�[����ϐ��̎Q��
	//�߂�l
	//	0				����I��
	//	-11				�ʐM�悩�璆�~�v��(ABORT)����M
	//	-12				�ʐM�悩��Z�b�V�����d���v���i�������RTS�j����M
	//	-13				PC���ő��M���s
	//	-14				PC���Ŏ�M���s�i�^�C���A�E�g�j
	//
	//���ӓ_
	//	�֐�������ɏI�������ꍇ�ApRecvData�ɂ͎�M�f�[�^���i�[���ꂽ�̈�ւ̃|�C���^������܂����A
	//	���̃|�C���^�͌Ăяo�����ŊJ������K�v���L��܂�
	//	pSendData������SYN(0x16)��ǉ����Ă���Ăяo����
	//	(abh3_can_trans�֐����Œǉ����Ă���j

	//�߂�l
	int32_t nExitCode = 0;

	//
	uint32_t nStage = 0;	//��ԑJ�ڏ����p�X�e�[�W�ԍ�
	int32_t nResult = 0;	//�ėp�̖߂�l
	uint8_t nAbort = 0;		//Abort�ԓ����̗��R

	//�z�X�g�����猩�����M�pID�Ǝ�M����pID
	uint32_t nSendID		= 0x00ec0000 | (nTargetID << 8) | m_pVar->config.nHostAdrs;
	uint32_t nSendDTID		= 0x00eb0000 | (nTargetID << 8) | m_pVar->config.nHostAdrs;
	uint32_t nRecvID		= 0x00ec0000 | (m_pVar->config.nHostAdrs << 8) | nTargetID;
	uint32_t nRecvDTID		= 0x00eb0000 | (m_pVar->config.nHostAdrs << 8) | nTargetID;
	uint32_t nID			= 0;	//
	uint8_t nMaxPacket		= 0;	//���M���p�P�b�g��
	uint8_t nPacketNum		= 0;	//�p�P�b�g�ԍ��i1-255)
	uint8_t nTotalPacket	= 0;	//��M���̑��p�P�b�g��
	//

	//��M�o�b�t�@�N���A
	ClearRecvBuffer();

	while(-1)
		{
		//RTS�𑗂�X�e�[�W
		//	PC�����M�m�[�h�EABH3����M�m�[�h�ő��M�v���𑗂�
		if(nStage == 0)
			{
			//���M���p�P�b�g�����������i�z�X�g�̍ő�l�j
			nMaxPacket = 0xff;

			//CM_RTS���M
			uint8_t* pPacket = CCan1939::CreateCMRTS(nSendDataSize,nMaxPacket);
			nResult = CanSend8(nSendID,pPacket,8);

			//�p�P�b�g�̊J��
			CCan1939::FreeBuffer(pPacket);

			//���M����H
			if(nResult == 0)
				nStage = 1;
			else
				{
				//���M���s
				nExitCode = -13;
				nStage = 99;	//RTS���M���Ɏ��s�A�������M���ĂȂ��̂ł��̂܂܏I��
				}
			}

		//CTS/EOMA/ABORT���󂯎��X�e�[�W
		//	PC�����M�m�[�h�EABH3����M�m�[�h�ő��M�v��(RTS)���̓f�[�^���M(DT)�̕ԓ����߂�
		else if(nStage == 1)
			{
			//��M
			uint8_t* pPacket = CCan1939::CreateBuffer();
			nResult = CanRecv8(&nID,pPacket);

			if(nResult == 0)
				{
				//��M�͐���

				//���̃p�P�b�g�H
				uint8_t nType = CCan1939::IsPacket(pPacket);

				//���M�v���H
				if(nType == CCan1939::DEF_CTS)
					{
					//�^�[�Q�b�g������w�肳�ꂽ���M���p�P�b�g����ۑ�
					nMaxPacket = pPacket[1];

					//���ɑ��M����p�P�b�g�ԍ���ۑ�
					nPacketNum = pPacket[2];

					//DT�𑗂�X�e�[�W�ɑJ��
					nStage = 2;
					}
				//�����ʒm�H
				else if(nType == CCan1939::DEF_EOMA)
					{
					//��M��������
					nStage = 3;
					}
				//���~�v���H
				else if(nType == CCan1939::DEF_ABORT)
					{
					//���~����
					nExitCode = -11;
					nStage = 99;
					}
				//�V�K�v���H
				else if(nType == CCan1939::DEF_RTS)
					{
					//�Z�b�V�����d�������ŃG���[
					nExitCode = -12;
					nAbort = 1;
					nStage = 10;
					}
				//��L�ȊO�H
				else
					{
					//��L�ȊO�͌��Ȃ��������ɂ��čđ���҂�
					Sleep(0);
					}
				}
			else
				{
				//��M�����Ɏ��s�i�^�C���A�E�g�j
				nExitCode = -14;
				nAbort = 3;	//Timeout
				nStage = 10;
				}

			//�p�P�b�g�̊J��
			CCan1939::FreeBuffer(pPacket);
			}

		//DT�𑗂�X�e�[�W
		//	PC�����M�m�[�h�EABH3����M�m�[�h��1�O�̕ԓ�(CTS)�Ŏw�肳�ꂽ�f�[�^�𑗂�
		//	����ʒu(nPacketNum)�ƌ�(nMaxPacket)�́A1�O�̕ԓ�(CTS)�Ŏw�肳��Ă���
		//	�f�[�^�̑��M���s�񐔂́u�ʐM�悪�w�肵���񐔁v�ł���A������f�[�^���Z���ꍇ��
		//	���Ȃ��񐔂ƂȂ�
		else if(nStage == 2)
			{
			//�v������0?
			if(nMaxPacket < 1)
				{
				//�Œ�1�͑���
				nMaxPacket = 1;
				}
			//
			for(uint8_t nLoop = 0;nLoop < nMaxPacket;nLoop++)
				{
				//CM_DT�p�P�b�g���쐬�i�f�[�^�I�[�o�[��������ꍇ��NULL���߂�j
				uint8_t* pPacket = CCan1939::CreateCMDT(pSendData,nSendDataSize,nPacketNum);
				//���M�Ώۂ̃f�[�^���L��H
				if(pPacket)
					{
					//���M
					nResult = CanSend8(nSendDTID,pPacket,8);	//CM_DT���M

					//�p�P�b�g�̊J��
					CCan1939::FreeBuffer(pPacket);

					//����ʒu���X�V
					++nPacketNum;
					}

				//���M���s�H
				if(nResult)
					break;	//�r���Ŕ�����
				}

			//CM_DT�p�P�b�g�̑��M�Ɏ��s�H
			if(nResult)
				{
				//���M���s
				nExitCode = -13;
				nAbort = 2;	//resource�s��
				nStage = 10;
				}
			else
				nStage = 1;
			}

		//EOMA���󂯎�������RTS��M�X�e�[�W
		//	PC����M�m�[�h�EABH3�����M�m�[�h�ɖ����ύX���A�ԓ�(CM_RTS)���͂��͂��Ȃ̂ł������M
		else if(nStage == 3)
			{
			//��M�̈�\�z
			uint8_t* pPacket = CCan1939::CreateBuffer();
			nResult = CanRecv8(&nID,pPacket);

			//��M�͐���H
			if(nResult == 0)
				{
				//���̃p�P�b�g�H
				uint8_t nType = CCan1939::IsPacket(pPacket);
				//CM_RTS?
				if(nType == CCan1939::DEF_RTS)
					{
					//���b�Z�[�W�T�C�Y���擾���A�o�b�t�@���\�z
					nRecvDataSize = CCan1939::Get16L(pPacket,1);
					pRecvData = new uint8_t[nRecvDataSize + 1]();	//�I�[�p��+1���ď������ς݂Ŋm�ۂ���

					//�^�[�Q�b�g������w�肳�ꂽ���p�P�b�g����ۑ�
					nTotalPacket = CCan1939::Get8L(pPacket,3);

					//�^�[�Q�b�g������w�肳�ꂽ���M���p�P�b�g����ۑ�
					nMaxPacket = pPacket[4];

					//�p�P�b�g�ԍ���������
					nPacketNum = 1;

					//CTS��ԓ�����X�e�[�W�ɑJ��
					nStage = 4;
					}
				//CM_EOMA?
				else if(nType == CCan1939::DEF_EOMA)
					{
					nStage = 6;
					}
				//CM_ABORT?
				else if(nType == CCan1939::DEF_ABORT)
					{
					//���~����
					nExitCode = -11;
					nStage = 99;
					}
				//��L�ȊO�H
				else
					{
					//��L�ȊO�͌��Ȃ��������ɂ��čđ���҂�
					Sleep(0);
					}
				}
			else
				{
				//��M�������Ɏ��s�AABORT���M
				nExitCode = -14;
				nAbort = 3;	//Timeout
				nStage = 10;	
				}

			//�p�P�b�g�̊J��
			CCan1939::FreeBuffer(pPacket);
			}
		//CTS�ԓ��X�e�[�W
		//	PC����M�m�[�h�EABH3�����M�m�[�h�ŁA��M�v��(CM_RTS)���̓f�[�^(CM_DT)�̕ԓ�(CM_CTS)���K�v�Ȃ̂ł���𑗐M
		else if(nStage == 4)
			{
			//���M���p�P�b�g�̓^�[�Q�b�g�����w�肵���l�����̂܂ܗ��p

			//CM_CTS�p�P�b�g������đ��M
			uint8_t nRemainPacket = nMaxPacket - nPacketNum + 1;				//�c��̃p�P�b�g��
			uint8_t* pPacket = CCan1939::CreateCMCTS(nRemainPacket,nPacketNum);
			nResult = CanSend8(nSendID,pPacket,8);

			//�p�P�b�g�̊J��
			CCan1939::FreeBuffer(pPacket);

			//CM_CTS���M����H
			if(nResult == 0)
				nStage = 5;
			else
				{
				//���M���s
				nExitCode = -13;
				nAbort = 2;	//resource�s��
				nStage = 10;
				}
			}

		//DT��M�X�e�[�W
		//	PC����M�m�[�h�EABH3�����M�m�[�h�ŁA�v��(CM_CTS)���̓f�[�^(CM_DT)�̕ԓ�(CM_RTS)���K�v�Ȃ̂ł���𑗐M
		else if(nStage == 5)
			{
			//
			if(nMaxPacket < 1)
				{
				nMaxPacket = 1;
				}

			//��M�̈�\�z
			bool bRetry = false;
			uint8_t* pPacket = CCan1939::CreateBuffer();

			//���M�����f�[�^���i�\�萔�j�̐�������M�������s��
			for(uint8_t nLoop = 0;nLoop < nMaxPacket;nLoop++)
				{
				//��M
				nResult = CanRecv8(&nID,pPacket);

				//��M�͐���H
				if(nResult == 0)
					{
					//�V�[�P���X�ԍ����قȂ�H
					if(pPacket[0] != nPacketNum)
						{
						//�v�������̂ƈقȂ镨���͂����̂ŁA�ēx�v������
						bRetry = true;
						break;
						}
					
					//�f�[�^�i�[�i7�o�C�g�j
					CCan1939::SetData(pRecvData,nRecvDataSize,CCan1939::packetnum2datapt(nPacketNum),&pPacket[1],7);

					//�ŏI�p�P�b�g�ԍ��H
					if(nPacketNum == nTotalPacket)
						break;	//����

					//�v�������p�P�b�g�ԍ��œ͂����̂ŁA���̃p�P�b�g�ԍ��ɍX�V
					++nPacketNum;
					}
				else
					{
					//��M�����Ɏ��s
					nExitCode = -14;
					nAbort = 3;	//Timeout
					nStage = 10;
					break;
					}
				}

			//�p�P�b�g�̊J��
			CCan1939::FreeBuffer(pPacket);

			//���g���C���K�v�H
			if(bRetry)
				{
				//�p�P�b�g�ԍ�(nPacketNum)���������Ȃ��ӏ�����ēx�f�[�^(CM_DT)��v������
				nStage = 4;
				}

			//����H
			else if(nResult == 0)
				{
				if(nPacketNum == nTotalPacket)
					nStage = 6;	//EOMA���M�X�e�[�W�ɑJ��
				else
					nStage = 4;	//CTS�ԓ��ɖ߂�
				}

			else
				{
				//��M���s�A��M���[�`�����ňڍs�悪�ݒ肳��Ă���ׁA�������Ȃ�
				Sleep(0);
				}
			}

		//EOMA�ԓ��X�e�[�W
		else if(nStage == 6)
			{
			//�p�P�b�g������đ��M
			uint8_t* pPacket = CCan1939::CreateCMEOMA(nTotalPacket * 8,nTotalPacket);
			nResult = CanSend8(nSendID,pPacket,8);
	
			//�p�P�b�g�̊J��
			CCan1939::FreeBuffer(pPacket);

			//���M����H
			if(nResult == 0)
				{
				//�����X�e�[�W�Ɉڍs
				nStage = 7;
				}
			else
				{
				//���M���s
				nExitCode = -13;
				nAbort = 2;
				nStage = 10;
				}
			}

		//�����X�e�[�W
		else if(nStage == 7)
			{
			//���튮��
			break;
			}

		//�z�X�g�����璆�~�v���X�e�[�W
		//	CM_ABORT�𑗂�AnAbort�ɗ��R�����Ă���(2...���\�[�X�s��  3..�^�C���A�E�g)
		else if(nStage == 10)
			{
			//ABORT�p�P�b�g������đ��M
			uint8_t* pPacket = CCan1939::CreateCMABORT(nAbort);
			nResult = CanSend8(nSendID,pPacket,8);

			//�p�P�b�g�̊J��
			CCan1939::FreeBuffer(pPacket);

			//�G���[�X�e�[�W�֑J��
			nStage = 99;
			}

		//��L�ȊO�i�G���[�X�e�[�W�j
		else
			{
			//�ُ�I��
			break;
			}
		}

	//�I��
	return(nExitCode);
	}
