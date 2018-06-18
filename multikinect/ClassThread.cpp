#include "ClassThread.h"

#include "Windows.h"
#include "process.h"

ClassThread::ClassThread(int idx_)
{
	argList = new int[idx_];	
	idx = idx_;
	std::cout << "in_create" << std::endl;	
}

ClassThread::~ClassThread()
{
	delete argList;
}

void ClassThread::ThreadStart()
{
	//_beginthread(ClassThread::ThreadBody, 0, this);
	MessageHandle = _beginthreadex(0, 0, &ClassThread::ThreadBody, this, 0, 0);
	std::cout << "in_start" << std::endl;
}

unsigned int ClassThread::ThreadBody(void *p)
{
	ClassThread* pCT = static_cast<ClassThread*>(p);
	pCT->Process();

	std::cout << "in_body" << std::endl;
	
	return 0;
}

void ClassThread::Process()
{
	for (;;)
	{
		//argList[0] = TRUE;
		std::cout << "in_process" << std::endl;
	}

	//argList[1] = FALSE;

	return;
}

void ClassThread::BoostTest()
{

}