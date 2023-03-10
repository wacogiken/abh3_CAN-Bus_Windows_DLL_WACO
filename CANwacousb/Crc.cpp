/*
 ******************************************************************************
 * Software License Agreement (BSD License)
 *
 *  Copyright (c) 2023, Waco Giken Co., Ltd.
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

//
#include "pch.h"
#include <limits.h>
#include "Crc.h"

//コンストラクタ
CCrc16::CCrc16(USHORT nKey,bool bReverse /* false */,bool bLeft /* true */)
	{
	::ZeroMemory(&m_var,sizeof(CCRC16_VAR));
	if(bReverse)
		nKey = ReverseKey(nKey);
	m_var.nKey = nKey;
	m_var.bReverse = bReverse;
	m_var.bLeft = bLeft;
	CreateCRCtable();
	}

//デストラクタ
CCrc16::~CCrc16()
	{
	}

//CRC計算
USHORT CCrc16::CalcCRC(USHORT nCurrentCRC,PBYTE pData,int nLength /* 1 */)
	{
	while(nLength--)
		{
		if(m_var.bLeft)
			{
			USHORT nTmpCRC = (nCurrentCRC << CHAR_BIT) ^ m_var.table[(BYTE)(nCurrentCRC >> (16 - CHAR_BIT)) ^ *pData];
			nCurrentCRC = nTmpCRC;
			++pData;
			}
		else
			{
			nCurrentCRC = (nCurrentCRC >> CHAR_BIT) ^ m_var.table[(BYTE)nCurrentCRC ^ *pData];
			++pData;
			}
		}
	return(~nCurrentCRC & 0xffff);
	}

//CRC計算テーブル構築
void CCrc16::CreateCRCtable()
	{
	ULONG nCrc = 0;
	for(int nLoop = 0; nLoop <= UCHAR_MAX;nLoop++)
		{
		if(m_var.bLeft)
			{
			//左回り
			nCrc = nLoop << (16 - CHAR_BIT);
			for (int nBit = 0;nBit < CHAR_BIT;nBit++)
				{
				if (nCrc & 0x8000U)
					nCrc = (nCrc << 1) ^ m_var.nKey;
				else
					nCrc <<= 1;
				}
			}
		else
			{
			//右回り
			nCrc = nLoop;
			for (int nBit = 0;nBit < CHAR_BIT;nBit++)
				{
				if (nCrc & 1)
					nCrc = (nCrc >> 1) ^ m_var.nKey;
				else
					nCrc >>= 1;
				}
			}
		m_var.table[nLoop] = (USHORT)nCrc;
		}
	}

//ビットの左右反転
USHORT CCrc16::ReverseKey(USHORT nKey)
	{
	USHORT nResult = 0;
	int nLoopCount = (int)(sizeof(USHORT) * CHAR_BIT);
	for(int nLoop = 0;nLoop < nLoopCount;nLoop++)
		{
		nResult <<= 1;
		if(nKey & 1)
			nResult |= 1;
		nKey >>= 1;
		}
	return(nResult);
	}


