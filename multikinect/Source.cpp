#include "libfreenect2\libfreenect2.hpp"
#include "libfreenect2\registration.h"
#include "libfreenect2\packet_pipeline.h"
#include "libfreenect2\logger.h"
#include "libfreenect2\frame_listener.hpp"
#include "libfreenect2\frame_listener_impl.h"

#include "viewer.h"

#include "opencv2\opencv.hpp"
#include <string>

#include "Windows.h"
#include "process.h"

//libfreenect2::Freenect2Device *devtopause;
//libfreenect2::Freenect2Device *devtopause1;
bool protonect_shutdown = false;

using namespace cv;
using namespace std;

#if 0
libfreenect2::PacketPipeline *pipeline = 0;
libfreenect2::PacketPipeline *pipeline1 = 0;

uint WINAPI RunKinectv2(void *arg)
{
	int idx = (int)arg;
	libfreenect2::Freenect2 freenect2;

	if (freenect2.enumerateDevices() == 0)
		std::cout << "[FAILED] No device connected" << std::endl;
	
	//libfreenect2::PacketPipeline *pipeline = 0;
	//pipeline = new libfreenect2::OpenGLPacketPipeline();

	//if (idx == 0)
	//{
	//	pipeline  = new libfreenect2::OpenGLPacketPipeline();
	//}
	//else if (idx == 1)
	//{
	//	pipeline = new libfreenect2::OpenCLPacketPipeline();
	//}
	
	libfreenect2::Freenect2Device *dev = 0;
	
	if (idx == 0)
	{
		if (pipeline)			
			dev = freenect2.openDevice(freenect2.getDeviceSerialNumber(idx), pipeline);
		else
			dev = freenect2.openDevice(freenect2.getDeviceSerialNumber(idx));
	}

	if (idx == 1)
	{
		if (pipeline1)
			dev = freenect2.openDevice(freenect2.getDeviceSerialNumber(idx), pipeline1);
		else
			dev = freenect2.openDevice(freenect2.getDeviceSerialNumber(idx));
	}

	libfreenect2::SyncMultiFrameListener listener(libfreenect2::Frame::Color | libfreenect2::Frame::Ir | libfreenect2::Frame::Depth);
	libfreenect2::FrameMap frames;
	dev->setColorFrameListener(&listener);
	dev->setIrAndDepthFrameListener(&listener);

	dev->start();

	std::cout << "device serial: " << dev->getSerialNumber() << std::endl;
	std::cout << "device firmware: " << dev->getFirmwareVersion() << std::endl;

	libfreenect2::Registration* registration = new libfreenect2::Registration(dev->getIrCameraParams(), dev->getColorCameraParams());
	libfreenect2::Frame undistorted(512, 424, 4), registered(512, 424, 4);

	size_t framecount = 0;

	std::string win_idx = "color";
	std::string win_idx1 = "depth";

	if (idx == 0)
	{
		win_idx.append("0");
		win_idx1.append("0");
	}
	else
	{
		win_idx.append("1");
		win_idx1.append("1");
	}

	while (!protonect_shutdown)
	{
		listener.waitForNewFrame(frames);
		libfreenect2::Frame *rgb = frames[libfreenect2::Frame::Color];
		libfreenect2::Frame *ir = frames[libfreenect2::Frame::Ir];
		libfreenect2::Frame *depth = frames[libfreenect2::Frame::Depth];

		registration->apply(rgb, depth, &undistorted, &registered);
		framecount++;

		cv::Mat colorImg(rgb->height, rgb->width, CV_8UC4, rgb->data);
		cv::Mat depthImg(depth->height, depth->width, CV_32FC1, depth->data);

		cv::namedWindow(win_idx, 1);
		cv::namedWindow(win_idx1, 1);
		cv::imshow(win_idx, colorImg);
		cv::imshow(win_idx1, depthImg);
		cv::waitKey(30);

		listener.release(frames);
	}

	delete registration;

	dev->stop();
	dev->close();

	return 0;
}
#endif

uint WINAPI RunKinectv2(void *arg)
{
	int idx = (int)arg;
	libfreenect2::Freenect2 freenect2;

	if (freenect2.enumerateDevices() == 0)
		std::cout << "[FAILED] No device connected" << std::endl;

	libfreenect2::PacketPipeline *pipeline = new libfreenect2::OpenGLPacketPipeline();
	//libfreenect2::PacketPipeline *pipeline = new libfreenect2::CpuPacketPipeline();
			
	libfreenect2::Freenect2Device *dev = freenect2.openDevice(freenect2.getDeviceSerialNumber(idx), pipeline);
	
	libfreenect2::SyncMultiFrameListener listener(libfreenect2::Frame::Color | libfreenect2::Frame::Ir | libfreenect2::Frame::Depth);
	libfreenect2::FrameMap frames;
	dev->setColorFrameListener(&listener);
	dev->setIrAndDepthFrameListener(&listener);

	dev->start();

	std::cout << "device serial: " << dev->getSerialNumber() << std::endl;
	std::cout << "device firmware: " << dev->getFirmwareVersion() << std::endl;

	libfreenect2::Registration* registration = new libfreenect2::Registration(dev->getIrCameraParams(), dev->getColorCameraParams());
	libfreenect2::Frame undistorted(512, 424, 4), registered(512, 424, 4);

	size_t framecount = 0;

	std::string win_idx = "color";
	std::string win_idx1 = "depth";

	if (idx == 0)
	{
		win_idx.append("0");
		win_idx1.append("0");
	}
	else
	{
		win_idx.append("1");
		win_idx1.append("1");
	}

	cv::Mat colorImg_(300, 400, CV_8UC4);
	cv::Mat depthImg_(300, 400, CV_32FC1);

	while (!protonect_shutdown)
	{
		listener.waitForNewFrame(frames);
		libfreenect2::Frame *rgb = frames[libfreenect2::Frame::Color];
		libfreenect2::Frame *ir = frames[libfreenect2::Frame::Ir];
		libfreenect2::Frame *depth = frames[libfreenect2::Frame::Depth];

		registration->apply(rgb, depth, &undistorted, &registered);
		framecount++;

		cv::Mat colorImg(rgb->height, rgb->width, CV_8UC4, rgb->data);
		cv::Mat depthImg(depth->height, depth->width, CV_32FC1, depth->data);
		cv::resize(colorImg, colorImg_, colorImg_.size(), 0, 0, 1);
		cv::resize(depthImg, depthImg_, depthImg_.size(), 0, 0, 1);

		cv::namedWindow(win_idx, 1);
		cv::namedWindow(win_idx1, 1);
		cv::imshow(win_idx, colorImg_);
		cv::imshow(win_idx1, depthImg_);
		cv::waitKey(30);

		listener.release(frames);
	}

	delete registration;

	//pipeline->~PacketPipeline();
	
	dev->stop();
	dev->close();


	return 0;
}

//int main(void)
//{
//	libfreenect2::Freenect2 freenect2;
//	libfreenect2::Freenect2 freenect2_1;
//	libfreenect2::Freenect2Device *dev = 0;
//	libfreenect2::Freenect2Device *dev1 = 0;
//
//	libfreenect2::PacketPipeline *pipeline = 0;
//	libfreenect2::PacketPipeline *pipeline1 = 0;
//
//	std::string serial1 = "020201243547";
//	std::string serial = "004868251147";
//
//	//serial = std::string("020201243547");
//	//serial1 = std::string("004868251147");
//	
//	bool viewer_enabled = true;
//	bool enable_rgb = true;
//	bool enable_depth = true;
//	int deviceId = -1;
//	size_t framemax = -1;
//
//	if (!pipeline)		
//		pipeline = new libfreenect2::OpenGLPacketPipeline();
//
//	if (!pipeline1)
//		pipeline1 = new libfreenect2::OpenGLPacketPipeline();
//	
//	if (freenect2.enumerateDevices() == 0)
//		std::cout << "[FAILED] No device connected" << std::endl;
//	if (freenect2_1.enumerateDevices() == 0)
//		std::cout << "[FAILED] No device connected" << std::endl;
//
//	//if (serial == "")
//	//{
//	//	serial = freenect2.getDeviceSerialNumber(0);
//	//	std::cout << "[Device 1] Serial Number: " << serial << std::endl;
//	//}
//
//	//if (serial1 == "")
//	//{
//	//	serial1 = freenect2.getDeviceSerialNumber(1);
//	//	std::cout << "[Device 0] Serial Number: " << serial1 << std::endl;
//	//}
//
//	//if (dev)
//		//dev = freenect2.openDefaultDevice();
//
//	if (pipeline)
//		dev = freenect2.openDevice(serial, pipeline);
//	//else
//		//dev = freenect2.openDevice(serial);
//
//	if (pipeline1)
//		dev1 = freenect2.openDevice(serial1, pipeline1);
//	//else
//		//dev1 = freenect2.openDevice(serial1);
//	
//	//ERROR CHECK
//	if (dev == 0)
//		std::cout << "[FAILED] failure opening device 0" << std::endl;
//	if (dev1 == 0)
//		std::cout << "[FAILED] failure opening device 1" << std::endl;
//
//	//devtopause = dev;
//	//devtopause1 = dev1;
//
//	libfreenect2::SyncMultiFrameListener listener(libfreenect2::Frame::Color | libfreenect2::Frame::Ir | libfreenect2::Frame::Depth);
//	libfreenect2::FrameMap frames;
//	dev->setColorFrameListener(&listener);
//	dev->setIrAndDepthFrameListener(&listener);
//	libfreenect2::SyncMultiFrameListener listener1(libfreenect2::Frame::Color | libfreenect2::Frame::Ir | libfreenect2::Frame::Depth);
//	libfreenect2::FrameMap frames1;
//	dev1->setColorFrameListener(&listener1);
//	dev1->setIrAndDepthFrameListener(&listener1);
//
//	dev->start();
//	dev1->start();
//
//	std::cout << "device serial: " << dev->getSerialNumber() << std::endl;
//	std::cout << "device firmware: " << dev->getFirmwareVersion() << std::endl;
//	std::cout << "device serial: " << dev1->getSerialNumber() << std::endl;
//	std::cout << "device firmware: " << dev1->getFirmwareVersion() << std::endl;
//
//	libfreenect2::Registration* registration = new libfreenect2::Registration(dev->getIrCameraParams(), dev->getColorCameraParams());
//	libfreenect2::Frame undistorted(512, 424, 4), registered(512, 424, 4);
//	libfreenect2::Registration* registration1 = new libfreenect2::Registration(dev1->getIrCameraParams(), dev1->getColorCameraParams());
//	libfreenect2::Frame undistorted1(512, 424, 4), registered1(512, 424, 4);
//
//	size_t framecount = 0;
//	size_t framecount1 = 0;
//
//	while (!protonect_shutdown)
//	{
//		listener.waitForNewFrame(frames);		
//		//listener1.waitForNewFrame(frames1);
//
//		libfreenect2::Frame *rgb = frames[libfreenect2::Frame::Color];
//		libfreenect2::Frame *ir = frames[libfreenect2::Frame::Ir];
//		libfreenect2::Frame *depth = frames[libfreenect2::Frame::Depth];
//		//libfreenect2::Frame *rgb1 = frames1[libfreenect2::Frame::Color];
//		//libfreenect2::Frame *ir1 = frames1[libfreenect2::Frame::Ir];
//		//libfreenect2::Frame *depth1 = frames1[libfreenect2::Frame::Depth];
//
//		registration->apply(rgb, depth, &undistorted, &registered);
//		//registration1->apply(rgb1, depth1, &undistorted1, &registered1);
//		
//		if (!frames.empty())framecount++;
//		//if (!frames1.empty())framecount1++;
//
//		std::cout << framecount << " " << framecount1 << std::endl;
//	
//		//Sleep(100);
//		cv::Mat colorImg(rgb->height, rgb->width, CV_8UC4, rgb->data);
//		cv::Mat depthImg(depth->height, depth->width, CV_32FC1, depth->data);
//		//cv::Mat colorImg1(rgb1->height, rgb1->width, CV_8UC4, rgb1->data);
//		//cv::Mat depthImg1(depth1->height, depth1->width, CV_32FC1, depth1->data);
//
//		cv::namedWindow("color1", 1);
//		cv::namedWindow("depth1", 1);
//		cv::namedWindow("color", 1);
//		cv::namedWindow("depth", 1);
//
//		cv::imshow("color", colorImg);
//		cv::imshow("depth", depthImg);		
//		//cv::imshow("color1", colorImg1);
//		//cv::imshow("depth1", depthImg1);
//		
//		cv::waitKey(0);
//
//		listener.release(frames);
//		listener1.release(frames1);
//	}
//
//	delete registration;
//	delete registration1;
//
//	dev->stop();
//	dev1->stop();
//	dev->close();
//	dev1->close();
//
//	return 0;
//}

//ORIGINAL MAIN ENTRY
//int main(void)
//{
//	//if (!pipeline)
//	//	pipeline = new libfreenect2::OpenGLPacketPipeline();
//
//	//if (!pipeline1)
//	//	pipeline1 = new libfreenect2::OpenGLPacketPipeline();
//
//	HANDLE hThread[2];
//	UINT dwThreadID[2];	
//	hThread[0] = (HANDLE)_beginthreadex(NULL, 0, RunKinectv2, (void*)0, 0, &dwThreadID[0]);
//	hThread[1] = (HANDLE)_beginthreadex(NULL, 0, RunKinectv2, (void*)1, 0, &dwThreadID[1]);
//	WaitForSingleObject(hThread[0], INFINITE);
//	WaitForSingleObject(hThread[1], INFINITE);
//
//	CloseHandle(hThread[0]);
//	CloseHandle(hThread[1]);
//	
//	getchar();
//
//	return 0;	
//}