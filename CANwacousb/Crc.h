#pragma once

#include "pch.h"

//
typedef struct _CCRC16_VAR
	{
	USHORT	nKey;
	bool	bReverse;
	bool	bLeft;
	USHORT	table[UCHAR_MAX + 1];
	} CCRC16_VAR,*pCCRC16_VAR;	

//
class CCrc16
{
public:
	CCrc16(USHORT nKey,bool bReverse = false,bool bLeft = true);
	virtual ~CCrc16();

protected:
	CCRC16_VAR m_var;
	void CreateCRCtable(void);
	USHORT ReverseKey(USHORT nKey);

public:
	USHORT CalcCRC(USHORT nCurrentCRC,PBYTE pData,int nLength = 1);
};
