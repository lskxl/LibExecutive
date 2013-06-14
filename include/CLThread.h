#ifndef CLTHREAD_H
#define CLTHREAD_H

#include <pthread.h>
#include "CLExecutive.h"
#include "CLStatus.h"
#include "CLEvent.h"

/*
CLThread������һ���̣߳��������������ڣ����̵߳�������

����������Ӷ��з��䣬�Ҳ��ص���delete�ͷ��ڴ�

���캯���е�bWaitForDeath����ָʾ�Ƿ���Ҫ�ȴ����߳̽�����Ĭ��
����ǲ���Ҫ�ġ�����Ҫ������Ϊtrue�������豣֤���һ��Ҫ����
WaitForDeath������������Դ�޷��ͷ�

Run��WaitForDeath����������سɹ�����ֻ�ܵ���һ�Ρ�
Run��������ʧ�ܣ�������CLThread������������ˣ�����Run������ֻ�ܴ���һ���¶���
*/
class CLThread : public CLExecutive
{
public:
	/*
	�����������������ʱ�����׳��ַ��������쳣
	*pExecutiveFunctionProvider���������ǴӶ��з����
	*/
	explicit CLThread(CLExecutiveFunctionProvider *pExecutiveFunctionProvider);
	CLThread(CLExecutiveFunctionProvider *pExecutiveFunctionProvider, bool bWaitForDeath);
	virtual ~CLThread();

	virtual CLStatus Run(void *pContext = 0);

	virtual CLStatus WaitForDeath();

private:
	static void* StartFunctionOfThread(void *pContext);

private:
	void *m_pContext;
	
	CLEvent m_EventForWaitingForNewThread;
	CLEvent m_EventForWaitingForOldThread;

	pthread_t m_ThreadID; 

	bool m_bWaitForDeath;
	bool m_bThreadCreated;
};

#endif
