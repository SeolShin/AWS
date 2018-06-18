#ifndef _CLASSTHREAD_H_

#include <iostream>
#include "CMultiKinectManager.h"

class ClassThread : public CMultiKinectManager
{
public:
	ClassThread(int idx_);
	~ClassThread();

	static unsigned int __stdcall ThreadBody(void *p);
	void ThreadStart();
	void BoostTest();

private:
	int *argList;
	void Process();
	int idx;
	unsigned int MessageHandle;
};

#endif
