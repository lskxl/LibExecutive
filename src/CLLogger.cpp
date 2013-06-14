#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "CLLogger.h"

#define LOG_FILE_NAME "logger"
#define MAX_SIZE 265
#define BUFFER_SIZE_LOG_FILE 4096

CLLogger* CLLogger::m_pLog = 0;
pthread_mutex_t CLLogger::m_Mutex = PTHREAD_MUTEX_INITIALIZER;

CLLogger::CLLogger()
{
	m_Fd = open(LOG_FILE_NAME, O_RDWR | O_CREAT | O_APPEND, S_IRUSR | S_IWUSR); 
	if(m_Fd == -1)
		throw "In CLLogger::CLLogger(), open error";

	m_pLogBuffer = new char[BUFFER_SIZE_LOG_FILE];
	m_nUsedBytesForBuffer = 0;
}

CLLogger::~CLLogger()
{
	delete [] m_pLogBuffer;

	close(m_Fd);
}

CLStatus CLLogger::WriteLogMsg(const char *pstrMsg, long lErrorCode)
{
	CLLogger *pLog = CLLogger::GetInstance();
	if(pLog == 0)
		return CLStatus(-1, 0);
	
	CLStatus s = pLog->WriteLog(pstrMsg, lErrorCode);
	if(s.IsSuccess())
		return CLStatus(0, 0);
	else
		return CLStatus(-1, 0);
}

CLStatus CLLogger::WriteLogDirectly(const char *pstrMsg, long lErrorCode)
{
	if((m_pLog == 0) || (pstrMsg == 0) || (strlen(pstrMsg) == 0))
		return CLStatus(-1, 0);

	int fd = open(LOG_FILE_NAME, O_RDWR | O_CREAT | O_APPEND, S_IRUSR | S_IWUSR); 
	if(fd == -1)
		return CLStatus(-1, 0);

	char buf[MAX_SIZE];
	snprintf(buf, MAX_SIZE, "	Error code: %ld\r\n",  lErrorCode);

	int r = pthread_mutex_lock(&m_Mutex);
	if(r != 0)
	{
		if(close(fd) == -1)
			return CLStatus(-1, errno);

		return CLStatus(-1, r);
	}

	try
	{
		if(m_pLog == 0)
			throw CLStatus(-1, 0);

		if(!WriteMsgAndErrcodeToFile(fd, pstrMsg, buf).IsSuccess())
			throw CLStatus(-1, 0);

		throw CLStatus(0, 0);
	}
	catch(CLStatus &s)
	{
		r = pthread_mutex_unlock(&m_Mutex);
		int r1 = close(fd);
		if((r != 0) || (r1 == -1))
			return CLStatus(-1, 0);

		return s;
	}
	catch(...)
	{
		pthread_mutex_unlock(&m_Mutex);
		close(fd);

		return CLStatus(-1, 0);
	}
}

int CLLogger::WriteOfProcessSafety(int fd, const void *buff, size_t nbytes)
{
	struct flock lock;
	lock.l_type = F_WRLCK;
	lock.l_start = 0;
	lock.l_whence = SEEK_END;
	lock.l_len = 0;

	if(fcntl(fd, F_SETLKW, &lock) == -1)
		return -1;

	ssize_t writedbytes = write(fd, buff, nbytes);
	if(writedbytes == -1)
		writedbytes = 0;

	lock.l_type = F_UNLCK;
	lock.l_start = -writedbytes;
	lock.l_whence = SEEK_END;
	lock.l_len = 0;

	fcntl(fd, F_SETLKW, &lock);

	return writedbytes;
}

CLStatus CLLogger::Flush()
{
	int r = pthread_mutex_lock(&m_Mutex);
	if(r != 0)
		return CLStatus(-1, r);

	try
	{
		if(m_pLog == 0)
			throw CLStatus(-1, 0);

	    if(m_nUsedBytesForBuffer == 0)
		    throw CLStatus(0, 0);

		if(WriteOfProcessSafety(m_Fd, m_pLogBuffer, m_nUsedBytesForBuffer) == -1)
			throw CLStatus(-1, errno);

		m_nUsedBytesForBuffer = 0;

		throw CLStatus(0, 0);
	}
	catch(CLStatus &s)
	{
		r = pthread_mutex_unlock(&m_Mutex);		
		if(r != 0)
			return CLStatus(-1, r);
		
	    return s;
	}
	catch(...)
	{
		r = pthread_mutex_unlock(&m_Mutex);		
		if(r != 0)
			return CLStatus(-1, r);

		return CLStatus(-1, 0);
	}
}

CLStatus CLLogger::WriteMsgAndErrcodeToFile(int fd, const char *pstrMsg, const char *pstrErrcode)
{
	int len = strlen(pstrErrcode) + strlen(pstrMsg) + 1;
	char *p = new char[len];
	memset(p, 0, len);
	strcat(p, pstrMsg);
	strcat(p, pstrErrcode);

	if(WriteOfProcessSafety(fd, p, len - 1) == -1)
	{
		delete [] p;
		return CLStatus(-1, 0);
	}

	delete [] p;
	return CLStatus(0, 0);
}

CLStatus CLLogger::WriteLog(const char *pstrMsg, long lErrorCode)
{
	if(pstrMsg == 0)
		return CLStatus(-1, 0);

	if(strlen(pstrMsg) == 0)
		return CLStatus(-1, 0);

	char buf[MAX_SIZE];
	snprintf(buf, MAX_SIZE, "	Error code: %ld\r\n",  lErrorCode);

	int len_strmsg = strlen(pstrMsg);
	int len_code = strlen(buf);
	unsigned int total_len = len_strmsg + len_code;

	int r = pthread_mutex_lock(&m_Mutex);
	if(r != 0)
		return CLStatus(-1, r);
	
	try
	{
		if(m_pLog == 0)
			throw CLStatus(-1, 0);

		if(total_len > BUFFER_SIZE_LOG_FILE)
			throw WriteMsgAndErrcodeToFile(m_Fd, pstrMsg, buf);

		unsigned int nleftroom = BUFFER_SIZE_LOG_FILE - m_nUsedBytesForBuffer;
		if(total_len > nleftroom)
		{
			if(WriteOfProcessSafety(m_Fd, m_pLogBuffer, m_nUsedBytesForBuffer) == -1)
				throw CLStatus(-1, errno);

			m_nUsedBytesForBuffer = 0;
		}

		memcpy(m_pLogBuffer + m_nUsedBytesForBuffer, pstrMsg, len_strmsg);
		m_nUsedBytesForBuffer += len_strmsg;

		memcpy(m_pLogBuffer + m_nUsedBytesForBuffer, buf, len_code);
		m_nUsedBytesForBuffer += len_code;

		throw CLStatus(0, 0);
	}
	catch (CLStatus& s)
	{
		r = pthread_mutex_unlock(&m_Mutex);
		if(r != 0)
			return CLStatus(-1, r);
		
		return s;
	}
	catch(...)
	{
		r = pthread_mutex_unlock(&m_Mutex);		
		if(r != 0)
			return CLStatus(-1, r);

		return CLStatus(-1, 0);
	}
}

CLLogger* CLLogger::GetInstance()
{
	return m_pLog;
}

CLStatus CLLogger::Create()
{
	//CLLibExecutiveInitializer��Initialize������
	//���ڲ���ÿ�������ʼ���������ɹ���
	//��˿��ܶ�ε���ǰ���Ѿ��ɹ��˵Ķ����Create����
	if(m_pLog != 0)
		return CLStatus(0, 0);

	m_pLog = new CLLogger();

	return CLStatus(0, 0);
}

CLStatus CLLogger::Destroy()
{
	if(m_pLog == 0)
		return CLStatus(0, 0);

	int r = pthread_mutex_lock(&m_Mutex);
	if(r != 0)
		return CLStatus(-1, r);

	try
	{
		if(m_pLog->m_nUsedBytesForBuffer != 0)
		{
			if(WriteOfProcessSafety(m_pLog->m_Fd, m_pLog->m_pLogBuffer, m_pLog->m_nUsedBytesForBuffer) == -1)
				throw CLStatus(-1, errno);
		}

		delete m_pLog;

		m_pLog = 0;

		throw CLStatus(0, 0);
	}
	catch(CLStatus& s)
	{
		r = pthread_mutex_unlock(&m_Mutex);
		if(r != 0)
			return CLStatus(-1, r);

		return s;
	}
	catch(...)
	{
		r = pthread_mutex_unlock(&m_Mutex);
		if(r != 0)
			return CLStatus(-1, r);

		return CLStatus(-1, 0);
	}
}