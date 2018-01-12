// -*- C++ -*-
/*!
* @file  Finger_Point.cpp
* @brief Finger_Point
* @date $Date$
*
* $Id$
*/

#include "Finger_Point.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include<windows.h>

// Module specification
// <rtc-template block="module_spec">
static const char* finger_point_spec[] =
{
	"implementation_id", "Finger_Point",
	"type_name", "Finger_Point",
	"description", "Finger_Point",
	"version", "1.0.0",
	"vendor", "rsdlab",
	"category", "Category",
	"activity_type", "PERIODIC",
	"kind", "DataFlowComponent",
	"max_instance", "1",
	"language", "C++",
	"lang_type", "compile",
	// Configuration variables
	"conf.default.Speed", "20",

	// Widget
	"conf.__widget__.Speed", "text",
	// Constraints

	"conf.__type__.Speed", "int",

	""
};

// </rtc-template>

/*!
* @brief constructor
* @param manager Maneger Object
*/
Finger_Point::Finger_Point(RTC::Manager* manager)
// <rtc-template block="initializer">
: RTC::DataFlowComponentBase(manager),
m_commandIn("command", m_command),
m_directionOut("direction", m_direction),
m_ManipulatorCommonInterface_CommonPort("ManipulatorCommonInterface_Common"),
m_ManipulatorCommonInterface_MiddlePort("ManipulatorCommonInterface_Middle")

// </rtc-template>
{
}

/*!
* @brief destructor
*/
Finger_Point::~Finger_Point()
{
}


RTC::ReturnCode_t Finger_Point::onInitialize()
{
	// Registration: InPort/OutPort/Service
	// <rtc-template block="registration">
	// Set InPort buffers
	addInPort("command", m_commandIn);

	// Set OutPort buffer
	addOutPort("direction", m_directionOut);

	// Set service provider to Ports

	// Set service consumers to Ports
	m_ManipulatorCommonInterface_CommonPort.registerConsumer("ManipulatorCommonInterface_Common", "JARA_ARM::ManipulatorCommonInterface_Common", m_ManipulatorCommonInterface_Common);
	m_ManipulatorCommonInterface_MiddlePort.registerConsumer("ManipulatorCommonInterface_Middle", "JARA_ARM::ManipulatorCommonInterface_Middle", m_ManipulatorCommonInterface_Middle);

	// Set CORBA Service Ports
	addPort(m_ManipulatorCommonInterface_CommonPort);
	addPort(m_ManipulatorCommonInterface_MiddlePort);

	// </rtc-template>

	// <rtc-template block="bind_config">
	// Bind variables and configuration variable
	bindParameter("Speed", m_Speed, "20");
	// </rtc-template>

	return RTC::RTC_OK;
}

/*
RTC::ReturnCode_t Finger_Point::onFinalize()
{
return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t Finger_Point::onStartup(RTC::UniqueId ec_id)
{
return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t Finger_Point::onShutdown(RTC::UniqueId ec_id)
{
return RTC::RTC_OK;
}
*/


RTC::ReturnCode_t Finger_Point::onActivated(RTC::UniqueId ec_id)
{
	return RTC::RTC_OK;
	
}


RTC::ReturnCode_t Finger_Point::onDeactivated(RTC::UniqueId ec_id)
{
	return RTC::RTC_OK;
}


RTC::ReturnCode_t Finger_Point::onExecute(RTC::UniqueId ec_id)
{
	if ((m_commandIn.isNew()))
	{
		int x;

		m_commandIn.read();
		std::cout << "m_command = " << m_command.data << std::endl;

		JARA_ARM::ManipInfo_var minfovar;
		JARA_ARM::ManipInfo minfo;
		JARA_ARM::JointPos jointPoint;

		m_ManipulatorCommonInterface_Common->getManipInfo(minfovar);
		minfo = minfovar;

		//�e���[�^�̒�`
		jointPoint.length(minfo.axisNum);

		//�����p���ݒ�
		jointPoint[0] = 0; //0
		jointPoint[1] = 0; //0
		jointPoint[2] = 90;//90
		jointPoint[3] = 0; //0

		m_rid = m_ManipulatorCommonInterface_Middle->movePTPJointAbs(jointPoint);
		if (m_rid->id != 0){//Error
			std::cout << "movePTPJointAbs ERROR" << std::endl;
			std::cout << m_rid->comment << std::endl << std::endl;
		}

		std::cout << "�����p��OK" << std::endl;

		if (m_command.data = 1)
		{
			//���ԍ��킹
			Sleep(1000);
				
				//�����_���Ɏ��s����֐��̍쐬
				//�����̎��������
				srand(time(NULL));

				//0~3�̓��C�K���Ȑ��𐶐� 
				x = rand() % 4;

				if (x == 0) //�����
				{
					jointPoint[0] = 0;
					jointPoint[1] = 0;
					jointPoint[2] = 60;
					jointPoint[3] = 0;
				}
				else if (x == 1) //������
				{
					jointPoint[0] = 0;
					jointPoint[1] = 0;
					jointPoint[2] = 120;
					jointPoint[3] = 0;
				}
				else if (x == 2) //������
				{
					jointPoint[0] = -30;
					jointPoint[1] = 0;
					jointPoint[2] = 90;
					jointPoint[3] = 0;
				}
				else if (x == 3) //�E����
				{
					jointPoint[0] = 30;
					jointPoint[1] = 0;
					jointPoint[2] = 90;
					jointPoint[3] = 0;
				}

				m_rid = m_ManipulatorCommonInterface_Middle->movePTPJointAbs(jointPoint);
			}

			//���M�p�̕ϐ���`
			int y;
			y = x;
			
			std::cout << "�I�񂾕�����" << y << std::endl;

			//�����p���ݒ�
			jointPoint[0] = 0; 
			jointPoint[1] = 0; 
			jointPoint[2] = 90;
			jointPoint[3] = 0; 

			//�����I��
			//�I�����ʂ𑗐M
			m_direction.data = y; 
			m_directionOut.write();
		}
	return RTC::RTC_OK;
}

/*
RTC::ReturnCode_t Finger_Point::onAborting(RTC::UniqueId ec_id)
{
return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t Finger_Point::onError(RTC::UniqueId ec_id)
{
return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t Finger_Point::onReset(RTC::UniqueId ec_id)
{
return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t Finger_Point::onStateUpdate(RTC::UniqueId ec_id)
{
return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t Finger_Point::onRateChanged(RTC::UniqueId ec_id)
{
return RTC::RTC_OK;
}
*/



extern "C"
{

	void Finger_PointInit(RTC::Manager* manager)
	{
		coil::Properties profile(finger_point_spec);
		manager->registerFactory(profile,
			RTC::Create<Finger_Point>,
			RTC::Delete<Finger_Point>);
	}

};

