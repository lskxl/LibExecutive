#ifndef CLMessageLoopManager_H
#define CLMessageLoopManager_H

#include <map>
#include "CLStatus.h"

class CLMessageObserver;
class CLMessage;
class CLExecutiveInitialFinishedNotifier;

typedef CLStatus (CLMessageObserver::*CallBackForMessageLoop)(CLMessage *);

#define QUIT_MESSAGE_LOOP 1

struct SLExecutiveInitialParameter
{
	void *pContext;
	CLExecutiveInitialFinishedNotifier *pNotifier;
};

/*
�ü̳���ϵ����Ӧ�Ӷ��з��䣬������ʾ����delete
����Ϣ��������CLStatus�ķ��ض����У�������QUIT_MESSAGE_LOOP����ֵ���������˳���Ϣѭ��
*/
class CLMessageLoopManager
{
public:
	/*
	pMessageObserverӦ�Ӷ��з��䣬�Ҳ��õ���delete
	*/
	CLMessageLoopManager(CLMessageObserver *pMessageObserver);
	virtual ~CLMessageLoopManager();

	virtual CLStatus EnterMessageLoop(void *pContext);

public:	
	/*
	MessageObserver���ô˺���������ע��������Ϣ������
	*/
	virtual CLStatus Register(unsigned long lMsgID, CallBackForMessageLoop pMsgProcessFunction);

protected:
	/*
	��ʼ���뷴��ʼ����Ϣѭ������Ҫ��֤��Ϣ�����Ѿ��������
	*/
	virtual CLStatus Initialize() = 0;
	virtual CLStatus Uninitialize() = 0;
	
	virtual CLMessage* WaitForMessage() = 0;
	virtual CLStatus DispatchMessage(CLMessage *pMessage);

private:
	CLMessageLoopManager(const CLMessageLoopManager&);
	CLMessageLoopManager& operator=(const CLMessageLoopManager&);

protected:
	CLMessageObserver *m_pMessageObserver;
	std::map<unsigned long, CallBackForMessageLoop> m_MsgMappingTable;
};

#endif
