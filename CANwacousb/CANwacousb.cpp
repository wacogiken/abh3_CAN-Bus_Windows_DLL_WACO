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
//	�{DLL�ŃG�N�X�|�[�g����֐����i�[���Ă���\�[�X�R�[�h
#include "pch.h"

//����̌x���𖳎�
#pragma warning(disable : 4996)

//#define _NO_CANABH3API_EXPORT	//��`�Ȃ��ŃG�N�X�|�[�g�A��`�L��ŃC���|�[�g
#include "CANwacousb.h"

//���S�ȍ폜
template <class T> void SafeDelete(T** ppT)
	{
	if (*ppT)
		{
		delete *ppT;
		*ppT = NULL;
		}
	}

//ABH3����N���X�ƃC���^�[�t�F�[�X�N���X�ւ̃|�C���^�i�O���[�o���j
static CAbh3*		g_pAbh3 = NULL;
static CWacoCanUsb*	g_pIF = NULL;

//================================================================================
//���[�U�[�p�֐�(Windows�ŗL�n)
//================================================================================

//DLL�g�p�J�n���ɌĂяo���K�v���L��܂�
CANABH3API void InitInstance(int32_t nIFnum)
	{
	//�T�v
	//	DLL�g�p�J�n���ɌĂяo���K�v���L��܂�
	//�p�����[�^
	//	nIFnum		�g�p�C���^�[�t�F�[�X�i0...Ixxat USB-to-CAN V2�j
	//�߂�l
	//	����
	//���ӓ_
	//	�{���i�ł́AIxxat USB-to-CAN V2�̂݃T�|�[�g����܂�

	//
	ExitInstance();
	//
	g_pIF = new CWacoCanUsb();
	g_pAbh3 = new CAbh3(g_pIF);
	}

//DLL�g�p�I�����ɌĂяo���K�v���L��܂�
CANABH3API void ExitInstance()
	{
	//�T�v
	//	DLL�g�p�I�����ɌĂяo���K�v���L��܂�
	//�p�����[�^
	//	����
	//�߂�l
	//	����
	//���ӓ_
	//	DLL�J�����O�ŌĂяo���ĉ�����

	SafeDelete(&g_pAbh3);
	SafeDelete(&g_pIF);
	}

//�g�p�\��CAN�C���^�[�t�F�[�X�����擾
CANABH3API int32_t GetInterfaceCount()
	{
	return(g_pAbh3->GetInterfaceCount());
	}

//�C���^�[�t�F�[�X���w�肷��
CANABH3API int32_t SetInterface(int32_t nDeviceNum)
	{
	return(g_pAbh3->SetInterface(nDeviceNum));
	}

//���݊J���Ă���C���^�[�t�F�[�X�ԍ����擾
CANABH3API int32_t GetCurrentInterface()
	{
	return(g_pAbh3->GetCurrentInterface());
	}

//���݃C���^�[�t�F�[�X���J���Ă��邩�H
CANABH3API int32_t IsOpenInterface()
	{
	return(g_pAbh3->IsOpenInterface());
	}

//�C���^�[�t�F�[�X���J���^�C���A�E�g���Ԃ�ݒ�
CANABH3API void SetOpenTimeout(uint32_t nTimeoutMS)
	{
	g_pAbh3->SetOpenTimeout(nTimeoutMS);
	}

//���M�^�C���A�E�g���Ԃ�ݒ�
CANABH3API void SetSendTimeout(uint32_t nTimeoutMS)
	{
	g_pAbh3->SetSendTimeout(nTimeoutMS);
	}

//��M�^�C���A�E�g���Ԃ�ݒ�
CANABH3API void SetRecvTimeout(uint32_t nTimeoutMS)
	{
	g_pAbh3->SetRecvTimeout(nTimeoutMS);
	}

//�ʐM�z�X�g�̃A�h���X��ݒ�
CANABH3API void SetHostID(uint8_t nAdrs)
	{
	g_pAbh3->SetHostID(nAdrs);
	}

//�ʐM�z�X�g�̃A�h���X���擾
CANABH3API uint8_t GetHostID(void)
	{
	return(g_pAbh3->GetHostID());
	}

//�ʐM���x���w��
CANABH3API void SetBaudrate(uint32_t nBaudrateKbps)
	{
	g_pAbh3->SetBaudrate(nBaudrateKbps);
	}

//�ݒ肵���ʐM���x���擾
CANABH3API uint32_t GetBaudrate()
	{
	return(g_pAbh3->GetBaudrate());
	}

//PC���N�����Ă���̎��Ԃ�[ms]�P�ʂŎ擾
CANABH3API uint32_t GetTm()
	{
	return(g_pAbh3->GetTm());
	}

//1�b�ԂɎ�M�����f�[�^�ʂ��擾���܂�
CANABH3API uint32_t GetCounter()
	{
	return(g_pAbh3->GetCounter());
	}

//����M�J�E���^���擾���܂����܂�
CANABH3API uint32_t GetTransmitCounter(uint8_t nRecv)
	{
	return(g_pAbh3->GetTransmitCounter(nRecv));
	}

//CAN�o�X�ɃG���[���������Ă��邩���ׂ܂�
CANABH3API uint32_t GetCANerror()
	{
	return(g_pAbh3->GetCANerror());
	}

//CAN�o�X�̃G���[�������s���܂�
CANABH3API uint32_t ResetCANerror()
	{
	return(g_pAbh3->ResetCANerror());
	}

//================================================================================
//���[�U�[�p�֐�(�ʐM�n)
//================================================================================

//�w�߂̏������{CAN�|�[�g���J��
CANABH3API int32_t abh3_can_init(uint8_t nTargetID,pCANABH3_RESULT pPtr)
	{
	//�T�v
	//	�w�߂̏������{CAN�|�[�g���J��
	//�ڍ�
	//	�ȉ��̏��ŏ������s��
	//	(1)CAN�|�[�g���J��
	//	(2)A/Y�w�߁AB/X�w�߂�0�ɐݒ肵�A�ڑ���ɃV���O���p�P�b�gDP0�𑗐M
	//	(3)(��M�o�b�t�@�w�莞�̂�)�V���O���p�P�b�gDP0�̎�M
	//�p�����[�^
	//	pPtr		��M�o�b�t�@�iNULL�̏ꍇ�͎�M���Ȃ��j
	//�߂�l
	//	0			����I��
	//	��L�ȊO	�ُ픭��
	//���ӓ_
	//	����

	return(g_pAbh3->abh3_can_init(nTargetID,pPtr));
	}

//CAN�|�[�g���J��
CANABH3API int32_t abh3_can_port_init(void)
	{
	//�T�v
	//	CAN�|�[�g���J��
	//�ڍ�
	//	CAN�|�[�g���J��
	//�p�����[�^
	//	����
	//�߂�l
	//	0			����I��
	//	��L�ȊO	�ُ픭��
	//���ӓ_
	//	����

	return(g_pAbh3->abh3_can_port_init());
	}

//�w�߂̏�����
CANABH3API int32_t abh3_cmd_init(uint8_t nTargetID,pCANABH3_RESULT pPtr)
	{
	//�T�v
	//	�w�߂̏�����
	//�ڍ�
	//	�ȉ��̏��ŏ������s��
	//	(1)A/Y�w�߁AB/X�w�߂�0�ɐݒ肵�A�ڑ���ɃV���O���p�P�b�gDP0�𑗐M
	//	(2)(��M�o�b�t�@�w�莞�̂�)�V���O���p�P�b�gDP0�̎�M
	//�p�����[�^
	//	pPtr		��M�o�b�t�@�iNULL�̏ꍇ�͎�M���Ȃ��j
	//�߂�l
	//	0			����I��
	//	��L�ȊO	�ُ픭��
	//���ӓ_
	//	����

	return(g_pAbh3->abh3_can_cmd_init(nTargetID,pPtr));
	}

//���݂̎�M�f�[�^�󋵂��ꊇ�擾
CANABH3API void abh3_can_copylastdata(uint8_t nTargetID,pCANABH3_LASTRECV pPtr)
	{
	//�T�v
	//	���݂̒ʐM�悩���M�����f�[�^�̈�̕������擾����
	//�ڍ�
	//	���݂̒ʐM�悩���M�����f�[�^�𔽉f�������\���̂��擾
	//�p�����[�^
	//	pPtr		�f�[�^�̈�̕����i�[��
	//�߂�l
	//	����
	//���ӓ_
	//	�擾�����̈�̓��A�X�V�t���O���������Ă���̈�̂ݗ��p���鎖
	//	�X�V�t���O�̉����́Aabh3_can_resetlastdata�֐��ōs����

	g_pAbh3->abh3_can_copylastdata(nTargetID,pPtr);
	}

//���݂̎�M�f�[�^�̈�̍X�V�t���O����������
CANABH3API void abh3_can_resetlastdata(uint8_t nTargetID,int32_t nAdrs)
	{
	//�T�v
	//	���݂̎�M�f�[�^�̈�̍X�V�t���O����������
	//�ڍ�
	//	��M�f�[�^�̈�Ɋ܂܂��X�V�t���O�̓��A�w��ӏ����N���A����
	//�p�����[�^
	//	nAdrs		�X�V�t���O����������A�h���X�A���� -1(�S�̈�)���w��
	//�߂�l
	//	����
	//���ӓ_
	//	abh3_can_copylastdata�֐��𗘗p���Ă���A�{�֐��𗘗p���閘��
	//	1�ȏ�̃p�P�b�g����M�\�ȏꍇ�A�p�����[�^�ɑS�̈���w�肵��
	//	���s����ƁA�������̍X�V�t���O���N���A����Ă��܂��������鎖�ɒ��ӁB
	//	��{�I�ɂ́Aabh3_can_copylastdata�Ŏ擾�����̈�������������鎞�ɁA
	//	�s�x�{�֐��ɃA�h���X���w�肵�ĉ����v�����鎖���]�܂���

	g_pAbh3->abh3_can_resetlastdata(nTargetID,nAdrs);
	}

//��M�Ώۂ��w�肵�Ď�M
CANABH3API int32_t abh3_can_read(uint8_t nTargetID,pCANABH3_RESULT pPtr,PACKETTYPE nType,uint8_t nAdrs)
	{
	//�T�v
	//	�w������ɍ���CAN�p�P�b�g����M����
	//�p�����[�^
	//	nTargetID	��M�Ώۂ��w�肷�锭�M��ID
	//	pPtr		��M�o�b�t�@
	//	nCode		��M�Ώۂ��w�肷��R�[�h
	//�߂�l
	//	0			����I��
	//	��L�ȊO	�ُ픭��
	//�ڍ�
	//	���Ɏ�M�o�b�t�@�ɏ��������������L��΁A������擾���Ė߂�
	//	�����ꍇ�́A��M�^�C���A�E�g���Ԗ��A��M��҂�
	//���ӓ_
	//	����
		
	return(g_pAbh3->abh3_can_read(nTargetID,pPtr,nType,nAdrs));
	}

//��M�o�b�t�@�̓��e����M�f�[�^�󋵂ɔ��f������
CANABH3API int32_t abh3_can_flush()
	{
	//�T�v
	//	��M�o�b�t�@�̓��e����M�f�[�^�󋵂ɔ��f������
	//�p�����[�^
	//	����
	//�߂�l
	//	��������
	//�ڍ�
	//	��M�o�b�t�@�ɂ���f�[�^�S�āi�v�������ő����Ă���ꍇ����z��j��
	//	�擾���A�u��M�f�[�^�󋵁v�ɔ��f������
	//���ӓ_
	//	�v�������ł̓z�X�g�։��������ė��Ȃ��ꍇ�A���̏����͕s�v�ƂȂ�
	//	�v�������Ńz�X�g�֑����Ă���f�[�^���L��ꍇ�́A����������
	//	���̊֐����g�p���āu��M�f�[�^�󋵁v�Ƀf�[�^���X�V�����鎖�ŁA
	//	�l�����f������B�i��M�f�[�^�̒l���K�v�ȏꍇ�́A�u��M�f�[�^�󋵁v�𗘗p���鎖�j
		
	return(g_pAbh3->abh3_can_flush());
	}

//�w�߂̑��M�i���ʁj
CANABH3API int32_t abh3_can_cmdAY(uint8_t nTargetID,int16_t cmd,pCANABH3_RESULT pPtr)
	{
	//�T�v
	//�p�����[�^
	//	pPtr		��M�o�b�t�@�iNULL�̏ꍇ�͎�M���Ȃ��j
	//�߂�l
	//	0			����I��
	//	��L�ȊO	�ُ픭��
	//���ӓ_
	//	����

	return(g_pAbh3->abh3_can_cmdAY(nTargetID,cmd,pPtr));
	}
CANABH3API int32_t abh3_can_cmdBX(uint8_t nTargetID,int16_t cmd,pCANABH3_RESULT pPtr)
	{
	//�T�v
	//�p�����[�^
	//	pPtr		��M�o�b�t�@�iNULL�̏ꍇ�͎�M���Ȃ��j
	//�߂�l
	//	0			����I��
	//	��L�ȊO	�ُ픭��
	//���ӓ_
	//	����

	return(g_pAbh3->abh3_can_cmdBX(nTargetID,cmd,pPtr));
	}

//�w�߂̑��M�i�����j
CANABH3API int32_t abh3_can_cmd(uint8_t nTargetID,int16_t cmdAY,int16_t cmdBX,pCANABH3_RESULT pPtr)
	{
	//�T�v
	//�p�����[�^
	//	pPtr		��M�o�b�t�@�iNULL�̏ꍇ�͎�M���Ȃ��j
	//�߂�l
	//	0			����I��
	//	��L�ȊO	�ُ픭��
	//���ӓ_
	//	����

	return(g_pAbh3->abh3_can_cmd(nTargetID,cmdAY,cmdBX,pPtr));
	}

//���͂̑��M�i�ꊇ�j
CANABH3API int32_t abh3_can_opSet(uint8_t nTargetID,int32_t data,int32_t mask,pCANABH3_RESULT pPtr)
	{
	//�T�v
	//�p�����[�^
	//	pPtr		��M�o�b�t�@�iNULL�̏ꍇ�͎�M���Ȃ��j
	//�߂�l
	//	0			����I��
	//	��L�ȊO	�ُ픭��
	//���ӓ_
	//	����

	return(g_pAbh3->abh3_can_opSet(nTargetID,data,mask,pPtr));
	}

//���͂̑��M�i�r�b�g�j
CANABH3API int32_t abh3_can_opBitSet(uint8_t nTargetID,int8_t num,int8_t data,pCANABH3_RESULT pPtr)
	{
	//�T�v
	//�p�����[�^
	//	pPtr		��M�o�b�t�@�iNULL�̏ꍇ�͎�M���Ȃ��j
	//�߂�l
	//	0			����I��
	//	��L�ȊO	�ُ픭��
	//���ӓ_
	//	����

	return(g_pAbh3->abh3_can_opBitSet(nTargetID,num,data,pPtr));
	}

//�w�߂Ɠ��͂̑��M�i�����j
CANABH3API int32_t abh3_can_cmdAndopSet(uint8_t nTargetID,int16_t cmdAY,int16_t cmdBX,int32_t data,int32_t mask,pCANABH3_RESULT pPtr)
	{
	//�T�v
	//�p�����[�^
	//	pPtr		��M�o�b�t�@�iNULL�̏ꍇ�͎�M���Ȃ��j
	//�߂�l
	//	0			����I��
	//	��L�ȊO	�ُ픭��
	//���ӓ_
	//	����

	return(g_pAbh3->abh3_can_cmdAndopSet(nTargetID,cmdAY,cmdBX,data,mask,pPtr));
	}



//�u���[�h�L���X�g�p�P�b�g�̃��N�G�X�g
CANABH3API int32_t abh3_can_reqBRD(uint8_t nTargetID,uint8_t num,uint8_t nBroadcast,pCANABH3_RESULT pPtr)
	{
	//�T�v
	//�p�����[�^
	//	pPtr		��M�o�b�t�@�iNULL�̏ꍇ�͎�M���Ȃ��j
	//�߂�l
	//	0			����I��
	//	��L�ȊO	�ُ픭��
	//���ӓ_
	//	����

	return(g_pAbh3->abh3_can_reqBRD(nTargetID,num,nBroadcast,pPtr));
	}

//�}���`�p�P�b�g�ɂ��TelABH3�p�P�b�g�̑���M
CANABH3API int32_t abh3_can_trans(uint8_t nTargetID,char* sbuf,char* rbuf,uint32_t* rbuflen)
	{
	//���ӓ_
	//	�A�v����DLL��HEAP������ɂȂ�Ȃ��悤�Ȏg�����i��FMFC�A�v���Ŏg�p�j��
	//	�����ꍇ�ADLL���Ŋm�ۂ����o�b�t�@���A�v�����ŊJ���o���Ȃ�(ASSERT�����)�ׁA
	//	�����ŃA�v�����̃o�b�t�@��DLL���̃o�b�t�@���e���R�s�[����
	//
	//�p�����[�^
	//	sbuf		���M�f�[�^(ANSI)
	//	rbuf		��M�f�[�^�i�[�̈�i�A�v�����Ŋm�ۂ���j
	//	rbuflen		��M�f�[�^�i�[�̈�̃T�C�Y�i�A�v�����Őݒ肵�A�{�֐��Ŋi�[�T�C�Y������j
	//�߂�l
	//	0			����
	//	-1			�ُ�
	//	-2			�o�b�t�@�T�C�Y������Ȃ�
	
	//
	char* pTmp = NULL;
	size_t nTmpLen = 0;
	int32_t nResult = g_pAbh3->abh3_can_trans(nTargetID,sbuf,pTmp,nTmpLen);
	//����H
	if(nResult == 0)
		{
		//�Œ���K�v�ȃT�C�Y��ݒ�
		*rbuflen = (uint32_t)nTmpLen;

		//�T�C�Y�������H
		if(*rbuflen >= nTmpLen)
			{
			::CopyMemory(rbuf,pTmp,nTmpLen);
			}
		else
			{
			//����Ȃ�
			nResult = -2;		//�̈�T�C�Y������Ȃ�����
			}
		//���p���Ă���ׁA�J�����K�v
		SafeDelete(&pTmp);
		}
	//
	return(nResult);
	}

//��������
CANABH3API int32_t abh3_can_finish(void)
	{
	//�T�v
	//�p�����[�^
	//�߂�l
	//	����
	//���ӓ_
	//	����

	return(g_pAbh3->abh3_can_finish());
	}

//================================================================================
//���[�U�[�p�֐�(�P�ʕϊ��n)
//================================================================================

//���x
CANABH3API int16_t cnvVel2CAN(float vel)
	{
	return(g_pAbh3->cnvVel2CAN(vel));
	}
CANABH3API float cnvCAN2Vel(int16_t vel)
	{
	return(g_pAbh3->cnvCAN2Vel(vel));
	}

//�d���l
CANABH3API int16_t cnvCur2CAN(float cur)
	{
	return(g_pAbh3->cnvCur2CAN(cur));
	}
CANABH3API float cnvCAN2Cur(int16_t cur)
	{
	return(g_pAbh3->cnvCAN2Cur(cur));
	}

//���ח�
CANABH3API float cnvCAN2Load(int16_t load)
	{
	return(g_pAbh3->cnvCAN2Load(load));
	}

//�A�i���O����
CANABH3API float cnvCAN2Analog(int16_t analog)
	{
	return(g_pAbh3->cnvCAN2Analog(analog));
	}

//�d���d��
CANABH3API float cnvCAN2Volt(int16_t volt)
	{
	return(g_pAbh3->cnvCAN2Volt(volt));
	}

//����
CANABH3API PACK_FLOAT flt_to_pkflt(float flt)
	{
	CPackFloat pf(flt);
	PACK_FLOAT result;
	result.u.nDirectData = pf.u.nDirectData;
	return(result);
	}
CANABH3API float pkflt_to_flt(PACK_FLOAT okflt)
	{
	CPackFloat pf(okflt);
	float result = 0.0f;
	pf.GetValue(result);
	return(result);
	}
CANABH3API int str_to_pkfltA(const char* str,PACK_FLOAT* p_pkflt)
	{
	CPackFloat pf((char*)str);
	p_pkflt->u.nDirectData = pf.u.nDirectData;
	return(0);
	}
CANABH3API int str_to_pkfltW(const wchar_t* str,PACK_FLOAT* p_pkflt)
	{
	CPackFloat pf((wchar_t*)str);
	p_pkflt->u.nDirectData = pf.u.nDirectData;
	return(0);
	}
CANABH3API char* pkflt_to_strA(PACK_FLOAT pkflt,char* str)
	{
	//���ӁE�i�[��̈�̃T�C�Y���s��Ȃ��ׁA�\���ȃo�b�t�@��n���K�v���L��
	CPackFloat pf(pkflt);
	char* pTmp = new char[128];
	pf.GetValueA(pTmp,128);
	strcpy(str,pTmp);
	delete [] pTmp;
	return(str);
	}
CANABH3API wchar_t* pkflt_to_strW(PACK_FLOAT pkflt,wchar_t* str)
	{
	//���ӁE�i�[��̈�̃T�C�Y���s��Ȃ��ׁA�\���ȃo�b�t�@��n���K�v���L��
	CPackFloat pf(pkflt);
	wchar_t* pTmp = new wchar_t[128];
	pf.GetValueW(pTmp,128);
	wcscpy(str,pTmp);
	delete [] pTmp;
	return(str);
	}
