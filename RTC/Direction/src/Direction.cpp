// -*- C++ -*-
/*!
 * @file  Direction.cpp
 * @brief ModuleDescription
 * @date $Date$
 *
 * $Id$
 */

#include "Direction.h"

// Module specification
// <rtc-template block="module_spec">
static const char* direction_spec[] =
  {
    "implementation_id", "Direction",
    "type_name",         "Direction",
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
Direction::Direction(RTC::Manager* manager)
    // <rtc-template block="initializer">
  : RTC::DataFlowComponentBase(manager),
    m_Face_AngleIn("Face_Angle", m_Face_Angle),
    m_DirectionOut("Direction", m_Direction)

    // </rtc-template>
{
}

/*!
 * @brief destructor
 */
Direction::~Direction()
{
}



RTC::ReturnCode_t Direction::onInitialize()
{
  // Registration: InPort/OutPort/Service
  // <rtc-template block="registration">
  // Set InPort buffers
  addInPort("Face_Angle", m_Face_AngleIn);
  
  // Set OutPort buffer
  addOutPort("Direction", m_DirectionOut);
  
  // Set service provider to Ports
  
  // Set service consumers to Ports
  
  // Set CORBA Service Ports
  
  // </rtc-template>

  // <rtc-template block="bind_config">
  // </rtc-template>
  
  return RTC::RTC_OK;
}

/*
RTC::ReturnCode_t Direction::onFinalize()
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t Direction::onStartup(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t Direction::onShutdown(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/


RTC::ReturnCode_t Direction::onActivated(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}


RTC::ReturnCode_t Direction::onDeactivated(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}

RTC::ReturnCode_t Direction::onExecute(RTC::UniqueId ec_id)
{
	if (m_Face_AngleIn.isNew()){
		m_Face_AngleIn.read();

		int data_id=-1;

		for (int i = 0; i < 6; i++){
			if (m_Face_Angle.data[i * 3]>0.000001 || m_Face_Angle.data[i * 3]<-0.000001){
				data_id = i;
				std::cout << "i:" << i<<"	";
			}
			
		}

		if (data_id == -1){ 
			std::cout << "-1" << std::endl;
			return RTC::RTC_OK; }

		std::cout << "pitch:" << m_Face_Angle.data[data_id * 3 + 0] << ","
			<< "yaw:" << m_Face_Angle.data[data_id * 3 + 1] << ","
			<< "roll:" << m_Face_Angle.data[data_id * 3 + 2];



		double pitch = m_Face_Angle.data[data_id * 3 + 0];
		double yaw = m_Face_Angle.data[data_id * 3 + 1];
		double roll = m_Face_Angle.data[data_id * 3 + 2];

		double output = -1;

		if (yaw>15)
		{
			output = 2;
			std::cout << "		方向：左向き" <<output<< std::endl;
		}
		else if (yaw < -15)
		{
			output = 3;
			std::cout << "		方向：右向き" << output<<std::endl;
		}
		else if (pitch > 30)
		{
			output = 0;
			std::cout << "		方向：上向き" <<output<< std::endl;
		}
		else if (pitch<-15)
		{
			output = 1;
			std::cout << "		方向：下向き" <<output<< std::endl;
		}
		else{ std::cout<<std::endl; }

		m_Direction.data = output;
		m_DirectionOut.write();
	}
  return RTC::RTC_OK;
}

/*
RTC::ReturnCode_t Direction::onAborting(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t Direction::onError(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t Direction::onReset(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t Direction::onStateUpdate(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t Direction::onRateChanged(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/



extern "C"
{
 
  void DirectionInit(RTC::Manager* manager)
  {
    coil::Properties profile(direction_spec);
    manager->registerFactory(profile,
                             RTC::Create<Direction>,
                             RTC::Delete<Direction>);
  }
  
};


