#ifndef _KETI_CMULTISENSOR_H_
#define _KETI_CMULTISENSOR_H_

#include "libfreenect2\libfreenect2.hpp"
#include "libfreenect2\registration.h"
#include "libfreenect2\packet_pipeline.h"
#include "libfreenect2\logger.h"
#include "libfreenect2\frame_listener.hpp"
#include "libfreenect2\frame_listener_impl.h"

#include "Windows.h"
#include "process.h"
#include <iostream>
#include "opencv2\opencv.hpp"

class _declspec(dllexport) CMultisensor
{
public:
	CMultisensor();
	~CMultisensor();

public:	
	unsigned int  __stdcall Process_(int arg);	
	void ThreadStart();	
	static unsigned int __stdcall ThreadBody(void* p);	

private:
	bool protonect_shutdown;
	HANDLE hThread[2];
	unsigned int dwThreadID[2];
};
#endif