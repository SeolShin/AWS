#include "CMultiKinectManager.h"

CMultiKinectManager::CMultiKinectManager()
{
	colorImg_0.create(300, 400, CV_8UC4);
	depthImg_0.create(300, 400, CV_32FC1);
	colorImg_1.create(300, 400, CV_8UC4);
	depthImg_1.create(300, 400, CV_32FC1);

	protonect_shutdown = false;
	
	//InitMultiSensor();
}

CMultiKinectManager::~CMultiKinectManager()
{
	colorImg_0.release();
	depthImg_0.release();
	colorImg_1.release();
	depthImg_1.release();

	//delete[] registered_0;
	//delete[] registered_1;
	//delete[] undistorted_0;
	//delete[] undistorted_1;
	//
	//delete frames_0;
	//delete frames_1;
	//delete listener_0;
	//delete listener_1;
	//delete dev_0;
	//delete dev_1;
}

#if 0
void CMultiKinectManager::InitMultiSensor()
{
	if (freenect2[0].enumerateDevices() == 0)		
		std::cerr << "[FAILED] No device connected" << std::endl;

	if (freenect2[1].enumerateDevices() == 0)
		std::cerr << "[FAILED] No device connected" << std::endl;

	pipeline[0] = new libfreenect2::OpenGLPacketPipeline();
	pipeline[1] = new libfreenect2::OpenGLPacketPipeline();

	//Open();
}

void CMultiKinectManager::Open()
{
	dev_0 = freenect2[0].openDevice(freenect2[0].getDeviceSerialNumber(0), pipeline[0]);
	dev_1 = freenect2[1].openDevice(freenect2[1].getDeviceSerialNumber(1), pipeline[1]);	

	listener_0 = new libfreenect2::SyncMultiFrameListener(libfreenect2::Frame::Color | libfreenect2::Frame::Ir | libfreenect2::Frame::Depth);
	listener_1 = new libfreenect2::SyncMultiFrameListener(libfreenect2::Frame::Color | libfreenect2::Frame::Ir | libfreenect2::Frame::Depth);
	
	frames_0 = new libfreenect2::FrameMap;
	frames_1 = new libfreenect2::FrameMap;

	dev_0->setColorFrameListener(listener_0);
	dev_1->setColorFrameListener(listener_1);
	dev_0->setIrAndDepthFrameListener(listener_0);
	dev_1->setIrAndDepthFrameListener(listener_0);
	dev_0->start();
	dev_1->start();

	std::cout << "[TEST PRINT]device serial: " << dev_0->getSerialNumber() << std::endl;
	std::cout << "[TEST PRINT]device serial: " << dev_1->getSerialNumber() << std::endl;
	std::cout << "[TEST PRINT]device firmware: " << dev_0->getFirmwareVersion() << std::endl;
	std::cout << "[TEST PRINT]device firmware: " << dev_1->getFirmwareVersion() << std::endl;

	registration_0 = new libfreenect2::Registration(dev_0->getIrCameraParams(), dev_0->getColorCameraParams());
	registration_1 = new libfreenect2::Registration(dev_1->getIrCameraParams(), dev_1->getColorCameraParams());

	undistorted_0 = new libfreenect2::Frame(512, 424, 4, NULL);
	undistorted_1 = new libfreenect2::Frame(512, 424, 4, NULL);
	registered_0 = new libfreenect2::Frame(512, 424, 4, NULL);
	registered_1 = new libfreenect2::Frame(512, 424, 4, NULL);	
}
#endif

void CMultiKinectManager::Start_0()
{
	//_beginthread(CMultiKinectManager::HandleThread_0, 0, this);
	//MessageHandle_0 = _beginthreadex(0, 0, &CMultiKinectManager::HandleThread_0, this, 0, &dwThreadID_0);
	hThread[0] = (HANDLE)_beginthreadex(0, 0, &CMultiKinectManager::HandleThread_0, this, 0, &dwThreadID_0);
}

void CMultiKinectManager::Start_1()
{
	//_beginthread(CMultiKinectManager::HandleThread_1, 0, this);
	//MessageHandle_1 = _beginthreadex(0, 0, &CMultiKinectManager::HandleThread_1, this, 0, &dwThreadID_1);
	hThread[1] = (HANDLE)_beginthreadex(0, 0, &CMultiKinectManager::HandleThread_1, this, 0, &dwThreadID_1);
}

unsigned int CMultiKinectManager::HandleThread_0(void *p)
{	
	int n0 = 0;
	CMultiKinectManager* pCMKM_0 = static_cast<CMultiKinectManager*>(p);
	//pCMKM_0->SingleProcess_0();
	pCMKM_0->RunKinectv2(&n0);

	WaitForSingleObject(pCMKM_0->hThread[0], INFINITE);
	CloseHandle(pCMKM_0->hThread[0]);
	return 0;
}

unsigned int CMultiKinectManager::HandleThread_1(void *p)
{
	int n1 = 1;
	CMultiKinectManager* pCMKM_1 = static_cast<CMultiKinectManager*>(p);
	//pCMKM_1->SingleProcess_1();
	pCMKM_1->RunKinectv2(&n1);
		
	WaitForSingleObject(pCMKM_1->hThread[1], INFINITE);
	CloseHandle(pCMKM_1->hThread[1]);
	return 0;
}

uint WINAPI CMultiKinectManager::RunKinectv2(void *arg)
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

#if 0
void CMultiKinectManager::SingleProcess_0(void)
{
	while (1)
	{
		//std::cout << "in_body_0" << std::endl;
		listener_0->waitForNewFrame(*frames_0);
		//if (!listener_0->hasNewFrame()) return;

		libfreenect2::Frame *rgb_0 = (*frames_0)[libfreenect2::Frame::Color];
		libfreenect2::Frame *ir_0 = (*frames_0)[libfreenect2::Frame::Ir];
		libfreenect2::Frame *depth_0 = (*frames_0)[libfreenect2::Frame::Depth];

		registration_0->apply(rgb_0, depth_0, undistorted_0, registered_0);

		cv::Mat colorImg0(rgb_0->height, rgb_0->width, CV_8UC4, rgb_0->data);
		cv::Mat depthImg0(depth_0->height, depth_0->width, CV_32FC1, depth_0->data);
		cv::resize(colorImg0, colorImg_0, colorImg_0.size(), 0, 0, 1);
		cv::resize(depthImg0, depthImg_0, depthImg_0.size(), 0, 0, 1);

		//cv::imshow("sensor0:depth", depthImg_0);
		//cv::imshow("sensor0:color", colorImg_0);
		//cv::waitKey(3);
		std::cout << "in_0" << std::endl;
		if (!depthImg_0.empty() && !colorImg_0.empty())	std::cout << "in_body_0" << std::endl;
		
		colorImg0.release();
		depthImg0.release();
	}
		listener_0->release(*frames_0);	
}

void CMultiKinectManager::SingleProcess_1(void)
{
	while (1)
	{
		listener_1->waitForNewFrame(*frames_1);
		//if (!listener_1->hasNewFrame()) return;

		libfreenect2::Frame *rgb_1 = (*frames_1)[libfreenect2::Frame::Color];
		libfreenect2::Frame *ir_1 = (*frames_1)[libfreenect2::Frame::Ir];
		libfreenect2::Frame *depth_1 = (*frames_1)[libfreenect2::Frame::Depth];

		registration_1->apply(rgb_1, depth_1, undistorted_1, registered_1);

		cv::Mat colorImg1(rgb_1->height, rgb_1->width, CV_8UC4, rgb_1->data);
		cv::Mat depthImg1(depth_1->height, depth_1->width, CV_32FC1, depth_1->data);
		cv::resize(colorImg1, colorImg_1, colorImg_1.size(), 0, 0, 1);
		cv::resize(depthImg1, depthImg_1, depthImg_1.size(), 0, 0, 1);

		//cv::imshow("sensor1:depth", depthImg_1);
		//cv::imshow("sensor1:color", colorImg_1);
		//cv::waitKey(3);
		std::cout << "in_1" << std::endl;
		if (!depthImg_1.empty() && !colorImg_1.empty())	std::cout << "in_body_1" << std::endl;

		colorImg1.release();
		depthImg1.release();
	}
		listener_1->release(*frames_1);	
}

void CMultiKinectManager::Process()
{
	listener_0->waitForNewFrame(*frames_0);	
	if (!listener_0->hasNewFrame()) return;

	listener_1->waitForNewFrame(*frames_1);
	if (!listener_1->hasNewFrame()) return;	

	libfreenect2::Frame *rgb_0 = (*frames_0)[libfreenect2::Frame::Color];
	libfreenect2::Frame *ir_0 = (*frames_0)[libfreenect2::Frame::Ir];
	libfreenect2::Frame *depth_0 = (*frames_0)[libfreenect2::Frame::Depth];
		
	libfreenect2::Frame *rgb_1 = (*frames_1)[libfreenect2::Frame::Color];
	libfreenect2::Frame *ir_1 = (*frames_1)[libfreenect2::Frame::Ir];
	libfreenect2::Frame *depth_1 = (*frames_1)[libfreenect2::Frame::Depth];

	registration_0->apply(rgb_0, depth_0, undistorted_0, registered_0);
	registration_1->apply(rgb_1, depth_1, undistorted_1, registered_1);	
	
	cv::Mat colorImg0(rgb_0->height, rgb_0->width, CV_8UC4, rgb_0->data);
	cv::Mat depthImg0(depth_0->height, depth_0->width, CV_32FC1, depth_0->data);
	cv::resize(colorImg0, colorImg_0, colorImg_0.size(), 0, 0, 1);
	cv::resize(depthImg0, depthImg_0, depthImg_0.size(), 0, 0, 1);
	//cv::imshow("test_d0", depthImg_0);
	//cv::imshow("test_c0", colorImg_0);
	//cv::waitKey(0);

	cv::Mat colorImg1(rgb_1->height, rgb_1->width, CV_8UC4, rgb_1->data);
	cv::Mat depthImg1(depth_1->height, depth_1->width, CV_32FC1, depth_1->data);
	cv::resize(colorImg1, colorImg_1, colorImg_1.size(), 0, 0, 1);
	cv::resize(depthImg1, depthImg_1, depthImg_1.size(), 0, 0, 1);
	//cv::imshow("test_d1", depthImg_1);
	//cv::imshow("test_c1", colorImg_1);
	//cv::waitKey(0);

	listener_0->release(*frames_0);
	listener_1->release(*frames_1);

	colorImg0.release();
	colorImg1.release();
	depthImg0.release();
	depthImg1.release();
}
#endif

void CMultiKinectManager::GetFramesMat(cv::Mat &src_depth_0, cv::Mat &src_color_0, cv::Mat &src_depth_1, cv::Mat &src_color_1)
{
	depthImg_0.copyTo(src_depth_0);
	colorImg_0.copyTo(src_color_0);
	depthImg_1.copyTo(src_depth_1);
	colorImg_1.copyTo(src_color_1);
}

#if 0
void CMultiKinectManager::Close()
{
	CloseHandle(this->hThread[0]);
	CloseHandle(this->hThread[1]);

	delete registration_0;
	delete registration_1;

	dev_0->stop();
	dev_0->close();
	dev_1->stop();
	dev_1->close();
}
#endif