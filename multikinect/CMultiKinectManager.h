#ifndef _CMULTIKINECT_MANAGER_H_
#define _CMULTIKINECT_MANAGER_H_

#include "libfreenect2\libfreenect2.hpp"
#include "libfreenect2\registration.h"
#include "libfreenect2\packet_pipeline.h"
#include "libfreenect2\logger.h"
#include "libfreenect2\frame_listener.hpp"
#include "libfreenect2\frame_listener_impl.h"

#include "opencv2\opencv.hpp"
#include "Windows.h"
#include "process.h"

class CMultiKinectManager
{
public:
	CMultiKinectManager();
	~CMultiKinectManager();

public:
	void Open();
	void Process(); //debug
	void Close();
	void GetFramesMat(cv::Mat &src_depth_0, cv::Mat &src_color_0, cv::Mat &src_depth_1, cv::Mat &src_color_1);	
	
	void SingleProcess_0(void);
	void SingleProcess_1(void);
	void Start_0();
	static unsigned int __stdcall HandleThread_0(void *p);
	void Start_1();
	static unsigned int __stdcall HandleThread_1(void *p);
	uint WINAPI RunKinectv2(void *arg);

private:
	void InitMultiSensor();	
	cv::Mat colorImg_0, colorImg_1;
	cv::Mat depthImg_0, depthImg_1;
	
	//libfreenect2::Freenect2 freenect2[2];
	//libfreenect2::PacketPipeline *pipeline[2];
	//libfreenect2::Freenect2Device *dev_0;
	//libfreenect2::Freenect2Device *dev_1;
	//libfreenect2::SyncMultiFrameListener *listener_0;
	//libfreenect2::SyncMultiFrameListener *listener_1;
	//libfreenect2::FrameMap *frames_0;
	//libfreenect2::FrameMap *frames_1;

	//libfreenect2::Registration* registration_0;
	//libfreenect2::Registration* registration_1;

	//libfreenect2::Frame *undistorted_0; 
	//libfreenect2::Frame *undistorted_1;
	//libfreenect2::Frame *registered_0;
	//libfreenect2::Frame *registered_1;

	bool protonect_shutdown;
	HANDLE hThread[2];
	UINT dwThreadID_0;
	UINT dwThreadID_1;
	unsigned int MessageHandle_0;
	unsigned int MessageHandle_1;
};
#endif