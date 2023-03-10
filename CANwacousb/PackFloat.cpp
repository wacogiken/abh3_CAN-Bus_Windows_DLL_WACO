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

#include "pch.h"
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "PackFloat.h"

//
#pragma warning(disable : 28182)

//==========================================================================================================
//�l��ݒ肷��
//==========================================================================================================

//�l��ݒ肷��(������)
int CPackFloat::SetValueA(char* pStr)
	{
	//����
	//	pStr		������ւ̃|�C���^
	//
	//�߂�l
	//	0�ȊO�ŃG���[

	//
	int32_t x_kasuu = 0;
	int32_t x_shisuu = 0;
	int32_t x_shousuutenn = 0;
	int32_t dp = 0;
	int err = 0;
	int32_t x = 0;

	if(pStr == NULL)
		err = 1;
	else if(*pStr == NULL)
		err = 1;
	else if((pStr = flA(pStr,&x,&dp)) == NULL)		//���l�擾
		err = 1;								//ERROR
	else
		{
		x_kasuu = x;
		x = 0;
		if(*pStr == 'e' || *pStr == 'E')		// �w���擾
			{
			++pStr;
			if((pStr = flA(pStr,&x,NULL)) == NULL)
				err = 2;						//ERROR:
			}
		if(dp < -8)							//�����ȉ�8���𒴉߂���ꍇ
			{
			x += dp - (-5);						//�w�����ɂ͂ݏo��
			dp = -5;							//�������𒲐�
			}
		else if(dp > 7)
			{
			x += dp - (-5);						//�w�����ɂ͂ݏo��
			dp = -5;							//�������𒲐�
			}
		if(*pStr)
			err = 5;							//ERROR:���͂���������Ɏc�肪�L��(�{���̒l���\���s��)
		else if(x < -64 || x > 63)				// �w���͈̔̓`�F�b�N
			err = 7;
		x_shisuu = x;
		x_shousuutenn = dp;
		}
	//
	if(!err)
		{
		this->u.info.kasuu = x_kasuu;
		this->u.info.shisuu = x_shisuu;
		this->u.info.shousuutenn = x_shousuutenn;
		}
	return err;
	}
int CPackFloat::SetValueW(wchar_t* pStr)
	{
	//����
	//	pStr		������ւ̃|�C���^
	//
	//�߂�l
	//	0�ȊO�ŃG���[
	//
	int32_t x_kasuu = 0;
	int32_t x_shisuu = 0;
	int32_t x_shousuutenn = 0;
	int32_t dp = 0;
	int err = 0;
	int32_t x = 0;

	if(pStr == NULL)
		err = 1;
	else if(*pStr == NULL)
		err = 1;
	else if((pStr = flW(pStr,&x,&dp)) == NULL)		//���l�擾
		err = 1;								//ERROR
	else
		{
		x_kasuu = x;
		x = 0;
		if(*pStr == L'e' || *pStr == L'E')		// �w���擾
			{
			++pStr;
			if((pStr = flW(pStr,&x,NULL)) == NULL)
				err = 2;						//ERROR:
			}
		if(dp < -8)							//�����ȉ�8���𒴉߂���ꍇ
			{
			x += dp - (-5);						//�w�����ɂ͂ݏo��
			dp = -5;							//�������𒲐�
			}
		else if(dp > 7)
			{
			x += dp - (-5);						//�w�����ɂ͂ݏo��
			dp = -5;							//�������𒲐�
			}
		if(*pStr)
			err = 5;							//ERROR:���͂���������Ɏc�肪�L��(�{���̒l���\���s��)
		else if(x < -64 || x > 63)				// �w���͈̔̓`�F�b�N
			err = 7;
		x_shisuu = x;
		x_shousuutenn = dp;
		}
	//
	if(!err)
		{
		this->u.info.kasuu = x_kasuu;
		this->u.info.shisuu = x_shisuu;
		this->u.info.shousuutenn = x_shousuutenn;
		}
	return err;
	}

//�l��ݒ肷��(Direct_value)
void CPackFloat::SetDirectValue(int32_t nValue)
	{
	this->u.nDirectData = nValue;
	}

//�l��ݒ肷��(PACK_FLOAT)
void CPackFloat::SetValue(PACK_FLOAT pack)
	{
	this->u.info.kasuu = pack.u.info.kasuu;
	this->u.info.shisuu = pack.u.info.shisuu;
	this->u.info.shousuutenn = pack.u.info.shousuutenn;
	}

//�l��ݒ肷��(float)
void CPackFloat::SetValue(float nValue)
	{
	int32_t x_kasuu = 0;
	int32_t x_shisuu = 0;
	int32_t x_shousuutenn = 0;
	int e = 0;
	int sign = 0;

	if(nValue < 0)
		{
		nValue = -nValue;
		sign = 1;
		}
	if(nValue == 0.0F)
		{
		x_shousuutenn = 0;
		x_shisuu = 0;
		x_kasuu = 0;
		}
	else
		{
		while(nValue > 1000000.0F)
			{
			nValue /= 10.0F;
			++e;
			}
		while(nValue < 100000.0F)
			{
			nValue *= 10.0F;
			--e;
			}
		//�[���T�v���X����
		int i = 0;
		while(((long)nValue % 10) == 0)
			{
			nValue /= 10;
			i++;
			}

		if((-9 <= e) && (e <= 0))
			{
			x_shousuutenn = e + i;
			x_shisuu = 0;
			}
		else
			{
			x_shousuutenn = 0;
			x_shisuu = e + i;
			}
		x_kasuu = (int32_t)(sign ? -nValue : nValue);
		}
	this->u.info.kasuu = x_kasuu;
	this->u.info.shisuu = x_shisuu;
	this->u.info.shousuutenn = x_shousuutenn;
	}

//==========================================================================================================
//�l���擾����
//==========================================================================================================

//�l���擾����(������)
void CPackFloat::GetValueA(char* pStr,uint32_t nMaxLength)
	{
	memset(pStr,0,(size_t)nMaxLength);

	char tmp[8],* q;
	char* p = pStr;
	int32_t x = this->u.info.kasuu;
	int32_t dp = this->u.info.shousuutenn;
	int32_t e = this->u.info.shisuu;
	int n = 0;

	if(x < 0)
		{
		*p++ = '-';
		x = -x;
		}
	if(dp >= 0)					// xxxx0000
		{
		p = cvA(x,p);
		while(dp--)
			*p++ = '0';
		}
	else
		{
		*cvA(x,tmp) = '\0';
		n = (int)strlen(tmp);
		if(n > -dp)				// xxx.xxx
			{
			for(q = tmp; *q; ++q)
				{
				if(n == -(dp--))
					*p++ = '.';
				*p++ = *q;
				}
			}
		else						// 0.00xxx
			{
			*p++ = '0';
			*p++ = '.';
			while(n <= -(++dp))
				*p++ = '0';
			for(q = tmp; *q; ++q)
				*p++ = *q;
			}
		}
	if(e)							// �w���� */
		{
		*p++ = 'e';
		if(e < 0)
			{
			*p++ = '-';
			e = -e;
			}
		p = cvA(e,p);
		}
	*p = '\0';
	}
void CPackFloat::GetValueW(wchar_t* pStr,uint32_t nMaxLength)
	{
	memset(pStr,0,(size_t)nMaxLength * sizeof(wchar_t));

	wchar_t tmp[8],* q;
	wchar_t* p = pStr;
	int32_t x = this->u.info.kasuu;
	int32_t dp = this->u.info.shousuutenn;
	int32_t e = this->u.info.shisuu;
	int n = 0;

	if(x < 0)
		{
		*p++ = L'-';
		x = -x;
		}
	if(dp >= 0)					// xxxx0000
		{
		p = cvW(x,p);
		while(dp--)
			*p++ = '0';
		}
	else
		{
		*cvW(x,tmp) = '\0';
		n = (int)wcslen(tmp);
		if(n > -dp)				// xxx.xxx
			{
			for(q = tmp; *q; ++q)
				{
				if(n == -(dp--))
					*p++ = L'.';
				*p++ = *q;
				}
			}
		else						// 0.00xxx
			{
			*p++ = L'0';
			*p++ = L'.';
			while(n <= -(++dp))
				*p++ = L'0';
			for(q = tmp; *q; ++q)
				*p++ = *q;
			}
		}
	if(e)							// �w���� */
		{
		*p++ = L'e';
		if(e < 0)
			{
			*p++ = L'-';
			e = -e;
			}
		p = cvW(e,p);
		}
	*p = '\0';
	}

//�l���擾����(Direct_value)
int32_t CPackFloat::GetDirectValue()
	{
	return((int32_t)this->u.nDirectData);
	}

//�l���擾����(PACK_FLOAT)
void CPackFloat::GetValue(pPACK_FLOAT pPack)
	{
	pPack->u.info.kasuu = this->u.info.kasuu;
	pPack->u.info.shisuu = this->u.info.shisuu;
	pPack->u.info.shousuutenn = this->u.info.shousuutenn;
	}

//�l���擾����(float)
void CPackFloat::GetValue(float& nValue)
	{
	int32_t n = this->u.info.shisuu + this->u.info.shousuutenn;
	nValue = (float)this->u.info.kasuu;
	if(n > 0)
		{
		while(n--)
			nValue *= 10.0F;
		}
	else if(n < 0)
		{
		while(n++)
			nValue /= 10.0F;
		}
	}


//==========================================================================================================
//�����p
//==========================================================================================================

// 10�i������𐮐��ɕϊ�
char* CPackFloat::flA(char* str,int32_t* rx,int* rdp)
	{
	if(str == NULL)
		return(NULL);

	int dp = 0,end = 0,err = 0,sign = 0;
	long x = 0;

	if(*str == '+')
		{
		++str;
		}
	else if(*str == '-')
		{
		sign = 1;
		++str;
		}
	while(*str)
		{
		switch(*str)
			{
			case '.':
				if(rdp == NULL || dp)
					{
					end = err = 6;
					}
				dp = -1;
				break;
			default:
				if(isdigit(*str))
					{
					if(x < 100000)
						{
						x = x * 10 + *str - '0';
						if(dp < 0)
							{
							--dp;
							}
						}
					else if(dp >= 0)
						{
						++dp;
						}
					}
				else
					{
					end = 4;
					}
				break;
			}
		if(end)
			break;
		++str;
		}
	if(rdp)
		{
		*rdp = (dp < 0) ? dp + 1 : dp;
		}
	*rx = sign ? -x : x;				//���ʂ̕���
	return str;							//����I��
	}
wchar_t* CPackFloat::flW(wchar_t* str,int32_t* rx,int* rdp)
	{
	if(str == NULL)
		return(NULL);					//������NULL

	int dp = 0,end = 0,err = 0,sign = 0;
	long x = 0;

	//����
	if(*str == L'+')
		{
		++str;
		}
	else if(*str == L'-')
		{
		sign = 1;
		++str;
		}
	while(*str)
		{
		switch(*str)
			{
			//�����_
			case '.':
				if(rdp == NULL || dp)
					{
					end = err = 6;
					}
				dp = -1;
				break;
			default:
				if(iswdigit(*str))
					{
					if(x < 100000)
						{
						x = x * 10 + *str - '0';
						if(dp < 0)
							{
							--dp;
							}
						}
					else if(dp >= 0)
						{
						++dp;
						}
					}
				else
					{
					end = 4;				//�s���ȕ����ŏI��
					}
				break;
			}
		if(end)
			break;
		++str;
		}
	if(rdp)
		{
		*rdp = (dp < 0) ? dp + 1 : dp;	//�}�C�i�X�̂Ƃ��͂P�����̂ŕ␳
		}
	*rx = sign ? -x : x;				//���ʂ̕���
	return str;							//����I��
	}

// ������ɕϊ�
char* CPackFloat::cvA(int32_t x,char* p)
	{
	ldiv_t dt;
	dt = ldiv(x,10);
	if(dt.quot)
		p = cvA(dt.quot,p);
	*p++ = (char)dt.rem + '0';
	return p;
	}
wchar_t* CPackFloat::cvW(int32_t x,wchar_t* p)
	{
	ldiv_t dt;
	dt = ldiv(x,10);
	if(dt.quot)
		p = cvW(dt.quot,p);
	*p++ = (wchar_t)dt.rem + L'0';
	return p;
	}
