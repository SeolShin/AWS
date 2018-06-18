#include "CMultiSensor.h"
#include <tuple>

CMultisensor::CMultisensor()
{
	protonect_shutdown = false;
}

CMultisensor::~CMultisensor()
{

}

#if 0
unsigned int __stdcall CMultisensor::ProcessTest(int arg)
{
	while (1)
	{
		std::cout << arg << std::endl;
	}

	return 0;
}

void CMultisensor::Pre(int arg)
{

	int idx = arg;
	
	if (freenect2.enumerateDevices() == 0)
		std::cout << "[FAILED] No device connected" << std::endl;
	if (freenect2_.enumerateDevices() == 0)
		std::cout << "[FAILED] No device connected" << std::endl;
	
	pipeline = new libfreenect2::OpenGLPacketPipeline();
	pipeline_ = new libfreenect2::OpenGLPacketPipeline();
	//libfreenect2::PacketPipeline *pipeline = new libfreenect2::CpuPacketPipeline();
	//libfreenect2::PacketPipeline *pipeline = new libfreenect2::OpenCLPacketPipeline();
	//libfreenect2::PacketPipeline *pipeline = new libfreenect2::DumpPacketPipeline();

	dev = freenect2.openDevice(freenect2.getDeviceSerialNumber(idx), pipeline);
	dev_ = freenect2_.openDevice(freenect2_.getDeviceSerialNumber(idx), pipeline_);

	//libfreenect2::SyncMultiFrameListener listener(libfreenect2::Frame::Color | libfreenect2::Frame::Ir | libfreenect2::Frame::Depth);
	listener = new libfreenect2::SyncMultiFrameListener(libfreenect2::Frame::Color | libfreenect2::Frame::Ir | libfreenect2::Frame::Depth);
	listener_ = new libfreenect2::SyncMultiFrameListener(libfreenect2::Frame::Color | libfreenect2::Frame::Ir | libfreenect2::Frame::Depth);
	dev->setColorFrameListener(listener);
	dev_->setColorFrameListener(listener_);
	dev->setIrAndDepthFrameListener(listener);
	dev_->setIrAndDepthFrameListener(listener_);

	dev->start();
	dev_->start();	
}
#endif

#if 0
unsigned int __stdcall CMultisensor::Process(void *p, int arg)
{
	CMultisensor* ptr = static_cast<CMultisensor*>(p);

	if (arg == 0)
	{
		libfreenect2::Registration* registration = new libfreenect2::Registration(dev->getIrCameraParams(), dev->getColorCameraParams());
		libfreenect2::Frame undistorted(512, 424, 4), registered(512, 424, 4);

		size_t framecount = 0;
		int idx = arg;

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
			ptr->listener->waitForNewFrame(frames);
			//listener.waitForNewFrame(frames);
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

			ptr->listener->release(frames);
		}

		delete registration;

		//pipeline->~PacketPipeline();

		dev->stop();
		dev->close();
	}
	else if (arg == 1)
	{
		libfreenect2::Registration* registration_ = new libfreenect2::Registration(dev_->getIrCameraParams(), dev_->getColorCameraParams());
		libfreenect2::Frame undistorted(512, 424, 4), registered(512, 424, 4);

		size_t framecount = 0;
		int idx = arg;

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
			ptr->listener_->waitForNewFrame(frames_);
			libfreenect2::Frame *rgb = frames[libfreenect2::Frame::Color];
			libfreenect2::Frame *ir = frames[libfreenect2::Frame::Ir];
			libfreenect2::Frame *depth = frames[libfreenect2::Frame::Depth];

			registration_->apply(rgb, depth, &undistorted, &registered);
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

			ptr->listener->release(frames_);
		}

		delete registration_;

		//pipeline->~PacketPipeline();

		dev_->stop();
		dev_->close();
	}

	return 0;
}
#endif

#if 1
unsigned int __stdcall CMultisensor::Process_(int arg)
{
	int idx = arg;
	libfreenect2::Freenect2 freenect2;

	if (freenect2.enumerateDevices() == 0)
		std::cout << "[FAILED] No device connected" << std::endl;

	libfreenect2::PacketPipeline *pipeline = new libfreenect2::OpenGLPacketPipeline();
	//libfreenect2::PacketPipeline *pipeline = new libfreenect2::CpuPacketPipeline();
	//libfreenect2::PacketPipeline *pipeline = new libfreenect2::OpenCLPacketPipeline();
	//libfreenect2::PacketPipeline *pipeline = new libfreenect2::DumpPacketPipeline();

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
#endif

//unsigned int __stdcall CMultisensor::ThreadBody(std::tuple<void*, int> arg)
unsigned int __stdcall CMultisensor::ThreadBody(void* p)
{		
	//int n_s = std::get<1>(arg);
	//CMultisensor* pCMKM_0 = static_cast<CMultisensor*>(std::get<0>(arg));

	DWORD t_id = (DWORD)GetCurrentThreadId();
	std::cout << t_id << std::endl;

	CMultisensor* pCMKM_0 = static_cast<CMultisensor*>(p);

	int s_id;

	if (pCMKM_0->dwThreadID[0] == t_id) s_id = 0;
	else if (pCMKM_0->dwThreadID[1] == t_id) s_id = 1;

	pCMKM_0->Process_(s_id);

	return 0; 
}   

void CMultisensor::ThreadStart()
{
	//typedef std::tuple<void*, int> ids;
	//ids s_0(this, dwThreadID_0);
	//ids s_1(this, dwThreadID_1);

	//auto argList0 = s_0;
	//auto argList1 = s_1;

	//hThread[0] = (HANDLE)_beginthreadex(0, 0, &CMultisensor::ThreadBody, this, 0, &dwThreadID_0);
	//hThread[1] = (HANDLE)_beginthreadex(0, 0, &CMultisensor::ThreadBody, this, 0, &dwThreadID_1);
	hThread[0] = (HANDLE)_beginthreadex(0, 0, &CMultisensor::ThreadBody, this, 0, &dwThreadID[0]);
	hThread[1] = (HANDLE)_beginthreadex(0, 0, &CMultisensor::ThreadBody, this, 0, &dwThreadID[1]);

	WaitForSingleObject(this->hThread[0], INFINITE);
	WaitForSingleObject(this->hThread[1], INFINITE);
	CloseHandle(this->hThread[0]);
	CloseHandle(this->hThread[1]);

	std::cout << "in_start" << std::endl;
}