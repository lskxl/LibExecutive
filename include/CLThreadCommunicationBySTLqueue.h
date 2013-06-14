#ifndef CLThreadCommunicationBySTLqueue_H
#define CLThreadCommunicationBySTLqueue_H

#include "CLExecutiveCommunication.h"
#include "CLStatus.h"

class CLMessage;
class CLMessageQueueBySTLqueue;

class CLThreadCommunicationBySTLqueue : public CLExecutiveCommunication
{
public:
	/*
	pMsgQueue����Ӷ��з��䣬�Ҳ�����ʾ����delete
	*/
	CLThreadCommunicationBySTLqueue(CLMessageQueueBySTLqueue *pMsgQueue);
	virtual ~CLThreadCommunicationBySTLqueue();

	virtual CLStatus PostExecutiveMessage(CLMessage *pMessage);

private:
	CLThreadCommunicationBySTLqueue(const CLThreadCommunicationBySTLqueue&);
	CLThreadCommunicationBySTLqueue& operator=(const CLThreadCommunicationBySTLqueue&);

private:
	CLMessageQueueBySTLqueue*m_pMsgQueue;
};

#endif