
#ifndef __SERVERTRANSFER_H
#define __SERVERTRANSFER_H

#include <sys/epoll.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <unistd.h>
#include <sys/types.h>
#include "common.h"
#include "Client.h"

class CServerTranfer{

private:
	CServerTranfer(){};

public:
	~CServerTranfer(){};
	static CServerTranfer* getInstance();

public:
	int initSvrSocket(const char* ip, int port, struct sockaddr_in& svrAddr);
	void do_epoll();
	void add_event(CClient* pClient, int state);
	void delete_event(CClient* pClient, int state);
	void modify_event(CClient* pClient, int state);

private:
	static CServerTranfer* m_pSelf;
	static const int FDSIZE = 1000;
	static const int EPOLLEVENTS = 100;
	static const int MAX_LENGTH = 1024 * 1024;

	int m_epollfd;
};

#define PTR_SVR_TRANSFER CServerTranfer::getInstance()

#endif
