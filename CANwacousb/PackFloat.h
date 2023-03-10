/*
 ******************************************************************************
 * Software License Agreement (BSD License)
 *
 *  Copyright (c) 2021-2023,2022 Waco Giken Co., Ltd.
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

#ifndef _PACK_FLOAT_H_INCLUDED_
#define _PACK_FLOAT_H_INCLUDED_

#include <stdint.h>
#include "typedef.h"

/*
// �^��`
typedef struct _PACK_FLOAT
	{
	union
		{
		struct _PACK_FLOAT_INFO
			{
			//LSB������L��(�����n�ɒ���)
			int32_t kasuu:21;			// ������(-999999�`0�`99999)
			int32_t shousuutenn:4;	// �����_(-8�`0�`7)
			int32_t shisuu:7;			// �w����(-64�`0�`63)
			//�����炪MSB��
			} info;
		int32_t	nDirectData;		// ���ڃA�N�Z�X�p
		} u;
	} *pPACK_FLOAT,PACK_FLOAT;
*/

//�N���X
class CPackFloat : public _PACK_FLOAT
{
public:
	//�R���X�g���N�^
	CPackFloat()
		{SetValue(0.0f);}
	CPackFloat(float nValue)
		{SetValue(nValue);}
	CPackFloat(double nValue)
		{SetValue(nValue);}
	CPackFloat(PACK_FLOAT pack)
		{SetValue(pack);}
	CPackFloat(char* pValue)
		{SetValueA(pValue);}
	CPackFloat(wchar_t* pValue)
		{SetValueW(pValue);}

	//�l��ݒ肷��
	int	SetValueA(char* pStr);								//�������l�Ƃ��Đݒ�
	int	SetValueW(wchar_t* pStr);							//�������l�Ƃ��Đݒ�
	void SetDirectValue(int32_t nValue);					//�����l�𒼐ڐݒ肷��
	void SetValue(PACK_FLOAT pack);							//����PACK_FLOAT�ϐ��Ɠ����l��ݒ�
	void SetValue(float nValue);							//float��l�Ƃ��Đݒ�
	void SetValue(double nValue)							//double��l�Ƃ��Đݒ�A�A�����x��float
		{SetValue((float)nValue);}

	//�l���擾����(������)
	void GetValueA(char* pStr,uint32_t nMaxLength);			//�l�𕶎���Ŏ擾
	void GetValueW(wchar_t* pStr,uint32_t nMaxLength);		//�l�𕶎���Ŏ擾
	void GetValue(pPACK_FLOAT pPack);						//�l��PACK_FLOAT�`���Ŏ擾
	void GetValue(float& nValue);							//�l��float�`���Ŏ擾
	void GetValue(double& nValue)							//�l��double�`���Ŏ擾�A�A�����x��float
		{float nTmp;GetValue(nTmp);nValue = (double)nTmp;}
	int32_t GetDirectValue(void);							//�����l�𒼐ڎ擾����

private:
	// 10�i������𐮐��ɕϊ�
	static char* flA(char* str, int32_t* rx, int* rdp);
	static wchar_t* flW(wchar_t* str, int32_t* rx, int* rdp);

	// ������ɕϊ�
	static char* cvA(int32_t x, char* p);
	static wchar_t* cvW(int32_t x, wchar_t* p);
};
			 
#endif
