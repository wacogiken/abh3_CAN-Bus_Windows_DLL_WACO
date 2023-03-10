//CEnumComPortExEx(COM�|�[�g��񋓂���N���X�E�f�o�C�X���t��)
// Copyright(C)2020 Y.OGAWA
//
//  Date		Auth		Ver		Comment
// -----------------------------------------------------------------
//	2020.04.16	Y.OGAWA		1.00	1st release
//									CEnumComPortEx���x�[�X�ɍ쐬
//
//�E�g�p�Ɋւ��鋖������
//�@�\�[�X�R�[�h�����쌠�\���̍폜�֎~
//
//�g�p���@
//	�^�[�Q�b�g�v���W�F�N�g�ɂ��̃t�@�C��(*.cpp/*.h)�̃����N��ǉ����Ďg�p
//
//	(1)�{�N���X��1�쐬����
//	(2)UpdateInformation�֐����Ăяo��
//	(3)GetPortInfo�֐���COM�|�[�g�ԍ����w�肵�ČĂяo���A�����擾����
//

#include <pch.h>
#include <tchar.h>
#include "EnumComPortEx.h"
#include "SafeRelease.inl"

//�������C�u���������N
#pragma comment(lib,"setupapi.lib")

//�R���X�g���N�^
CEnumComPortEx::CEnumComPortEx()
	{
	//�����ϐ��̏�����
	::ZeroMemory(&m_var,sizeof(ECPEX_VAR));
	//�Z�}�t�H�\�z
	m_var.hSema = ::CreateSemaphore(NULL,1,1,NULL);
	}

//�f�X�g���N�^
CEnumComPortEx::~CEnumComPortEx()
	{
	//�S�J��
	Clear();
	//�Z�}�t�H�J��
	::CloseHandle(m_var.hSema);

	}

//COM�|�[�g�����擾���Ċi�[����
DWORD CEnumComPortEx::UpdateInformation()
	{
	//�߂�l
	//	�񋓂��ꂽCOM�|�[�g�̐����߂�܂�
	//

	//�������i�����������b�N�ɒ��Ӂj
	Clear();

	//������L
	if(!Lock())
		return(ERR_ERROR);	//������L���s

	//�f�o�C�X���i�[��
	SP_DEVINFO_DATA DeviceInfoData;
	ZeroMemory(&DeviceInfoData,sizeof(SP_DEVINFO_DATA));
	DeviceInfoData.cbSize = sizeof(SP_DEVINFO_DATA);

	//�ꎞ�I�ȃo�b�t�@
	BYTE tmpBuffer[MAX_STRLEN * sizeof(TCHAR)];
	PTCHAR pTmpBuffer = (PTCHAR)&tmpBuffer;
	DWORD nValue = 0;

	//�f�o�C�X���������ׂ̃n���h�����擾
	HDEVINFO hDevInfo = SetupDiGetClassDevs(&GUID_DEVINTERFACE_COMPORT,NULL,NULL,(DIGCF_PRESENT | DIGCF_DEVICEINTERFACE));
	if(hDevInfo == INVALID_HANDLE_VALUE)
		return(ERR_SYSTEM);	//�擾���s�i�G���[���͑S�f�o�C�X�g�p�s�����j

	// �񋓂̏I���܂Ń��[�v
	DWORD nDeviceNum = 0;
	while(SetupDiEnumDeviceInfo(hDevInfo,nDeviceNum,&DeviceInfoData))
		{
		// �f�o�C�X�C���X�^���XID���擾
		SetupDiGetDeviceInstanceId(hDevInfo,&DeviceInfoData,(PTSTR)pTmpBuffer,sizeof(tmpBuffer),&nValue);
		//�f�o�C�XID�����Ƀf�o�C�X�̏����擾
		HKEY hKey = SetupDiOpenDevRegKey(hDevInfo,&DeviceInfoData,DICS_FLAG_GLOBAL,0,DIREG_DEV,KEY_QUERY_VALUE);
		if(hKey)
			{
			DWORD nType = 0;
			nValue = MAX_STRLEN;
			RegQueryValueEx(hKey,_T("PortName"),NULL,&nType,(LPBYTE)&tmpBuffer,&nValue);
			//�f�o�C�X���̍ŏ���"COM"���H
			if((pTmpBuffer[0] == 'C') && (pTmpBuffer[1] == 'O') && (pTmpBuffer[2] == 'M'))
				{
				//"COM"����n�܂�f�o�C�X��

				//COM�|�[�g�ԍ����i�[�i4�����ڂ��琔���������Ă���Ƃ����O��j
				int nCOMport = (int)::_tcstoul((PTCHAR)&pTmpBuffer[3],NULL,10);

				//�i�[��|�C���^���擾
				//pECP_PORTINFO pInfo = GetInfo(m_var.nEnablePortCount);
				//�f�o�C�X�����擾
				nValue = 0;
                SetupDiGetDeviceRegistryProperty(hDevInfo,&DeviceInfoData,SPDRP_FRIENDLYNAME,NULL,NULL,0,&nValue);
                if(nValue)
	                {
					//�f�o�C�X�����擾�o�����̂Ŋi�[����

					//�f�o�C�X���̗̈���\�z
					m_var.info[nCOMport].pName = new TCHAR[nValue + 1]();
					//�i�[
					SetupDiGetDeviceRegistryProperty(hDevInfo,&DeviceInfoData,SPDRP_FRIENDLYNAME,NULL,(LPBYTE)m_var.info[nCOMport].pName,nValue,NULL);
					m_var.info[nCOMport].pName[nValue] = 0;	//�I�[�R�[�h
					//�g�p�t���O
					m_var.info[nCOMport].bAlive = true;
	                }
				}
			}
		//���̃f�o�C�X
		++nDeviceNum;
		} 
	//��L�J��
	Unlock();

	//����
	return(ERR_NOERROR);
	}

