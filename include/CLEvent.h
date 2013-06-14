#ifndef CLEVENT_H
#define CLEVENT_H

#include <string>
#include "CLStatus.h"
#include "CLMutex.h"
#include "CLConditionVariable.h"

using namespace std;

struct SLEventInfo;

/*
Ĭ������£�����һ����ʼ���źţ��Զ������źŵ��¼������ڻ���һ���ȴ��̣߳�
�ڹ��캯���У���bSemaphore������Ϊtrue������ü���Set�����ɻ��Ѽ��λ���߳�
*/
class CLEvent
{
public: 
	/*
	���캯����������������ʱ�����׳��ַ��������쳣
	*/
	CLEvent( );
	explicit CLEvent(bool bSemaphore);
	explicit CLEvent(const char *pstrEventName);
	CLEvent(const char *pstrEventName, bool bSemaphore);
	virtual ~CLEvent();

public: 
	CLStatus Set();
	CLStatus Wait();

private:
	CLEvent(const CLEvent&);
	CLEvent& operator=(const CLEvent&);

private:
	CLMutex m_Mutex;
	CLConditionVariable m_Cond;
	SLEventInfo *m_pEventInfo;
	bool m_bNeededDestroy;
	string m_strEventName;
};

#endif