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
//	CAN�p�f�o�C�X�N���X
//	�iCAN�C���^�[�t�F�[�X�̎������s���N���X�̌p�����j
//
//����
//2023/02/24	yo0043		OnCanRecv�ɑ҂����ԂȂ��@�\�ǉ�
//
//�g�p���@
//�E�{�N���X���p�������N���X���\�z���A���z�֐��S�Ă��I�[�o���C�h���A��������������
//�ECAN����N���X(CAbh3)���̃R���X�g���N�^�ɁA�p�������N���X��n��
//	�i�p�������N���X�́ACAN����N���X�̔j�������܂ŁA���݂��Ă���K�v���L��j
//�E�{�N���X���p�������N���X�ł́A���M�y�ю�M���̃f�[�^���i�o�C�g�T�C�Y�j��AddCounter�ɓn��
//�@�i��L�A�v���P�[�V��������́A��莞�Ԗ���GetCounter�֐����g�p���A�擾��������Mbit(s)����
//	�@�ʐM�{�[���[�g�̒l����ACAN�o�X�̗��p�����Z�o�\�ƂȂ�j

#include "pch.h"
#include "CanIF.h"
#include <timeapi.h>	//Windows.h����ɂȂ�l�ɃC���N���[�h����

//TimeAPI(GetTM�֐��ׂ̈ɕK�v)
#pragma comment(lib,"winmm.lib")

//���p�\��CAN�C���^�[�t�F�[�X�����擾����ꍇ�ɌĂяo����܂�
int32_t CCanIF::OnGetInterfaceCount()
	{
	//�T�v
	//	�w��CAN�C���^�[�t�F�[�X�̗��p�\�����擾����ꍇ�ɌĂяo����܂�
	//�p�����[�^
	//	����
	//�߂�l
	//	���p�\�C���^�[�t�F�[�X��

	//todo:CAN�C���^�[�t�F�[�X�̐���߂��������������ĉ�����

	return(0);
	}

//�w��CAN�C���^�[�t�F�[�X���J���ꍇ�ɌĂяo����܂�
int32_t CCanIF::OnOpenInterface(int32_t nDeviceNum)
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

	return(-1);
	}

//�J����CAN�C���^�[�t�F�[�X�����ꍇ�ɌĂяo����܂�
void CCanIF::OnCloseInterface()
	{
	//�T�v
	//	�J����CAN�C���^�[�t�F�[�X�����ꍇ�ɌĂяo����܂�
	//�p�����[�^
	//	����
	//�߂�l
	//	����

	//todo:CAN�C���^�[�t�F�[�X���J���Ă���ꍇ�ɕ��鏈�����������ĉ�����

	}

//CAN�C���^�[�t�F�[�X�����M����ꍇ�ɌĂяo����܂�
int32_t CCanIF::OnCanRecv(uint32_t* pCanID,uint8_t* pData8,bool bNoWait /* false */)
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

	return(-1);
	}

//CAN�C���^�[�t�F�[�X�ɑ��M����ꍇ�ɌĂяo����܂�
int32_t CCanIF::OnCanSend(uint32_t nCanID,uint8_t* pData8,uint8_t nLength)
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

	return(-1);
	}

//CAN�C���^�[�t�F�[�X�ɃG���[���L�邩���ׂ�ꍇ�ɌĂяo����܂�
int32_t CCanIF::OnCanGetError()
	{
	//�T�v
	//	CAN�C���^�[�t�F�[�X�ɃG���[���L�邩���ׂ܂�
	//�p�����[�^
	//	����
	//�߂�l
	//	0				����
	//	0�ȊO			�ُ�

	//todo:CAN�C���^�[�t�F�[�X�ɃG���[���L�邩���ׂ鏈�����������ĉ�����

	return(-1);
	}

//CAN�C���^�[�t�F�[�X�̃G���[����������ꍇ�ɌĂяo����܂�
int32_t CCanIF::OnCanClearError()
	{
	//�T�v
	//	CAN�C���^�[�t�F�[�X�̃G���[���������܂�
	//�p�����[�^
	//	����
	//�߂�l
	//	0				����
	//	0�ȊO			�ُ�

	//todo:CAN�C���^�[�t�F�[�X�̃G���[���������鏈�����������ĉ�����

	return(-1);
	}

//��M�o�b�t�@���N���A����ꍇ�ɌĂяo����܂�
int32_t CCanIF::OnCanRecvClear()
	{
	//�T�v
	//	��M�o�b�t�@���N���A����ꍇ�ɌĂяo����܂�
	//�p�����[�^
	//	����
	//�߂�l
	//	0				����
	//	0�ȊO			�ُ�

	//todo:��M�o�b�t�@���N���A���鏈�����������ĉ�����

	return(-1);
	}

//================================================================================
//��ʂ���̐ݒ���󂯂�֐�
//================================================================================

//CAN�C���^�[�t�F�[�X�̃^�C���A�E�g��ݒ�
void CCanIF::SetTimeout(uint32_t nOpenMS,uint32_t nSendMS,uint32_t nRecvMS)
	{
	//�T�v
	//	CAN�C���^�[�t�F�[�X�̃^�C���A�E�g���ꊇ�ݒ�
	//�p�����[�^
	//	nOpenMS		������J�����̃^�C���A�E�g[ms]
	//	nSendMS		���M���̃^�C���A�E�g[ms]
	//	nRecvMS		��M���̃^�C���A�E�g[ms]
	//�߂�l
	//	����

	m_var.timeout.nOpen = nOpenMS;
	m_var.timeout.nSend = nSendMS;
	m_var.timeout.nRecv = nRecvMS;
	}
void CCanIF::SetTimeout_Open(uint32_t nTimeoutMS)
	{
	m_var.timeout.nOpen = nTimeoutMS;
	}
void CCanIF::SetTimeout_Send(uint32_t nTimeoutMS)
	{
	m_var.timeout.nSend = nTimeoutMS;
	}
void CCanIF::SetTimeout_Recv(uint32_t nTimeoutMS)
	{
	m_var.timeout.nRecv = nTimeoutMS;
	}

//���݂�CAN�C���^�[�t�F�[�X�^�C���A�E�g�ݒ���擾
void CCanIF::GetTimeout(uint32_t& nOpenMS,uint32_t& nSendMS,uint32_t& nRecvMS)
	{
	//�T�v
	//	CAN�C���^�[�t�F�[�X�̃^�C���A�E�g���ꊇ�擾
	//�p�����[�^
	//	nOpenMS		������J�����̃^�C���A�E�g[ms]���i�[����Q��
	//	nSendMS		���M���̃^�C���A�E�g[ms]���i�[����Q��
	//	nRecvMS		��M���̃^�C���A�E�g[ms]���i�[����Q��
	//�߂�l
	//	����
	nOpenMS = m_var.timeout.nOpen;
	nSendMS = m_var.timeout.nSend;
	nRecvMS = m_var.timeout.nRecv;
	}
uint32_t CCanIF::GetTimeout_Open(void)
	{
	return(m_var.timeout.nOpen);
	}
uint32_t CCanIF::GetTimeout_Send(void)
	{
	return(m_var.timeout.nSend);
	}
uint32_t CCanIF::GetTimeout_Recv(void)
	{
	return(m_var.timeout.nRecv);
	}

//CAN�C���^�[�t�F�[�X�̃{�[���[�g��ݒ�
void CCanIF::SetBaudrate(uint32_t nBaudrate)
	{
	//�T�v
	//	CAN�C���^�[�t�F�[�X�̃{�[���[�g��ݒ�
	//�p�����[�^
	//	nBaudrate		�f�o�C�X�ˑ��̃{�[���[�g���w�肷��l
	//�߂�l
	//	����
	//���ӓ_
	//	�ݒ�l�̒P�ʓ��́A�f�o�C�X�Ɉˑ�����
	//	
	m_var.nBaudrate = nBaudrate;
	}

//���݂�CAN�C���^�[�t�F�[�X�̃{�[���[�g���擾
uint32_t CCanIF::GetBaudrate(void)
	{
	return(m_var.nBaudrate);
	}

//�������锭�M��ID���w��
void CCanIF::SetIgnoreID(uint8_t nID)
	{
	m_var.nIgnoreID = nID;
	}

//�������锭�M��ID���擾
uint8_t CCanIF::GetIgnoreID(uint8_t nID)
	{
	return(m_var.nIgnoreID);
	}

//================================================================================
//��ʊ܂߂ė��p�\�Ȋ֐�
//================================================================================

//���ݎ�����[ms]�P�ʂŎ擾
uint32_t CCanIF::GetTm()
	{
	MMTIME x;
	timeGetSystemTime(&x,sizeof(MMTIME));
	return(uint32_t(x.u.ticks));
	}


//����M�T�C�Y�J�E���^���擾
uint32_t CCanIF::GetCounter()
	{
	//�T�v
	//	�Ō�ɂ��̊֐��𗘗p���Ă��獡��̌Ăяo�����̑���M�T�C�Y���r�b�g�P�ʂŖ߂�
	//���ӓ_
	//	�A�v���P�[�V�����w�����GetCounter�֐����r�I���m�Ȉ������Ŏ擾����K�v���L��B

	//���݂̃J�E���^��ۑ����A�J�E���^������������
	m_var.counter.nOldBitSize = m_var.counter.nBitSize;
	m_var.counter.nBitSize = 0;
	//���J�E���^�̒l�i�ۑ����ꂽ�l�j��߂�
	return(m_var.counter.nOldBitSize);
	}
