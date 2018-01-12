// -*- C++ -*-
/*!
 * @file  KinectFace.cpp
 * @brief Image on Kinect
 * @date $Date$
 *
 * @author 名城大学
 * ロボットシステムデザイン研究室
 *
 * $Id$
 */

#define ERROR_CHECK( ret )	\
	if ( (ret) != S_OK){\
		std::stringstream ss;\
	ss << "failed　" #ret " " << std::hex << ret << std::endl;\
	throw std::runtime_error(ss.str().c_str());\
	}


#define WIDTH 1920
#define HEIGHT 1080
#define _USE_MATH_DEFINES


#include "KinectFace.h"
#include <Kinect.h>
#include <Kinect.face.h>
#include <sstream>
#include <opencv2/opencv.hpp>
#include <atlbase.h>
#include <array>

#include <Math.h>

// Module specification
// <rtc-template block="module_spec">
static const char* kinectface_spec[] =
  {
    "implementation_id", "KinectFace",
    "type_name",         "KinectFace",
    "description",       "Image on Kinect",
    "version",           "1.0.0",
    "vendor",            "rsdlab",
    "category",          "Sensor",
    "activity_type",     "PERIODIC",
    "kind",              "DataFlowComponent",
    "max_instance",      "1",
    "language",          "C++",
    "lang_type",         "compile",
    // Configuration variables
    "conf.default.Output", "raw,camera,jpeg",
    "conf.default.Size", "480x270",

    // Widget
    "conf.__widget__.Output", "checkbox",
    "conf.__widget__.Size", "text",
    // Constraints
    "conf.__constraints__.Output", "(raw,camera,jpeg)",
    "conf.__constraints__.Size", "(240x135,480x270,960x540,1920x1080)",

    "conf.__type__.Output", "string",
    "conf.__type__.Size", "string",

    ""
  };
// </rtc-template>


/*!
 * @brief constructor
 * @param manager Maneger Object
 */
KinectFace::KinectFace(RTC::Manager* manager)
    // <rtc-template block="initializer">
  : RTC::DataFlowComponentBase(manager),
    m_CameraImageOut("CameraImage", m_CameraImage),
    m_FaceAngleOut("FaceAngle", m_FaceAngle),
    m_FacepropertyOut("Faceproperty", m_Faceproperty)


    // </rtc-template>
{

}

/*!
 * @brief destructor
 */
KinectFace::~KinectFace()
{

}

/*!
 * Kinectへ接続する
 */

RTC::ReturnCode_t KinectFace::onInitialize()
{
	std::cout << "onInitialize_start" << std::endl;
  // Registration: InPort/OutPort/Service
  // <rtc-template block="registration">
  // Set InPort buffers
  
  // Set OutPort buffer
  addOutPort("CameraImage", m_CameraImageOut);
  addOutPort("FaceAngle", m_FaceAngleOut);
  addOutPort("Faceproperty", m_FacepropertyOut);
  
  // Set service provider to Ports
  
  // Set service consumers to Ports
  
  // Set CORBA Service Ports
  
  // </rtc-template>

  // <rtc-template block="bind_config">
  // Bind variables and configuration variable
  bindParameter("Output", m_Output, "raw,camera,jpeg");
  bindParameter("Size", m_Size, "480x270");



  // </rtc-template>
  std::cout << "onInitialize_end" << std::endl;
  return RTC::RTC_OK;
}

/*
RTC::ReturnCode_t KinectFace::onFinalize()
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t KinectFace::onStartup(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t KinectFace::onShutdown(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*!
 * ポート及びデータの初期化
 */

//グローバル変数っぽいの
//Kinect
CComPtr<IKinectSensor> kinect;

//ColorImgae
CComPtr<IColorFrameReader> colorFrameReader = nullptr;
int colorWidth;
int colorHeight;
unsigned int colorBtytesPerPixel;
ColorImageFormat colorFormat;
std::vector<BYTE> colorBuffer;

//DepthImage
//CComPtr<IDepthFrameReader> depthFrameReader = nullptr;
//int depthPointX;
//int depthPointY;
//int depthWidth;
//int depthHeight;
//std::vector<UINT16> depthBuffer;
//const char* DepthWindowName = "Depth Imgae";

//Face&Body
std::array<CComPtr<IFaceFrameReader>, BODY_COUNT> faceFrameReader;
CComPtr<IBodyFrameReader> bodyFrameReader = nullptr;
IBody* bodies[BODY_COUNT];
cv::Mat colorFaceImage;

double Angleoutput[BODY_COUNT * 3];
int Facepropout[BODY_COUNT * 8];

RTC::ReturnCode_t KinectFace::onActivated(RTC::UniqueId ec_id)
{
	std::cout << "onActivated_start" << std::endl;

	std::cout << "KinectSensor Open" << std::endl;
	//KinectSensor
	kinect = nullptr;

	//Get Kinect & Kinect Open
	ERROR_CHECK(::GetDefaultKinectSensor(&kinect));
	
	if (kinect == nullptr){ std::cout << "kinect not found" << std::endl; }

	ERROR_CHECK(kinect->Open());

	std::cout << "onActivated_ColorImage_set" << std::endl;
	//ColorImgae
	colorFrameReader = nullptr;
	colorFormat = ColorImageFormat_Bgra;
	CComPtr<IColorFrameSource> colorFrameSource;
	CComPtr<IFrameDescription> colorFrameDescription;

	ERROR_CHECK(kinect->get_ColorFrameSource(&colorFrameSource));
	ERROR_CHECK(colorFrameSource->OpenReader(&colorFrameReader));

	ERROR_CHECK(colorFrameSource->CreateFrameDescription(ColorImageFormat::ColorImageFormat_Bgra, &colorFrameDescription));
	ERROR_CHECK(colorFrameDescription->get_Width(&colorWidth));
	ERROR_CHECK(colorFrameDescription->get_Height(&colorHeight));
	ERROR_CHECK(colorFrameDescription->get_BytesPerPixel(&colorBtytesPerPixel));

	colorBuffer.resize(colorWidth * colorHeight * colorBtytesPerPixel);

	//std::cout << "onActivated_DepthImage_set" << std::endl;
	//DepthImage
	//CComPtr<IDepthFrameSource> depthFrameSource;
	//CComPtr<IFrameDescription> depthFrameDescription;
	//
	//depthFrameReader = nullptr;
	//ERROR_CHECK(kinect->get_DepthFrameSource(&depthFrameSource));
	//ERROR_CHECK(depthFrameSource->OpenReader(&depthFrameReader));
	//
	//ERROR_CHECK(depthFrameSource->get_FrameDescription(&depthFrameDescription));
	//ERROR_CHECK(depthFrameDescription->get_Width(&depthWidth));
	//ERROR_CHECK(depthFrameDescription->get_Height(&depthHeight));
	//
	//depthPointX = depthWidth / 2;
	//depthPointY = depthHeight / 2;
	//
	//UINT16 minDepthReliableDistance;
	//UINT16 maxDepthReliableDistance;
	//
	//ERROR_CHECK(depthFrameSource->get_DepthMinReliableDistance(&minDepthReliableDistance));
	//ERROR_CHECK(depthFrameSource->get_DepthMaxReliableDistance(&maxDepthReliableDistance));
	//
	//std::cout << "Depth_min		:" << minDepthReliableDistance << std::endl;
	//std::cout << "Depth_max		:" << maxDepthReliableDistance << std::endl;
	//
	//depthBuffer.resize(depthWidth * depthHeight);

	std::cout << "onActivated_BodyFace_set" << std::endl;
	//Body&Face

	CComPtr<IBodyFrameSource> bodyFrameSource;

	ERROR_CHECK(kinect->get_BodyFrameSource(&bodyFrameSource));
	ERROR_CHECK(bodyFrameSource->OpenReader(&bodyFrameReader));
	
	DWORD features =
		FaceFrameFeatures::FaceFrameFeatures_BoundingBoxInColorSpace
		| FaceFrameFeatures::FaceFrameFeatures_PointsInColorSpace
		| FaceFrameFeatures::FaceFrameFeatures_RotationOrientation
		| FaceFrameFeatures::FaceFrameFeatures_Happy
		| FaceFrameFeatures::FaceFrameFeatures_RightEyeClosed
		| FaceFrameFeatures::FaceFrameFeatures_LeftEyeClosed
		| FaceFrameFeatures::FaceFrameFeatures_MouthOpen
		| FaceFrameFeatures::FaceFrameFeatures_MouthMoved
		| FaceFrameFeatures::FaceFrameFeatures_LookingAway
		| FaceFrameFeatures::FaceFrameFeatures_Glasses
		| FaceFrameFeatures::FaceFrameFeatures_FaceEngagement;


	for (int count = 0; count < BODY_COUNT; count++)
	{
		CComPtr<IFaceFrameSource> faceFrameSource;
		ERROR_CHECK(CreateFaceFrameSource(kinect, 0, features, &faceFrameSource));
		ERROR_CHECK(faceFrameSource->OpenReader(&faceFrameReader[count]));
	}



	//cv::namedWindow(DepthWindowName);
	//cv::setMouseCallback(DepthWindowName,&KinectApp::mouseCalback,this);
	//std::cout << "onActivated_m_CameraImage_set" << std::endl;
	//m_CameraImage.data.image.height = colorHeight;
	//m_CameraImage.data.image.width = colorWidth;
	//m_CameraImage.data.image.format = Img::RGB;
	cv::namedWindow("Color Image");
	//cv::namedWindow("Depth Image");
	cv::namedWindow("ColorFaceImage");
	//Kinectはカメラパラメータ・歪みパラメータとる
	//気が向いたらやる(かもしれない)
	//m_CameraImage.data.intrinsic.distortion_coefficient = ;
	//m_CameraImage.data.intrinsic.matrix_element = ;


  return RTC::RTC_OK;
}

RTC::ReturnCode_t KinectFace::onDeactivated(RTC::UniqueId ec_id)
{
	std::cout << "onDeactivated_start" << std::endl;

	cv::destroyAllWindows();

	if (kinect != nullptr){
		kinect->Close();
		kinect=nullptr;
	};

	std::cout << "onDeactivated_end" << std::endl;
	return RTC::RTC_OK;
}

void updateColorFrame()
{
	std::cout << "updateColorFrame_start" << std::endl;
	CComPtr<IColorFrame> colorFrame;
	auto ret = colorFrameReader->AcquireLatestFrame(&colorFrame);
	if (FAILED(ret)){
		//std::cout << "Color image could not be acquired" << std::endl;
		return;
	}
	ERROR_CHECK(colorFrame->CopyConvertedFrameDataToArray(colorBuffer.size(), &colorBuffer[0], colorFormat));
	std::cout << "updateColorFrame_end" << std::endl;
}

void KinectFace::drawColorFrame()
{
	std::cout << "drawColorFrame_start" << std::endl;
	cv::Mat colorImage(colorHeight, colorWidth, CV_8UC4, &colorBuffer[0]);
	cv::Mat temp;
	cv::imshow("Color Image", colorImage);
	//setTimestamp(m_CameraImage);
	//m_CameraImage.data.captured_time = m_CameraImage.tm;
	//m_CameraImage.data.image.format = Img::CF_RGB;
	cv::cvtColor(colorImage, temp, CV_RGBA2BGR);
	//m_CameraImage.data.image.height = colorHeight;
	//m_CameraImage.data.image.width = colorWidth;
	//m_CameraImage.data.image.raw_data.length(colorWidth*colorHeight * 3);
	//for (int i(0); i < colorHeight; i++){
	//	memcpy(&m_CameraImage.data.image.raw_data[i*colorWidth * 3], &temp.data[i*temp.cols*3], colorWidth * 3);
	//}
	cv::cvtColor(temp,colorFaceImage,CV_RGB2BGR);

	//colorFaceImage = temp.clone();
	//colorFaceImage = cv::Mat(colorHeight, colorWidth, CV_8UC4, &colorBuffer[0]);
	cv::waitKey(1);
	//m_CameraImageOut.write();
	temp.release();
	colorImage.release();
	std::cout << "drawColorFrame_end" << std::endl;
}

//
//void updateDepthFrame()
//{
//	std::cout << "updateDepthFrame_start" << std::endl;
//	CComPtr<IDepthFrame> depthFrame;
//	auto ret = depthFrameReader->AcquireLatestFrame(&depthFrame);
//	if (ret != S_OK){
//	//	std::cout << "Depth image could not be acquired" << std::endl;
//		return;
//	}
//
//	ERROR_CHECK(depthFrame->CopyFrameDataToArray(depthBuffer.size(),&depthBuffer[0]));
//	std::cout << "updateDepthFrame_end" << std::endl;
//}
//
//void KinectFace::drawDepthFrame()
//{
//	std::cout << "drawDepthFrame_start" << std::endl;
//	cv::Mat depthImage(depthHeight, depthWidth, CV_8UC1);
//
//	for (int i = 0; i < depthImage.total(); ++i)
//	{
//		depthImage.data[i] = depthBuffer[i] % 255;
//	}
//	cv::imshow("Depth Image", depthImage);
//	cv::waitKey(1);
//
//	//コンポーネントの型なおしたらいれる
//	/*
//	setTimestamp(m_DepthImage);
//	m_DepthImage.data.captured_time = m_CameraImage.tm;
//	m_DepthImage.data.image.format = Img::CF_RGB;
//	m_DepthImage.data.image.height = depthHeight;
//	m_DepthImage.data.image.width = depthWidth;
//	m_DepthImage.data.image.raw_data.length(depthWidth*depthHeight);
//	for (int i(0); i < depthHeight; i++){
//		memcpy(&m_DepthImage.data.image.raw_data[i*depthWidth], &DepthImage.data[i*DepthImage.step], depthWidth);
//	}
//	m_DepthImageOut.write();
//	*/
//	depthImage.release();
//	std::cout << "drawDepthFrame_end" << std::endl;
//}

double opitch = 0.0;
double oyaw = 0.0;
double oroll = 0.0;

void quaternion2degree(Vector4 faceResult,double* pitch, double* yaw, double* roll)
{
	std::cout << "quaternion2degree_start" << std::endl;
	Vector4 quaternion = faceResult;
	double w = quaternion.w;
	double x = quaternion.x;
	double y = quaternion.y;
	double z = quaternion.z;
	double PITCH = (double)(atan2(2 * (y * z + w * x), w * w - x * x - y * y + z * z) / M_PI * 180.0);
	double YAW = (double)(asin(2 * (w * y - x * z)) / M_PI * 180.0);
	double ROLL = (double)(atan2(2 * (x * y + w * z), w * w + x * x - y * y - z * z) / M_PI * 180.0);

	*pitch = PITCH - opitch;
	*yaw = YAW - oyaw;
	*roll = ROLL - oroll;
	std::cout << "quaternion2degree_end" << std::endl;
}

std::string labels[8] = {"Happy","Engaged","Wearing Glsses","Left Eye Closed","Right Eye Clsed", "Mouth Open","Mouse Moved","Looking Away"};
cv::Scalar colors[6] = { cv::Scalar(0, 0, 0), cv::Scalar(255, 0, 0), cv::Scalar(0, 255, 0), cv::Scalar(0, 0, 255), cv::Scalar(255, 255, 0), cv::Scalar(255, 0, 255) };

inline void result(const CComPtr<IFaceFrameResult>& result, const int count)
{
	std::cout << "result_start" << std::endl;
	std::array<PointF, FacePointType::FacePointType_Count> points;
	ERROR_CHECK(result->GetFacePointsInColorSpace(FacePointType::FacePointType_Count, &points[0]));
	for (const PointF p : points)
	{
		int x = static_cast<int>(std::ceil(p.X));
		int y = static_cast<int>(std::ceil(p.Y));
	
		cv::circle(colorFaceImage, cv::Point(x, y), 5, colors[count], -1, CV_AA);
	}

	RectI box;
	//ERROR_CHECK(result->GetFacePointsInColorSpace(FacePointType::FacePointType_Count, &points[0]));
	ERROR_CHECK(result->get_FaceBoundingBoxInColorSpace(&box));
	int width = box.Right - box.Left;
	int height = box.Bottom - box.Top;
	cv::rectangle(colorFaceImage, cv::Rect(box.Left, box.Top, width, height),colors[count]);
	
	Vector4 quaternion;
	double pitch, yaw, roll;

	ERROR_CHECK(result->get_FaceRotationQuaternion(&quaternion));
	quaternion2degree(quaternion, &pitch, &yaw, &roll);

	//output
	Angleoutput[3 * count + 0] = pitch;
	Angleoutput[3 * count + 1] = yaw;
	Angleoutput[3 * count + 2] = roll;

	int offset = 30;
	if (box.Left&&box.Bottom)
	{
		std::string rotation = "Pitch, Yaw, Roll : "
			+ std::to_string(pitch) + ","
			+ std::to_string(yaw) + ","
			+ std::to_string(roll);
		cv::putText(colorFaceImage, rotation, cv::Point(box.Left, box.Bottom + offset), 0, 0.5f, colors[count],2, CV_AA);
	}

	std::array<DetectionResult, FaceProperty::FaceProperty_Count> results;
	ERROR_CHECK(result->GetFaceProperties(FaceProperty::FaceProperty_Count, &results[0]));

	if (box.Left&&box.Bottom)
	{
		std::string l;
		int i = 0;
		for (const DetectionResult r : results)
		{
			l= labels[i];
			std::string property;
			switch (r)
			{
			case DetectionResult::DetectionResult_Yes:
				property = l + " : Yes";
				//Facepropout[8 * count + i] = 0;
				break;
			case DetectionResult::DetectionResult_Maybe:
				property = l + " : Maybe";
				//Facepropout[8 * count + i] = 1;
				break;
			case DetectionResult::DetectionResult_No:
				property = l + " : No";
				//Facepropout[8 * count + i] = 2;
				break;
			case DetectionResult::DetectionResult_Unknown:
				property = l + " : Unknown";
				//Facepropout[8 * count + i] = 3;
				break;
			}
			std::cout << property << std::endl;
			i++;
			offset += 30;
			cv::putText(colorFaceImage, property, cv::Point(box.Left, box.Bottom + offset), 0, 0.f, colors[count],2, CV_AA);
		}
	}
	std::cout << "result_end" << std::endl;
}

void KinectFace::updateBodyFrame()
{
	//std::cout << "updateBodyFrame_start" << std::endl;
	CComPtr<IBodyFrame> bodyFrame;
	bodyFrame = nullptr;
	HRESULT ret = bodyFrameReader->AcquireLatestFrame(&bodyFrame);

	if (FAILED(ret))
	{
		std::cout << "updateBodyFrame_retuen" << std::endl;
		return;
	}

	std::array<CComPtr<IBody>, BODY_COUNT> bodies;
	int number=0;
	ERROR_CHECK(bodyFrame->GetAndRefreshBodyData(BODY_COUNT, &bodies[0]));
	for (int count = 0; count < BODY_COUNT; count++)
	{
		CComPtr<IBody> body = bodies[count];
		BOOLEAN tracked;
		ERROR_CHECK(body->get_IsTracked(&tracked));
		if (!tracked)
		{
			continue;
		}

		UINT64 trackingId;
		ERROR_CHECK(body->get_TrackingId(&trackingId));

		CComPtr<IFaceFrameSource> faceFrameSource;
		ERROR_CHECK(faceFrameReader[count]->get_FaceFrameSource(&faceFrameSource));
		ERROR_CHECK(faceFrameSource->put_TrackingId(trackingId));
		number++;
	}
	m_FaceAngle.data.length(BODY_COUNT * 3);
	m_Faceproperty.data.length(BODY_COUNT * 8);
	//std::cout << "updateBodyFrame_end" << std::endl;
}

void KinectFace::updateFaceFrame()
{
	std::cout << "updateFaceFrame_start" << std::endl;

	for (int i = 0; i < BODY_COUNT; i++)
	{
		//角度用メモリ初期化
		Angleoutput[i * 3 + 0] = 0.0;
		Angleoutput[i * 3 + 1] = 0.0;
		Angleoutput[i * 3 + 2] = 0.0;

		//顔情報用メモリ初期化
		//Facepropout[i * 8 + 0] = 0;
		//Facepropout[i * 8 + 1] = 0;
		//Facepropout[i * 8 + 2] = 0;
		//Facepropout[i * 8 + 3] = 0;
		//Facepropout[i * 8 + 4] = 0;
		//Facepropout[i * 8 + 5] = 0;
		//Facepropout[i * 8 + 6] = 0;
		//Facepropout[i * 8 + 7] = 0;
	}

	for (int count = 0; count < BODY_COUNT; count++)
	{
		CComPtr<IFaceFrame> faceFrame;
		HRESULT ret = faceFrameReader[count]->AcquireLatestFrame(&faceFrame);
		
		if (FAILED(ret))
		{
			continue;
		}

		BOOLEAN tracked;
		ERROR_CHECK(faceFrame->get_IsTrackingIdValid(&tracked));
		if (!tracked)
		{
			continue;
		}

		CComPtr<IFaceFrameResult> faceResult;
		ERROR_CHECK(faceFrame->get_FaceFrameResult(&faceResult));
		if (faceResult != nullptr)
		{
			result(faceResult, count);
			std::cout << "check_point1" << std::endl;
		}

		for (int i = 0; i <BODY_COUNT; i++){
			std::cout << "check_point_____"<<i << std::endl;
			//m_FaceAngle.data[i * 3 + 0] = Angleoutput[i * 3 + 0];
			//m_FaceAngle.data[i * 3 + 0] = Angleoutput[i * 3 + 0];
			//m_FaceAngle.data[i * 3 + 0] = Angleoutput[i * 3 + 0];

			//m_Faceproperty.data[i * 8 + 0] = Facepropout[i * 8 + 0];
			//m_Faceproperty.data[i * 8 + 1] = Facepropout[i * 8 + 1];
			//m_Faceproperty.data[i * 8 + 2] = Facepropout[i * 8 + 2];
			//m_Faceproperty.data[i * 8 + 3] = Facepropout[i * 8 + 3];
			//m_Faceproperty.data[i * 8 + 4] = Facepropout[i * 8 + 4];
			//m_Faceproperty.data[i * 8 + 5] = Facepropout[i * 8 + 5];
			//m_Faceproperty.data[i * 8 + 6] = Facepropout[i * 8 + 6];
			//m_Faceproperty.data[i * 8 + 7] = Facepropout[i * 8 + 7];
		
			m_FaceAngle.data[i * 3 + 0] = Angleoutput[i * 3 + 0];
			m_FaceAngle.data[i * 3 + 1] = Angleoutput[i * 3 + 1];
			m_FaceAngle.data[i * 3 + 2] = Angleoutput[i * 3 + 2];

			//m_Faceproperty.data[i * 8 + 0] = 0;
			//m_Faceproperty.data[i * 8 + 1] = 0;
			//m_Faceproperty.data[i * 8 + 2] = 0;
			//m_Faceproperty.data[i * 8 + 3] = 0;
			//m_Faceproperty.data[i * 8 + 4] = 0;
			//m_Faceproperty.data[i * 8 + 5] = 0;
			//m_Faceproperty.data[i * 8 + 6] = 0;
			//m_Faceproperty.data[i * 8 + 7] = 0;
		}
		std::cout << "check_point2" << std::endl;
		m_FaceAngleOut.write();
		//m_FacepropertyOut.write();

	}
	cv::imshow("ColorFaceImage", colorFaceImage);
	cv::waitKey(1);
	std::cout << "updateFaceFrame_end" << std::endl;
}

RTC::ReturnCode_t KinectFace::onExecute(RTC::UniqueId ec_id)
{

	std::cout << "loop_start" << std::endl;
	//画像の更新
	updateColorFrame();

	//画像の表示
	drawColorFrame();

	//デプス画像更新
	//updateDepthFrame();

	//デプス画像表示
	//drawDepthFrame();

	//体検出
	updateBodyFrame();
	//顔情報の更新出力
	updateFaceFrame();

	std::cout << "loop_end" << std::endl;

	return RTC::RTC_OK;
}

/*
RTC::ReturnCode_t KinectFace::onAborting(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t KinectFace::onError(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*!
 * Kinectへの再接続を試みる
 */

RTC::ReturnCode_t KinectFace::onReset(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}

/*
RTC::ReturnCode_t KinectFace::onStateUpdate(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t KinectFace::onRateChanged(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/



extern "C"
{
 
  void KinectFaceInit(RTC::Manager* manager)
  {
    coil::Properties profile(kinectface_spec);
    manager->registerFactory(profile,
                             RTC::Create<KinectFace>,
                             RTC::Delete<KinectFace>);
  }
  
};


