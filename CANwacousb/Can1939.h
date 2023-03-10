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

#include <windows.h>
#include <stdint.h>

//�T�v
//	CAN�p�P�b�g�̒��g(SAE1939�d�l)���\�z����ׂ̃N���X
//	ID�ɂ��Ă͈�؍l�����Ȃ�
//
//�X�V����
//	2020.xx.xx	yo0043	1st release
//	2021.02.15	yo0043	2nd release
//						�R�����g�ǉ����i�R�[�h�ɕύX�����j
//


class CCan1939
{
public:

	//�񋓎q
	typedef enum
		{
		//�}���`�p�P�b�g�֘A��`
		 DEF_RTS			= 0x10			//CM.RTS�̐擪�o�C�g�ɓ���l
		,DEF_CTS			= 0x11			//CM.CTS�̐擪�o�C�g�ɓ���l
		,DEF_EOMA			= 0x13			//CM.EOMA�̐擪�o�C�g�ɓ���l
		,DEF_BAM			= 0x20			//CM_BAM�̐擪�o�C�g�ɓ���l
		,DEF_ABORT			= 0xff			//CM_ABORT�̐擪�o�C�g�ɓ���l
		,DEF_SIGN			= 0x00ef00		//CM_*�Ŏg�����А�p�V�O�l�`��(6,7,8�o�C�g�ڂɓ���)
		,DEF_UNKNOWN		= 0x00			//�}���`�p�P�b�g��M���ʎ��A�s���Ȍ��ʂ��o���ꍇ�̒l
		} ENUM;

public:

	//CAN�o�b�t�@�̍\�z
	static uint8_t* CreateBuffer(uint8_t nInitValue = 0x00)
		{
		//�T�v
		//	CAN�Ŏg�p����8[bytes]�̃o�b�t�@���\�z
		//�p�����[�^
		//	nInitValue		�o�b�t�@�̏����l
		//�߂�l
		//	�m�ۂ����o�b�t�@�ւ̃|�C���^
		//���ӓ_
		//	�m�ۂ����o�b�t�@�́A�Ăяo�����ŊJ��(delete)���邩�A
		//	FreeBuffer�����o�֐����g�p���ĊJ������K�v���L��܂�

		uint8_t* pResult = new uint8_t[8];
		::FillMemory(pResult,8,nInitValue);
		return(pResult);
		}

	//CAN�o�b�t�@�̊J��
	static void FreeBuffer(uint8_t* pBuffer)
		{
		//�T�v
		//	CreateBuffer�����o�֐��ō\�z�����o�b�t�@�̊J��
		//�p�����[�^
		//	pBuffer			�o�b�t�@�ւ̃|�C���^
		//�߂�l
		//	����
		//���ӓ_
		//	pBuffer������(NULL�w��)�̏ꍇ�́A�������܂���

		if(pBuffer)
			delete pBuffer;
		}

	//�K�؂Ȉʒu�Ƀf�[�^���i�[
	static void SetData(uint8_t* pDst,uint32_t nDstSize,uint32_t nDstPt,uint8_t* pSrc,uint32_t nSrcSize)
		{
		//�T�v
		//	�K�؂Ȉʒu�Ƀf�[�^���i�[
		//�p�����[�^
		//	pDst		�������ݐ�f�[�^�̐擪�ʒu
		//	nDstSize	�������ݐ�f�[�^�̗̈�T�C�Y
		//	nDstPt		�������ݐ�̗v�f�ԍ�
		//	pSrc		�������݃f�[�^�̐擪�ʒu
		//	nSrcSize	�������݃f�[�^�T�C�Y
		//�߂�l
		//	����
		//���ӓ_
		//	�i�[��̃o�b�t�@�͕K���m�ۍς݂ł��鎖

		//�������݉\�T�C�Y
		uint32_t nRemain = nDstSize - nDstPt;
		if(nRemain < nSrcSize)
			nSrcSize = nRemain;

		//��������
		::memcpy(&pDst[nDstPt],pSrc,size_t(nSrcSize));
		}

	//�T�v
	//	�l�̐ݒ�(8,16,24,32bit�w��j
	//�p�����[�^
	//	pBuffer		�i�[��z��̐擪�v�f
	//	nPt			�i�[��z��̗v�f�ԍ�
	//	nValue		�i�[�f�[�^
	//�߂�l
	//	����
	//���l
	//	�o�b�t�@�̐擪����LSB���ɑ�������
	static void Set8L(uint8_t* pBuffer,int32_t nPt,uint8_t nValue)
		{
		pBuffer[nPt] = nValue;
		}
	static void Set16L(uint8_t* pBuffer,int32_t nPt,uint16_t nValue)
		{
		Set8L(pBuffer,nPt,uint8_t(nValue));
		Set8L(pBuffer,nPt + 1,uint8_t(nValue >> 8));
		}
	static void Set24L(uint8_t* pBuffer,int32_t nPt,uint32_t nValue)
		{
		Set8L(pBuffer,nPt,uint8_t(nValue));
		Set8L(pBuffer,nPt + 1,uint8_t(nValue >> 8));
		Set8L(pBuffer,nPt + 2,uint8_t(nValue >> 16));
		}
	static void Set32L(uint8_t* pBuffer,int32_t nPt,uint32_t nValue)
		{
		Set16L(pBuffer,nPt,uint16_t(nValue));
		Set16L(pBuffer,nPt + 2,uint16_t(nValue >> 16));
		}

	//�T�v
	//	�l�̎擾(8,16,24,32bit�w��)
	//�p�����[�^
	//	pBuffer		�i�[��z��̐擪�v�f
	//	nPt			�i�[��z��̗v�f�ԍ�
	//�߂�l
	//	�w�肵���v�f�ԍ��̒l���߂�܂�
	static uint8_t Get8L(uint8_t* pBuffer,int32_t nPt)
		{
		return(pBuffer[nPt]);
		}
	static uint16_t Get16L(uint8_t* pBuffer,int32_t nPt)
		{
		uint16_t nResult = Get8L(pBuffer,nPt + 0) | (Get8L(pBuffer,nPt + 1) << 8);
		return(nResult);
		}
	static uint32_t Get24L(uint8_t* pBuffer,int32_t nPt)
		{
		uint32_t nResult = Get8L(pBuffer,nPt + 0) | (Get8L(pBuffer,nPt + 1) << 8) | (Get8L(pBuffer,nPt + 2) << 16);
		return(nResult);
		}
	static uint32_t Get32L(uint8_t* pBuffer,int32_t nPt)
		{
		uint32_t nResult = Get16L(pBuffer,nPt + 0) | (Get16L(pBuffer,nPt + 2) << 16);
		return(nResult);
		}

	//�T�v
	//	�V���O���p�P�b�g(DP0)�̍\�z
	//�p�����[�^
	//	nAYcmd		A/Y�w��
	//	nBXcmd		B/X�w��
	//	nBITcmd		����(bit�Ή�)
	//�߂�l
	//	�\�z���ꂽ�V���O���p�P�b�g(DP0)�ւ̃|�C���^���߂�܂�
	//���ӓ_
	//	�߂�l�͌Ăяo�����ŊJ������K�v���L��܂�
	static uint8_t* CreateSGL0(uint16_t nAYcmd,uint16_t nBXcmd,uint32_t nBITcmd)
		{
		uint8_t* pBuffer = CreateBuffer(0x00);
		Set16L(pBuffer,0,nAYcmd);
		Set16L(pBuffer,2,nBXcmd);
		Set32L(pBuffer,4,nBITcmd);
		return(pBuffer);
		}

	//�T�v
	//	�V���O���p�P�b�g(DP1)�̍\�z
	//�p�����[�^
	//	����
	//�߂�l
	//	�\�z���ꂽ�V���O���p�P�b�g(DP1)�ւ̃|�C���^���߂�܂�
	//���ӓ_
	//	�߂�l�͌Ăяo�����ŊJ������K�v���L��܂�
	static uint8_t* CreateSGL1(void)
		{
		uint8_t* pBuffer = CreateBuffer(0x00);
		return(pBuffer);
		}

	//�T�v
	//	�u���[�h�L���X�g�p�P�b�g�̍\�z
	//�p�����[�^
	//	nSign		�f�[�^�擪3�o�C�g�ɓ���Œ�R�[�h(0x00ffxx)
	//�߂�l
	//	�\�z���ꂽ�u���[�h�L���X�g�p�P�b�g�ւ̃|�C���^���߂�܂�
	//���ӓ_
	//	�߂�l�͌Ăяo�����ŊJ������K�v���L��܂�
	static uint8_t* CreateBRD(uint32_t nSign)
		{
		uint8_t* pBuffer = CreateBuffer(0x00);
		Set24L(pBuffer,0,nSign);
		return(pBuffer);
		}

	//�T�v
	//	�}���`�p�P�b�g(CM.RTS)�̍\�z
	//�p�����[�^
	//	nDataSize		�f�[�^�T�C�Y
	//	nMaxResponce	�ő�ԓ���
	//�߂�l
	//	�\�z���ꂽ�}���`�p�P�b�g(CM_RTS)�ւ̃|�C���^���߂�܂�
	//���ӓ_
	//	�߂�l�͌Ăяo�����ŊJ������K�v���L��܂�
	static uint8_t* CreateCMRTS(uint32_t nDataSize,uint8_t nMaxResponce = 0x08)
		{
		uint8_t* pBuffer = CreateBuffer(0xff);
		uint8_t nMsgCount = datasize2msgcount(nDataSize);
		uint16_t nMsgSize = nMsgCount * 8;
		Set8L(pBuffer,0,DEF_RTS);
		Set16L(pBuffer,1,nDataSize);
		Set8L(pBuffer,3,nMsgCount);
		Set8L(pBuffer,4,nMaxResponce);
		Set24L(pBuffer,5,DEF_SIGN);
		return(pBuffer);
		}

	//�T�v
	//	�}���`�p�P�b�g(CM_CTS)�̍\�z
	//�p�����[�^
	//	nMaxResponce	�ő�ԓ���
	//	nNextPacketNum	���p�P�b�g�ԍ�
	//�߂�l
	//	�\�z���ꂽ�}���`�p�P�b�g(CM_CTS)�ւ̃|�C���^���߂�܂�
	//���ӓ_
	//	�߂�l�͌Ăяo�����ŊJ������K�v���L��܂�
	static uint8_t* CreateCMCTS(uint8_t nMaxResponce,uint8_t nNextPacketNum)
		{
		uint8_t* pBuffer = CreateBuffer(0xff);
		Set8L(pBuffer,0,DEF_CTS);
		Set8L(pBuffer,1,nMaxResponce);
		Set8L(pBuffer,2,nNextPacketNum);
		Set24L(pBuffer,5,DEF_SIGN);
		return(pBuffer);
		}

	//�T�v
	//	�}���`�p�P�b�g(CM_DT)�̍\�z
	//�p�����[�^
	//	pData			���M�f�[�^�̐擪
	//	nDataSize		�f�[�^�T�C�Y
	//	nPacketNum		�p�P�b�g�ԍ�
	//�߂�l
	//	�\�z���ꂽ�}���`�p�P�b�g(CM_DT)�ւ̃|�C���^���߂�܂�
	//���ӓ_
	//	�߂�l�͌Ăяo�����ŊJ������K�v���L��܂�
	static uint8_t* CreateCMDT(uint8_t* pData,uint32_t nDataSize,uint8_t nPacketNum)
		{
		//�f�[�^�̑��M�Ώۈʒu���Z�o���A�f�[�^�͈͓����`�F�b�N����
		uint32_t nDataPt = (nPacketNum - 1) * 7;
		if(nDataPt + 1 > nDataSize)
			return(NULL);	//�I�[�o�[����
		//
		uint8_t* pBuffer = CreateBuffer(0xff);
		Set8L(pBuffer,0,nPacketNum);
		for(uint8_t nLoop = 0;nLoop < 7;nLoop++)
			{
			if(nDataPt + nLoop >= nDataSize)
				break;								//�I�[����
			else
				Set8L(pBuffer,1 + nLoop,pData[nDataPt + nLoop]);
			}
		return(pBuffer);
		}

	//�T�v
	//	�}���`�p�P�b�g(CM_EOMA)�̍\�z
	//�p�����[�^
	//	nMsgSize		���b�Z�[�W�T�C�Y
	//	nPacketCount	�p�P�b�g��
	//�߂�l
	//	�\�z���ꂽ�}���`�p�P�b�g(CM_EOMA)�ւ̃|�C���^���߂�܂�
	//���ӓ_
	//	�߂�l�͌Ăяo�����ŊJ������K�v���L��܂�
	static uint8_t* CreateCMEOMA(uint16_t nMsgSize,uint8_t nPacketCount)
		{
		uint8_t* pBuffer = CreateBuffer(0xff);
		Set8L(pBuffer,0,DEF_EOMA);
		Set16L(pBuffer,1,nMsgSize);
		Set8L(pBuffer,3,nPacketCount);
		Set24L(pBuffer,5,DEF_SIGN);
		return(pBuffer);
		}

	//�T�v
	//	�}���`�p�P�b�g(CM_ABORT)�̍\�z
	//�p�����[�^
	//	nCause			���~���R
	//�߂�l
	//	�\�z���ꂽ�}���`�p�P�b�g(CM_ABORT)�ւ̃|�C���^���߂�܂�
	//���ӓ_
	//	�߂�l�͌Ăяo�����ŊJ������K�v���L��܂�
	static uint8_t* CreateCMABORT(uint8_t nCause)
		{
		uint8_t* pBuffer = CreateBuffer(0xff);
		Set8L(pBuffer,0,DEF_ABORT);
		Set8L(pBuffer,1,nCause);
		Set24L(pBuffer,5,DEF_SIGN);
		return(pBuffer);
		}

	//�T�v
	//	�}���`�p�P�b�g(CM_BAM)�̍\�z
	//�p�����[�^
	//	nMsgSize		���b�Z�[�W�T�C�Y
	//	nPacketCount	�p�P�b�g��
	//�߂�l
	//	�\�z���ꂽ�}���`�p�P�b�g(CM_BAM)�ւ̃|�C���^���߂�܂�
	//���ӓ_
	//	�߂�l�͌Ăяo�����ŊJ������K�v���L��܂�
	static uint8_t* CreateCMBAM(uint16_t nMsgSize,uint8_t nPacketCount)
		{
		uint8_t* pBuffer = CreateBuffer(0xff);
		Set8L(pBuffer,0,DEF_BAM);
		Set16L(pBuffer,1,nMsgSize);
		Set8L(pBuffer,3,nPacketCount);
		Set24L(pBuffer,5,DEF_SIGN);
		return(pBuffer);
		}

	//�T�v
	//	�f�[�^�T�C�Y���瑗�M�񐔂ɕϊ�
	//�p�����[�^
	//	nDatasize		�f�[�^�T�C�Y
	//�߂�l
	//	�f�[�^�𑗐M����̂ɕK�v�ȃp�P�b�g��
	static uint8_t datasize2msgcount(uint32_t nDatasize)
		{
		if(nDatasize > 7 * 0xff)
			return(0);	//1[byte]�ɓ���Ȃ����M��
		uint8_t nResult = nDatasize / 7;
		if(nDatasize % 7)
			++nResult;
		return(nResult);
		}

	//�T�v
	//	�p�P�b�g�ԍ�����f�[�^�i�[�ʒu���Z�o
	//�p�����[�^
	//	nPacket			�p�P�b�g�ԍ�
	//�߂�l
	//	���̃p�P�b�g�ɑΉ�����f�[�^�ʒu
	static uint32_t packetnum2datapt(uint8_t nPacket)
		{
		uint32_t nResult = (nPacket - 1) * 7;
		return(nResult);
		}

	//�T�v
	//	�p�P�b�g��ʂ��擾
	//�p�����[�^
	//	pData			�p�P�b�g�擪�ւ̃|�C���^
	//�߂�l
	//	DEF_UNKNOWN		�s���ȃp�P�b�g
	//	DEF_*			�Y������p�P�b�g���
	static uint8_t IsPacket(uint8_t* pData)
		{
		uint8_t nResult = pData[0];
		switch(nResult)
			{
			case DEF_RTS:
			case DEF_CTS:
			case DEF_EOMA:
			case DEF_ABORT:
				break;
			default:
				nResult = DEF_UNKNOWN;
				break;
			}
		return(nResult);
		}

public:
	CCan1939()
		{}
	virtual ~CCan1939()
		{}
};
