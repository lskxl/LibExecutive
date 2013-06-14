#ifndef CLMessageQueueBySTLqueue_H
#define CLMessageQueueBySTLqueue_H

#include <queue>
#include "CLStatus.h"
#include "CLMutex.h"
#include "CLEvent.h"

class CLMessage;

/*
�������̰߳�ȫ��
��Ҫ��CLMsgLoopManagerForMsgQueue ���ʹ�ã�����������Ҫ�Ӷ��з��䣬�Ҳ��õ���delete
*/
class CLMessageQueueBySTLqueue
{
public:
	/*
	���캯����������������ʱ�����׳��ַ������͵��쳣
	*/
	CLMessageQueueBySTLqueue();
	virtual ~CLMessageQueueBySTLqueue();

public:
	CLStatus PushMessage(CLMessage * pMessage);
	CLMessage* GetMessage();

private:
	CLStatus Push(CLMessage * pMessage);
	CLMessage* Pop();

private:
	CLMessageQueueBySTLqueue(const CLMessageQueueBySTLqueue&);
	CLMessageQueueBySTLqueue& operator=(const CLMessageQueueBySTLqueue&);

private:
	std::queue<CLMessage*> m_MessageQueue;
	CLMutex m_Mutex;
	CLEvent m_Event;
};

#endif