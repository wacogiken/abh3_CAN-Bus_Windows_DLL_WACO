#pragma once

#include "pch.h"
//#include <afxwin.h>
#include <setupapi.h>
#include <winioctl.h>
#include <wchar.h>
#include <shlobj.h>

class CEnumComPortEx
{
//================================================================================
//���[�U�[���g�p�\�ȗv�f
//================================================================================

public:
	//��`
	typedef enum
		{
		//
		 MAX_PORT	= 256
		,MAX_STRLEN	= 256
		
		//�G���[�֘A
		,ERR_NOERROR	= 0
		,ERR_ERROR		= 1		//�ڍוs���ȃG���[�A���͏ڍׂ�m�点��K�v�������G���[
		,ERR_SYSTEM		= 2		//�����G���[�i�@�\���g���Ȃ��ꍇ���j


		} ENUM;

	//
	typedef struct ECPEX_INFO
		{
		bool	bAlive;
		PTCHAR	pName;
		} ECPEX_INFO,*pECPEX_INFO;

protected:
	//
	typedef struct _ECPEX_VAR
		{
		HANDLE		hSema;
		ECPEX_INFO	info[MAX_PORT];
		} ECPEX_VAR,*pECPEX_VAR;
	ECPEX_VAR m_var;

public:
	//COM�|�[�g�����擾���Ċi�[����
	DWORD UpdateInformation(void);

	//�i�[����Ă�����ւ̃|�C���^���擾����
	pECPEX_INFO GetPortInfo(int nCOMport)
		{
		if((nCOMport < 1) || (nCOMport >= MAX_PORT))
			return(NULL);
		else if(!m_var.info[nCOMport].bAlive)
			return(NULL);
		return(&m_var.info[nCOMport]);
		}

	//COM�|�[�g�ԍ����w��l���猟��
	int GetFirstPort(int nBeginPort = 1)
		{
		//�T�v
		//	�V�X�e���ɓ��ڂ��ꂽCOM�|�[�g�̓��A�w��ԍ����͂�����傫���|�[�g�ԍ����擾
		//�p�����[�^
		//	nBeginPort	�������J�n����COM�|�[�g�ԍ�
		//�߂�l
		//	1�ȏ�		COM�|�[�g�ԍ�
		//	0			COM�|�[�g�����݂��Ȃ�

		for(int nCOMport = nBeginPort;nCOMport < MAX_PORT;nCOMport++)
			{
			pECPEX_INFO pInfo = GetPortInfo(nCOMport);
			if(pInfo)
				return(nCOMport);
			}
		return(0);
		}

	//COM�|�[�g�ԍ����w��l���傫����������
	int GetNextPort(int& nCOMport)
		{
		//�T�v
		//	�V�X�e���ɓ��ڂ��ꂽCOM�|�[�g�̓��A�w��ԍ����傫���|�[�g�ԍ����擾
		//�p�����[�^
		//	nCOMport	COM�|�[�g�ԍ��i������傫���l�������ΏۂƂȂ�j
		//�߂�l
		//	1�ȏ�		���ɔ�������COM�|�[�g�ԍ�
		//	0			�w��l���傫��COM�|�[�g�����݂��Ȃ�

		nCOMport = GetFirstPort(nCOMport + 1);
		return(nCOMport);
		}


//================================================================================
//�����p
//================================================================================
private:

	//������L
	bool Lock(DWORD nTimeoutMS = INFINITE)
		{
		if(::WaitForSingleObject(m_var.hSema,nTimeoutMS) != WAIT_OBJECT_0)
			return(false);
		return(true);
		}

	//�����J��
	void Unlock(void)
		{
		::ReleaseSemaphore(m_var.hSema,1,NULL);
		}

	//����������
	void Clear(void)
		{
		if(Lock())
			{
			for(int nCOMport = 1;nCOMport < MAX_PORT;nCOMport++)
				{
				m_var.info[nCOMport].bAlive = false;
				if(m_var.info[nCOMport].pName)
					{
					delete m_var.info[nCOMport].pName;
					m_var.info[nCOMport].pName = NULL;
					}
				}
			Unlock();
			}
		}

public:
	//�R���X�g���N�^
	CEnumComPortEx();
	//�f�X�g���N�^
	virtual ~CEnumComPortEx();

};
