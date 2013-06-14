#ifndef CLEXECUTIVE_H
#define CLEXECUTIVE_H

#include "CLStatus.h"

class CLExecutiveFunctionProvider;

/* 
CLExecutive�����ڴ���һ��ִ����
*/
class CLExecutive
{
public:
	/*
	���캯���е�CLExecutiveFunctionProvider���Ͳ���������Ӷ��з��䣬�ҷ�����
	�����ٵ���delete�ͷ��ڴ�
	*/
	explicit CLExecutive(CLExecutiveFunctionProvider *pExecutiveFunctionProvider);
	virtual ~CLExecutive();

	/*
	Run�������ڴ���һ��ִ���壬����ִ���屻����������Run�����ŷ���
	Run���������س����򱾶����CLExecutiveFunctionProvider���󶼽����Զ�ɾ��
	*/
	virtual CLStatus Run(void *pContext = 0) = 0;

	/*
	�ȴ���������ִ��������
	*/
	virtual CLStatus WaitForDeath() = 0;
	
protected:
	CLExecutiveFunctionProvider *m_pExecutiveFunctionProvider;

private:
	CLExecutive(const CLExecutive&);
	CLExecutive& operator=(const CLExecutive&);
};

#endif
