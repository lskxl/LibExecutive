#ifndef CLEXECUTIVEFUNCTIONFORMSGLOOP_H
#define CLEXECUTIVEFUNCTIONFORMSGLOOP_H

#include "CLStatus.h"
#include "CLExecutiveFunctionProvider.h"

class CLMessageLoopManager;

/*
��������߳̽�����Ϣѭ��
���캯���У�pMsgLoopManager�Ӷ��з��䣬���ص���delete
*/
class CLExecutiveFunctionForMsgLoop : public CLExecutiveFunctionProvider
{
public:
	explicit CLExecutiveFunctionForMsgLoop(CLMessageLoopManager *pMsgLoopManager);
	virtual ~CLExecutiveFunctionForMsgLoop();

	virtual CLStatus RunExecutiveFunction(void* pContext);

private:
	CLExecutiveFunctionForMsgLoop(const CLExecutiveFunctionForMsgLoop&);
	CLExecutiveFunctionForMsgLoop& operator=(const CLExecutiveFunctionForMsgLoop&);

private:
	CLMessageLoopManager *m_pMsgLoopManager;
};

#endif
