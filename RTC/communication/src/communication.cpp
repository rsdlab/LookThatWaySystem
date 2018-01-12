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

std::string voice_data;	//音声データ
double judge_data;		//判断データ
double command_data;	//コマンドデータ
std::string talk_data;	//音声出力データ
int count=0;				//勝利数カウント
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

	talk_data = "起動しました．";
	talk_data = SjistoUTF8(talk_data);
	m_output.data = talk_data.c_str();
	m_outputOut.write();

	Sleep(2000);

	talk_data = "これからシステムの説明にはいります";
	talk_data = SjistoUTF8(talk_data);
	m_output.data = talk_data.c_str();
	m_outputOut.write();
	
	Sleep(3000);

	talk_data = "今回ははいという合図とともにあっちむいてほいをおこないます";
	talk_data = SjistoUTF8(talk_data);
	m_output.data = talk_data.c_str();
	m_outputOut.write();

	Sleep(5000);

	talk_data = "アームが動いてからいちびょうごのかおのほうこうで勝敗を決めます．";
	talk_data = SjistoUTF8(talk_data);
	m_output.data = talk_data.c_str();
	m_outputOut.write();

	Sleep(5000);

	talk_data = "準備はできましたか？";
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

		if (voice_data == "はい")
		{
			
			talk_data = "あっちむいて";
			talk_data = SjistoUTF8(talk_data);
			m_output.data = talk_data.c_str();
			m_outputOut.write();

			Sleep(1000);

			talk_data = "ほい";
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
				talk_data = "いっしょうめ．";
				talk_data = SjistoUTF8(talk_data);
				m_output.data = talk_data.c_str();
				m_outputOut.write();

				Sleep(1000);

				talk_data = "続けて準備はよろしいですか？．";
				talk_data = SjistoUTF8(talk_data);
				m_output.data = talk_data.c_str();
				m_outputOut.write();

				selectflag = 0;
			}
			else if (count == 2)
			{
				talk_data = "にしょうめ．";
				talk_data = SjistoUTF8(talk_data);
				m_output.data = talk_data.c_str();
				m_outputOut.write();

				Sleep(1000);

				talk_data = "続けて準備はよろしいですか？．";
				talk_data = SjistoUTF8(talk_data);
				m_output.data = talk_data.c_str();
				m_outputOut.write();
				
				selectflag = 0;
			}
			else if (count == 3)
			{
				talk_data = "さんしょうめ．";
				talk_data = SjistoUTF8(talk_data);
				m_output.data = talk_data.c_str();
				m_outputOut.write();

				Sleep(1000);

				talk_data = "あなたの勝利です．";
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

			talk_data = "あなたのまけです．";
			talk_data = SjistoUTF8(talk_data);
			m_output.data = talk_data.c_str();
			m_outputOut.write();

			Sleep(1500);

			talk_data = "続けますか？";
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
	//Unicodeへ変換後の文字列長を得る
	int lenghtUnicode = MultiByteToWideChar(CP_UTF8, 0, srcUTF8.c_str(), srcUTF8.size() + 1, NULL, 0);

	//必要な分だけUnicode文字列のバッファを確保
	wchar_t* bufUnicode = new wchar_t[lenghtUnicode];

	//UTF8からUnicodeへ変換
	MultiByteToWideChar(CP_UTF8, 0, srcUTF8.c_str(), srcUTF8.size() + 1, bufUnicode, lenghtUnicode);

	//ShiftJISへ変換後の文字列長を得る
	int lengthSJis = WideCharToMultiByte(CP_THREAD_ACP, 0, bufUnicode, -1, NULL, 0, NULL, NULL);

	//必要な分だけShiftJIS文字列のバッファを確保
	char* bufShiftJis = new char[lengthSJis];

	//UnicodeからShiftJISへ変換
	WideCharToMultiByte(CP_THREAD_ACP, 0, bufUnicode, lenghtUnicode + 1, bufShiftJis, lengthSJis, NULL, NULL);

	std::string strSJis(bufShiftJis);

	delete bufUnicode;
	delete bufShiftJis;

	return strSJis;
}
std::string communication::SjistoUTF8(std::string srcSjis)
{
	//Unicodeへ変換後の文字列長を得る
	int lenghtUnicode = MultiByteToWideChar(CP_THREAD_ACP, 0, srcSjis.c_str(), srcSjis.size() + 1, NULL, 0);

	//必要な分だけUnicode文字列のバッファを確保
	wchar_t* bufUnicode = new wchar_t[lenghtUnicode];

	//ShiftJISからUnicodeへ変換
	MultiByteToWideChar(CP_THREAD_ACP, 0, srcSjis.c_str(), srcSjis.size() + 1, bufUnicode, lenghtUnicode);


	//UTF8へ変換後の文字列長を得る
	int lengthUTF8 = WideCharToMultiByte(CP_UTF8, 0, bufUnicode, -1, NULL, 0, NULL, NULL);

	//必要な分だけUTF8文字列のバッファを確保
	char* bufUTF8 = new char[lengthUTF8];

	//UnicodeからUTF8へ変換
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


