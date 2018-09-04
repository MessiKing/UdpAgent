/*
 * ClientTransfer.h
 *
 *  Created on: Aug 25, 2018
 *      Author: wangyd
 */

#ifndef CLIENTTRANSFER_H_
#define CLIENTTRANSFER_H_

#include <thread>
#include <sys/types.h>
#include <sys/socket.h>
#include <vector>
#include "configfile.h"
#include "common.h"
#include "PosixRecvQueue.h"

typedef struct __threadArg{
	int index;
	void* handle;
}TArgs, *ptrTArgs;

class CClientTransfer{

private:
	CClientTransfer(){};

public:
	static CClientTransfer* getInstance();
	~CClientTransfer(){};

public:
	void SetConfig(ConfigFile& conf);
	void InitSocket();
	void Run();
	void procRecv();
//	void* procRecv(void* argc);
	int Sendto(const char* buf, size_t len, const struct sockaddr * dst, socklen_t addrlen);
	void procRecvMsg(int argc);

public:
	void GetRemotSvrAddr(std::string& ip, int& port){ip = m_remotSvrIP; port = m_remotSvrPort;}
	int GetLocalListenPort(int& port){return m_localListenPort;}

private:
	static CClientTransfer* m_pSelf;

	int m_remotSvrPort;
	std::string m_remotSvrIP;
	int m_localListenPort;
	int m_threadCount;
	int m_sockfd;

	static const int MAX_LENGTH = 1024 * 1024;

	std::vector<CRecvQueue*> m_vecRecvQueue;
//	std::vector<std::thread> m_vecThreadid;

	pthread_mutex_t m_mutex;
};

#define PTR_CLIENT_TRANSFER CClientTransfer::getInstance()


#endif /* CLIENTTRANSFER_H_ */
