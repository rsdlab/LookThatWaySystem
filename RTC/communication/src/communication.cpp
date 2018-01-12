// -*- C++ -*-
/*!
 * @file  communication.cpp
 * @brief ModuleDescription
 * @date $Date$
 *
 * $Id$
 */

#include "communication.h"

// Module specification
// <rtc-template block="module_spec">
static const char* communication_spec[] =
  {
    "implementation_id", "communication",
    "type_name",         "communication",
    "description",       "ModuleDescription",
    "version",           "1.0.0",
    "vendor",            "rsdlab",
    "category",          "Category",
    "activity_type",     "PERIODIC",
    "kind",              "DataFlowComponent",
    "max_instance",      "1",
    "language",          "C++",
    "lang_type",         "compile",
    ""
  };
// </rtc-template>

/*!
 * @brief constructor
 * @param manager Maneger Object
 */
communication::communication(RTC::Manager* manager)
    // <rtc-template block="initializer">
  : RTC::DataFlowComponentBase(manager),
    m_dataIn("data", m_data),
    m_judgeIn("judge", m_judge),
    m_outputOut("output", m_output),
    m_commandOut("command", m_command)

    // </rtc-template>
{
}

/*!
 * @brief destructor
 */
communication::~communication()
{
}

std::string voice_data;	//�����f�[�^
double judge_data;		//���f�f�[�^
double command_data;	//�R�}���h�f�[�^
std::string talk_data;	//�����o�̓f�[�^
int count=0;				//�������J�E���g
int flag = 0;
int flag2 = 0;
int selectflag;
//int i = 30;

RTC::ReturnCode_t communication::onInitialize()
{
  // Registration: InPort/OutPort/Service
  // <rtc-template block="registration">
  // Set InPort buffers
  addInPort("data", m_dataIn);
  addInPort("judge", m_judgeIn);
  
  // Set OutPort buffer
  addOutPort("output", m_outputOut);
  addOutPort("command", m_commandOut);
  
  // Set service provider to Ports
  
  // Set service consumers to Ports
  
  // Set CORBA Service Ports
  
  // </rtc-template>

  // <rtc-template block="bind_config">
  // </rtc-template>
  
  return RTC::RTC_OK;
}



RTC::ReturnCode_t communication::onActivated(RTC::UniqueId ec_id)
{
	Sleep(1000);

	talk_data = "�N�����܂����D";
	talk_data = SjistoUTF8(talk_data);
	m_output.data = talk_data.c_str();
	m_outputOut.write();

	Sleep(2000);

	talk_data = "���ꂩ��V�X�e���̐����ɂ͂���܂�";
	talk_data = SjistoUTF8(talk_data);
	m_output.data = talk_data.c_str();
	m_outputOut.write();
	
	Sleep(3000);

	talk_data = "����͂͂��Ƃ������}�ƂƂ��ɂ������ނ��Ăق��������Ȃ��܂�";
	talk_data = SjistoUTF8(talk_data);
	m_output.data = talk_data.c_str();
	m_outputOut.write();

	Sleep(5000);

	talk_data = "�A�[���������Ă��炢���т傤���̂����̂ق������ŏ��s�����߂܂��D";
	talk_data = SjistoUTF8(talk_data);
	m_output.data = talk_data.c_str();
	m_outputOut.write();

	Sleep(5000);

	talk_data = "�����͂ł��܂������H";
	talk_data = SjistoUTF8(talk_data);
	m_output.data = talk_data.c_str();
	m_outputOut.write();

	selectflag = 0;

  return RTC::RTC_OK;
}


RTC::ReturnCode_t communication::onDeactivated(RTC::UniqueId ec_id)
{
	

  return RTC::RTC_OK;
}


RTC::ReturnCode_t communication::onExecute(RTC::UniqueId ec_id)
{
	if (m_dataIn.isNew())
	{
		m_dataIn.read();
		if (selectflag == 0)
		{
			flag = 1;
		}
		
	}
	
	if (m_judgeIn.isNew())
	{
		m_judgeIn.read();
		if (selectflag == 1)
		{
			flag2 = 1;
		}
		
	}

	if (flag==1)
	{
		voice_data = m_data.data;
		voice_data = UTF8toSjis(voice_data);
		printf("%s\n", voice_data);

		if (voice_data == "�͂�")
		{
			
			talk_data = "�������ނ���";
			talk_data = SjistoUTF8(talk_data);
			m_output.data = talk_data.c_str();
			m_outputOut.write();

			Sleep(1000);

			talk_data = "�ق�";
			talk_data = SjistoUTF8(talk_data);
			m_output.data = talk_data.c_str();
			m_outputOut.write();

			command_data = 1;
			m_command.data = command_data;
			m_commandOut.write();

			selectflag = 1;

		}
			while (m_dataIn.isEmpty());
			m_dataIn.read();
			flag = 0;

			printf("%d", selectflag);
			
	}
	
	if (flag2==1)
	{

		judge_data = m_judge.data;
		
		printf("%d\n", judge_data);
		
		if (judge_data == 1)
		{
			count++;
			if (count == 1)
			{
				talk_data = "�������傤�߁D";
				talk_data = SjistoUTF8(talk_data);
				m_output.data = talk_data.c_str();
				m_outputOut.write();

				Sleep(1000);

				talk_data = "�����ď����͂�낵���ł����H�D";
				talk_data = SjistoUTF8(talk_data);
				m_output.data = talk_data.c_str();
				m_outputOut.write();

				selectflag = 0;
			}
			else if (count == 2)
			{
				talk_data = "�ɂ��傤�߁D";
				talk_data = SjistoUTF8(talk_data);
				m_output.data = talk_data.c_str();
				m_outputOut.write();

				Sleep(1000);

				talk_data = "�����ď����͂�낵���ł����H�D";
				talk_data = SjistoUTF8(talk_data);
				m_output.data = talk_data.c_str();
				m_outputOut.write();
				
				selectflag = 0;
			}
			else if (count == 3)
			{
				talk_data = "���񂵂傤�߁D";
				talk_data = SjistoUTF8(talk_data);
				m_output.data = talk_data.c_str();
				m_outputOut.write();

				Sleep(1000);

				talk_data = "���Ȃ��̏����ł��D";
				talk_data = SjistoUTF8(talk_data);
				m_output.data = talk_data.c_str();
				m_outputOut.write();
				
				selectflag = 0;

				count = 0;
			}
		}

		if (judge_data == 0)
		{
			count = 0;

			talk_data = "���Ȃ��̂܂��ł��D";
			talk_data = SjistoUTF8(talk_data);
			m_output.data = talk_data.c_str();
			m_outputOut.write();

			Sleep(1500);

			talk_data = "�����܂����H";
			talk_data = SjistoUTF8(talk_data);
			m_output.data = talk_data.c_str();
			m_outputOut.write();

			selectflag = 0;

		}

		while (m_dataIn.isEmpty());
		m_dataIn.read();
		flag2 = 0;

		printf("%d", selectflag);
	}

  return RTC::RTC_OK;
}



std::string communication::UTF8toSjis(std::string srcUTF8)
{
	//Unicode�֕ϊ���̕����񒷂𓾂�
	int lenghtUnicode = MultiByteToWideChar(CP_UTF8, 0, srcUTF8.c_str(), srcUTF8.size() + 1, NULL, 0);

	//�K�v�ȕ�����Unicode������̃o�b�t�@���m��
	wchar_t* bufUnicode = new wchar_t[lenghtUnicode];

	//UTF8����Unicode�֕ϊ�
	MultiByteToWideChar(CP_UTF8, 0, srcUTF8.c_str(), srcUTF8.size() + 1, bufUnicode, lenghtUnicode);

	//ShiftJIS�֕ϊ���̕����񒷂𓾂�
	int lengthSJis = WideCharToMultiByte(CP_THREAD_ACP, 0, bufUnicode, -1, NULL, 0, NULL, NULL);

	//�K�v�ȕ�����ShiftJIS������̃o�b�t�@���m��
	char* bufShiftJis = new char[lengthSJis];

	//Unicode����ShiftJIS�֕ϊ�
	WideCharToMultiByte(CP_THREAD_ACP, 0, bufUnicode, lenghtUnicode + 1, bufShiftJis, lengthSJis, NULL, NULL);

	std::string strSJis(bufShiftJis);

	delete bufUnicode;
	delete bufShiftJis;

	return strSJis;
}
std::string communication::SjistoUTF8(std::string srcSjis)
{
	//Unicode�֕ϊ���̕����񒷂𓾂�
	int lenghtUnicode = MultiByteToWideChar(CP_THREAD_ACP, 0, srcSjis.c_str(), srcSjis.size() + 1, NULL, 0);

	//�K�v�ȕ�����Unicode������̃o�b�t�@���m��
	wchar_t* bufUnicode = new wchar_t[lenghtUnicode];

	//ShiftJIS����Unicode�֕ϊ�
	MultiByteToWideChar(CP_THREAD_ACP, 0, srcSjis.c_str(), srcSjis.size() + 1, bufUnicode, lenghtUnicode);


	//UTF8�֕ϊ���̕����񒷂𓾂�
	int lengthUTF8 = WideCharToMultiByte(CP_UTF8, 0, bufUnicode, -1, NULL, 0, NULL, NULL);

	//�K�v�ȕ�����UTF8������̃o�b�t�@���m��
	char* bufUTF8 = new char[lengthUTF8];

	//Unicode����UTF8�֕ϊ�
	WideCharToMultiByte(CP_UTF8, 0, bufUnicode, lenghtUnicode + 1, bufUTF8, lengthUTF8, NULL, NULL);

	std::string strUTF8(bufUTF8);

	delete bufUnicode;
	delete bufUTF8;

	return strUTF8;
}

extern "C"
{
 
  void communicationInit(RTC::Manager* manager)
  {
    coil::Properties profile(communication_spec);
    manager->registerFactory(profile,
                             RTC::Create<communication>,
                             RTC::Delete<communication>);
  }
  
};


