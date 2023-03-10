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

//
#include <Windows.h>
#include <stdint.h>

//
class CCanIF
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

//================================================================================
//�ݒ�p�֐�
//================================================================================
public:

	//CAN�C���^�[�t�F�[�X�̃^�C���A�E�g��ݒ�
	void SetTimeout(uint32_t nOpenMS,uint32_t nSendMS,uint32_t nRecvMS);
	void SetTimeout_Open(uint32_t nTimeoutMS);
	void SetTimeout_Send(uint32_t nTimeoutMS);
	void SetTimeout_Recv(uint32_t nTimeoutMS);

	//���݂�CAN�C���^�[�t�F�[�X�^�C���A�E�g�ݒ���擾
	void GetTimeout(uint32_t& nOpenMS,uint32_t& nSendMS,uint32_t& nRecvMS);
	uint32_t GetTimeout_Open(void);
	uint32_t GetTimeout_Send(void);
	uint32_t GetTimeout_Recv(void);

	//CAN�C���^�[�t�F�[�X�̃{�[���[�g��ݒ�
	void SetBaudrate(uint32_t nBaudrate);

	//���݂�CAN�C���^�[�t�F�[�X�̃{�[���[�g���擾
	uint32_t GetBaudrate(void);

	//�������锭�M��ID���w��
	void SetIgnoreID(uint8_t nID);

	//�������锭�M��ID���擾
	uint8_t GetIgnoreID(uint8_t nID);

//================================================================================
//��ʊ܂߂ė��p�\�Ȋ֐�
//================================================================================
public:

	//���ݎ�����[ms]�P�ʂŎ擾
	uint32_t GetTm(void);

	//����M�T�C�Y�J�E���^���擾
	uint32_t GetCounter(void);

//================================================================================
//�p����̂ݗ��p�\�Ȋ֐�
//================================================================================
protected:

	//�w��ID����������Ώۂ����ׂ܂�
	bool IsIgnoreID(uint32_t nID)
		{
		//�T�v
		//	���O�ݒ肳�ꂽ�u��������ID�v�Ɣ�r���܂�
		//�p�����[�^
		//	nID		��r����ID�i�ʏ�͎�M����CAN�p�P�b�g�̔��M�����w�肷��j
		//�߂�l
		//	true	��������ΏۂƔ��f
		//	false	�������Ȃ��ΏۂƔ��f
		//���ӓ_
		//	��{�I�ɂ́A��������Ώۂ�ID�Ƃ��āA���O�Ƀz�X�g��(PC)��ID���w�肵�Ă���
		//�@����͉�����J�������Ɏ����ݒ肳���
		//	CAN�p�P�b�g����M������A���̃p�P�b�g�̔��M�������̊֐��Ń`�F�b�N����
		//	����́uPC�����瑗�M���ꂽ�p�P�b�g�v����M�����ׂ̑[�u�ƂȂ�B
		//

		//�w��ID���̂�0���H�i���M��ID��0�͂��蓾�Ȃ��j
		if(nID == 0)
			return(true);		//����ȃp�P�b�g�ł͂Ȃ��̂Ŗ�������

		//���蔭�M���𖳎�����ݒ肩�H
		if(m_var.nIgnoreID)
			{
			//�w��ID�̔��M������v���邩�H
			if(m_var.nIgnoreID == uint8_t(nID))
				return(true);	//��v���Ă���̂Ŗ�������
			}
		
		//�����ɍ���Ȃ��̂Ŗ������Ȃ�
		return(false);
		}

	//��M�T�C�Y�J�E���^�ɒl�����Z
	void AddCounter(uint32_t nDLCsize)
		{
		//���ۂ̃o�X��L�T�C�Y�́A�w�b�_54bits + �f�[�^bits��
		uint32_t nBitSize = 54 + (nDLCsize * 8);
		m_var.counter.nBitSize += nBitSize;
		}

//================================================================================
//�����p
//================================================================================
private:

	//�����ϐ�
	typedef struct _CANIF_VAR
		{
		uint32_t	nBaudrate;
		uint8_t		nIgnoreID;			//0�ȊO�œ����l�̔��M��(ID�̉���8bit)�𖳎�����
		//uint32_t	nOnlyID;			//0�ȊO�ł���ID�̂ݎ�M����inIgnoreID���D��j
		struct _TMO
			{
			uint32_t	nOpen;
			uint32_t	nSend;
			uint32_t	nRecv;
			} timeout;
		struct _COUNTER
			{
			uint32_t	nOldBitSize;//1�O�̊���Ԃő���M�����r�b�g�T�C�Y
			uint32_t	nBitSize;	//���݂̊���ԓ��ɑ���M�����r�b�g�T�C�Y
			} counter;


		} CANIF_VAR,*pCANIF_VAR;
	CANIF_VAR m_var;

public:
	//�R���X�g���N�^
	CCanIF()
		{
		::memset(&m_var,0,sizeof(CANIF_VAR));
		}
	//�f�X�g���N�^
	virtual ~CCanIF()
		{
		}
};

