#ifndef CLMESSAGEOBSERVER_H
#define CLMESSAGEOBSERVER_H

#include "CLStatus.h"

class CLMessageLoopManager;

class CLMessageObserver
{
public:
	CLMessageObserver();
	virtual ~CLMessageObserver();

	/*
	��Ҫ�ڳ�ʼ���У������Ϣ��������ע��
	*/
	virtual CLStatus Initialize(CLMessageLoopManager *pMessageLoop, void* pContext) = 0;

private:
	CLMessageObserver(const CLMessageObserver&);
	CLMessageObserver& operator=(const CLMessageObserver&);
};

#endif
