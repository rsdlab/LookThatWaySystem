// -*- C++ -*-
/*!
 * @file  Game_Judge.cpp
 * @brief Game_Judge
 * @date $Date$
 *
 * $Id$
 */

#include "Game_Judge.h"

// Module specification
// <rtc-template block="module_spec">
static const char* game_judge_spec[] =
  {
    "implementation_id", "Game_Judge",
    "type_name",         "Game_Judge",
    "description",       "Game_Judge",
    "version",           "1.0.0",
    "vendor",            "VenderName",
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
Game_Judge::Game_Judge(RTC::Manager* manager)
    // <rtc-template block="initializer">
  : RTC::DataFlowComponentBase(manager),
    m_arm_directionIn("arm_direction", m_arm_direction),
    m_face_directionIn("face_direction", m_face_direction),
    m_game_resultOut("game_result", m_game_result)

    // </rtc-template>
{
}

/*!
 * @brief destructor
 */
Game_Judge::~Game_Judge()
{
}



RTC::ReturnCode_t Game_Judge::onInitialize()
{
  // Registration: InPort/OutPort/Service
  // <rtc-template block="registration">
  // Set InPort buffers
  addInPort("arm_direction", m_arm_directionIn);
  addInPort("face_direction", m_face_directionIn);
  
  // Set OutPort buffer
  addOutPort("game_result", m_game_resultOut);
  
  // Set service provider to Ports
  
  // Set service consumers to Ports
  
  // Set CORBA Service Ports
  
  // </rtc-template>

  // <rtc-template block="bind_config">
  // </rtc-template>
  
  return RTC::RTC_OK;
}

/*
RTC::ReturnCode_t Game_Judge::onFinalize()
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t Game_Judge::onStartup(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t Game_Judge::onShutdown(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/


RTC::ReturnCode_t Game_Judge::onActivated(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}


RTC::ReturnCode_t Game_Judge::onDeactivated(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}


RTC::ReturnCode_t Game_Judge::onExecute(RTC::UniqueId ec_id)
{
	{
		if (m_arm_directionIn.isNew() & m_face_directionIn.isNew())
		{
			//アーム方向の受信
			m_arm_directionIn.read();
			std::cout << "Arm_direction = " << m_arm_direction.data << std::endl;
			
			Sleep(1500);

			//顔方向の受信
			m_face_directionIn.read();
			std::cout << "face_direction = " << m_face_direction.data << std::endl;

			//人間の負けの場合
			if (m_arm_direction.data == m_face_direction.data){

				m_game_result.data = 0;

				std::cout << "負け " << m_game_result.data << std::endl;

				}

			//人間の勝ちの場合
			else if (m_arm_direction.data != m_face_direction.data){
				m_game_result.data = 1;

				std::cout << "勝ち " << m_game_result.data << std::endl;
				}

			//勝敗結果を送信
			m_game_resultOut.write();

		}

		return RTC::RTC_OK;

	}
}
/*
RTC::ReturnCode_t Game_Judge::onAborting(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t Game_Judge::onError(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t Game_Judge::onReset(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t Game_Judge::onStateUpdate(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t Game_Judge::onRateChanged(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/



extern "C"
{
 
  void Game_JudgeInit(RTC::Manager* manager)
  {
    coil::Properties profile(game_judge_spec);
    manager->registerFactory(profile,
                             RTC::Create<Game_Judge>,
                             RTC::Delete<Game_Judge>);
  }
  
};


