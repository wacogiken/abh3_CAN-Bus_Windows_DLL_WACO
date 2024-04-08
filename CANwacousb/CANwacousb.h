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

#include <Windows.h>
#include <stdint.h>
#include "typedef.h"
#include "ABH3.h"
#include "PackFloat.h"
#include "WacoCanUsb.h"

//============================================================
//�G�N�X�|�[�g�H
//============================================================

 #ifndef _NO_CANABH3_EXPORT
  #define CANABH3API __declspec(dllexport)
#else
  #define CANABH3API __declspec(dllimport)
#endif

//============================================================
//�ȉ������P�[�W��C����`���Ƃ���
//============================================================

#ifdef __cplusplus
 extern "C" {
#endif //__cplusplus

//============================================================
//���[�U�[�p�֐�(Windows�ŗL�n)
//============================================================

//�g�p�J�n���ɌĂяo���K�v���L��܂�
CANABH3API void InitInstance(int32_t nIFnum);

//�g�p�I�����ɌĂяo���K�v���L��܂�
CANABH3API void ExitInstance(void);

//�g�p�\��CAN�C���^�[�t�F�[�X�����擾
CANABH3API int32_t GetInterfaceCount(void);

//�C���^�[�t�F�[�X���w�肷��
CANABH3API int32_t SetInterface(int32_t nDeviceNum);

//���݊J���Ă���C���^�[�t�F�[�X�ԍ����擾
CANABH3API int32_t GetCurrentInterface(void);

//���݃C���^�[�t�F�[�X���J���Ă��邩�H
CANABH3API int32_t IsOpenInterface(void);

//�C���^�[�t�F�[�X���J���^�C���A�E�g���Ԃ�ݒ�
CANABH3API void SetOpenTimeout(uint32_t nTimeoutMS);

//���M�^�C���A�E�g���Ԃ�ݒ�
CANABH3API void SetSendTimeout(uint32_t nTimeoutMS);

//��M�^�C���A�E�g���Ԃ�ݒ�
CANABH3API void SetRecvTimeout(uint32_t nTimeoutMS);

//�ʐM�z�X�g�̃A�h���X��ݒ�
CANABH3API void SetHostID(uint8_t nAdrs);

//�ʐM�z�X�g�̃A�h���X���擾
CANABH3API uint8_t GetHostID(void);

//�ʐM���x���w��
CANABH3API void SetBaudrate(uint32_t nBaudrateKbps);

//�ݒ肵���ʐM���x���擾
CANABH3API uint32_t GetBaudrate(void);

//PC���N�����Ă���̎��Ԃ�[ms]�P�ʂŎ擾
CANABH3API uint32_t GetTm(void);

//1�b�ԂɎ�M�����f�[�^�ʂ��擾���܂�
CANABH3API uint32_t GetCounter(void);

//����M�J�E���^���擾���܂�
CANABH3API uint32_t GetTransmitCounter(uint8_t nRecv);

//CAN�o�X�ɃG���[���������Ă��邩���ׂ܂�
CANABH3API uint32_t GetCANerror(void);

//CAN�o�X�̃G���[�������s���܂�
CANABH3API uint32_t ResetCANerror(void);

//================================================================================
//���[�U�[�p�֐�(�ʐM�n)
//================================================================================

//�w�߂̏������{CAN�|�[�g���J��
CANABH3API int32_t abh3_can_init(uint8_t nTargetID,pCANABH3_RESULT pPtr);

//CAN�|�[�g���J��
CANABH3API int32_t abh3_can_port_init(void);

//�w�߂̏�����
CANABH3API int32_t abh3_cmd_init(uint8_t nTargetID,pCANABH3_RESULT pPtr);

//�w�߂̑��M�i���ʁj
CANABH3API int32_t abh3_can_cmdAY(uint8_t nTargetID,int16_t cmd,pCANABH3_RESULT pPtr);
CANABH3API int32_t abh3_can_cmdBX(uint8_t nTargetID,int16_t cmd,pCANABH3_RESULT pPtr);

//�w�߂̑��M�i�����j
CANABH3API int32_t abh3_can_cmd(uint8_t nTargetID,int16_t cmdAY,int16_t cmdBX,pCANABH3_RESULT pPtr);

//���͂̑��M�i�ꊇ�j
CANABH3API int32_t abh3_can_opSet(uint8_t nTargetID,int32_t data,int32_t mask,pCANABH3_RESULT pPtr);

//���͂̑��M�i�r�b�g�j
CANABH3API int32_t abh3_can_opBitSet(uint8_t nTargetID,int8_t num,int8_t data,pCANABH3_RESULT pPtr);

//�w�߂Ɠ��͂̑��M�i�����j
CANABH3API int32_t abh3_can_cmdAndopSet(uint8_t nTargetID,int16_t cmdAY,int16_t cmdBX,int32_t data,int32_t mask,pCANABH3_RESULT pPtr);

//�u���[�h�L���X�g�p�P�b�g�̃��N�G�X�g
CANABH3API int32_t abh3_can_reqBRD(uint8_t nTargetID,uint8_t num,uint8_t nBroadcast,pCANABH3_RESULT pPtr);

//�}���`�p�P�b�g�ɂ��TelABH3�p�P�b�g�̑���M
CANABH3API int32_t abh3_can_trans(uint8_t nTargetID,char* sbuf,char* rbuf,uint32_t* rbuflen);

//���݂̎�M�f�[�^�̈�̕������擾
CANABH3API void abh3_can_copylastdata(uint8_t nTargetID,pCANABH3_LASTDATA pPtr);

//��M�f�[�^�̈�̍X�V�t���O������
CANABH3API void abh3_can_resetlastdata(uint8_t nTargetID,int32_t nAdrs);

//�w���ނ̃p�P�b�g����M
CANABH3API int32_t abh3_can_read(uint8_t nTargetID,pCANABH3_RESULT pPtr,PACKETTYPE nType,uint8_t nAdrs);

//��M�o�b�t�@�̓��e����M�f�[�^�󋵂ɔ��f������
CANABH3API int32_t abh3_can_flush(void);

//��������
CANABH3API int32_t abh3_can_finish(void);

//================================================================================
//���[�U�[�p�֐�(�P�ʕϊ��n)
//================================================================================

//���x��������l�֕ϊ�
CANABH3API int16_t cnvVel2CAN(float vel);

//�����l���瑬�x�֕ϊ�
CANABH3API float cnvCAN2Vel(int16_t vel);

//�d���l��������l�ɕϊ�
CANABH3API int16_t cnvCur2CAN(float cur);

//�����l����d���l�ɕϊ�
CANABH3API float cnvCAN2Cur(int16_t cur);

//�����l���畉�ח��ɕϊ�
CANABH3API float cnvCAN2Load(int16_t load);

//�����l����A�i���O���͂ɕϊ�
CANABH3API float cnvCAN2Analog(int16_t analog);

//�����l����d���d���ɕϊ�
CANABH3API float cnvCAN2Volt(int16_t volt);

//��������PACK_FLOAT�֕ϊ�
CANABH3API PACK_FLOAT flt_to_pkflt(float flt);

//PACK_FLOAT��������֕ϊ�
CANABH3API float pkflt_to_flt(PACK_FLOAT okflt);

//ANSI�����񂩂�PACK_FLOAT�֕ϊ�
CANABH3API int str_to_pkfltA(const char* str,PACK_FLOAT* p_pkflt);

//UNICODE�����񂩂�PACK_FLOAT�֕ϊ�
CANABH3API int str_to_pkfltW(const wchar_t* str,PACK_FLOAT* p_pkflt);

//PACK_FLOAT����ANSI������ɕϊ�
CANABH3API char* pkflt_to_strA(PACK_FLOAT pkflt,char* str);

//PACK_FLOAT����UNICODE������ɕϊ�
CANABH3API wchar_t* pkflt_to_strW(PACK_FLOAT pkflt,wchar_t* str);

//================================================================================
//���[�U�[�p�֐�(���O�n)
//================================================================================

//���O�@�\�������y�у��O�t�H���_�ݒ�
CANABH3API int32_t canLogInit(uint8_t nUtf16,void* pLogFolder);

//���O�@�\����
CANABH3API int32_t canLogCtrl(uint8_t nCmd);



//============================================================
//�����P�[�W����
//============================================================
#ifdef __cplusplus
 }
#endif //__cplusplus
