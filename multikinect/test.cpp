#pragma once
//#include "CMultiKinectManager.h"
#include "ClassThread.h"
#include "process.h"
#include "Windows.h"

#if 0
int main(void)
{
	ClassThread test(2);
	//HANDLE  hThread[2];
	//int hThreadId[2];
	//hThread[0] = (HANDLE)_beginthreadex(0, 0, ClassThread::Process, 0, 0, &hThreadId[0]);		
	//hThread[1] = (HANDLE)_beginthreadex(0, 0, ClassThread::Process, 0, 0, &hThreadId[1]);

	test.Open();

	while (1)
	{
		test.ThreadStart();
	}

	CloseThreadpool(NULL);

	return 0;
}
#endif

#if 0
int main(void)
{
	CMultiKinectManager test;
	//test.Open();
	
	cv::Mat c0(300, 400, CV_8UC4);
	cv::Mat d0(300, 400, CV_32FC1);
	cv::Mat c1(300, 400, CV_8UC4);
	cv::Mat d1(300, 400, CV_32FC1);

	while (1)
	{
		test.Start_0();
		test.Start_1();
	}

	//while (1)
	//{
	//	//test.Process();
	//	//test.Start_0();
	//	//test.Start_1();
	//	test.GetFramesMat(d0, c0, d1, c1);

	//	cv::imshow("sensor0:depth", d0);
	//	cv::imshow("sensor1:depth", d1);
	//	cv::imshow("sensor0:color", c0);
	//	cv::imshow("sensor1:color", c1);
	//	cv::waitKey(3);		
	//}

	//test.Close();

	c0.release();
	d0.release();
	c1.release();
	d1.release();

	return 0;
}
#endif