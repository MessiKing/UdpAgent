/*
 * ServerTransfer.cpp
 *
 *  Created on: Aug 26, 2018
 *      Author: wangyd
 */

#include <strings.h>
#include <string>
#include "ServerTransfer.h"
#include "Client.h"

CServerTranfer* CServerTranfer::m_pSelf;

CServerTranfer* CServerTranfer::getInstance()
{
	if(NULL == m_pSelf)
		m_pSelf = new CServerTranfer();
	return m_pSelf;
}

int CServerTranfer::initSvrSocket(const char* ip, int port, struct sockaddr_in& svrAddr)
{
	int svrfd;
	svrfd = socket(AF_INET, SOCK_DGRAM, 0);
	if (svrfd == -1)
	{
		//perror("socket error:");
		//exit(1);
	}
	bzero(&svrAddr, sizeof(struct sockaddr_in));
	svrAddr.sin_family = AF_INET;
	inet_pton(AF_INET,ip,&svrAddr.sin_addr);
	svrAddr.sin_port = htons(port);
//	if (bind(listenfd,(struct sockaddr*)&servaddr,sizeof(servaddr)) == -1)
//	{
//		perror("bind error: ");
//		exit(1);
//	}
	return svrfd;
}

void CServerTranfer::do_epoll()
{
	struct epoll_event events[EPOLLEVENTS];
	int ret, len;
	socklen_t recvAddrLen;
	char buf[MAX_LENGTH];
	std::string msg;

	m_epollfd = epoll_create(FDSIZE);

	CClient* pClient;
	for ( ; ; )
	{
		//获取已经准备好的描述符事件
		ret = epoll_wait(m_epollfd, events, EPOLLEVENTS, -1);
		for(int index = 0; index < ret; ++index)
		{
			// 只处理收到的信息
			if(events[index].events && EPOLLIN )
			{
				::bzero(buf, MAX_LENGTH);

				recvAddrLen = sizeof(struct sockaddr);
				pClient = (CClient*)(events[index].data.ptr);
				len = recvfrom(pClient->GetSvrFd(), buf, MAX_LENGTH, 0, (struct sockaddr*)(pClient->GetSvrAddr()), &recvAddrLen);
				//LOG_DEBUG("[S->C] Recv message from server. fd:"<<pClient->GetSvrFd()<<" length:"<<len);
				if(len == -1)
				{
					continue;
				}

				msg = std::string(buf, len);
				pClient->procRecvServerMsg(msg);
			}
		}
	}
	close(m_epollfd);
}

void CServerTranfer::add_event(CClient* pClient, int state)
{
	//LOG_DEBUG("Add client to epoll. fd:"<<pClient->GetSvrFd());

    struct epoll_event ev;
    ev.events = state;
    ev.data.ptr = pClient;
    epoll_ctl(m_epollfd, EPOLL_CTL_ADD, pClient->GetSvrFd(), &ev);
}

void CServerTranfer::delete_event(CClient* pClient, int state)
{
    struct epoll_event ev;
    ev.events = state;
    ev.data.ptr = pClient;
    epoll_ctl(m_epollfd, EPOLL_CTL_DEL, pClient->GetSvrFd(), &ev);
}

void CServerTranfer::modify_event(CClient* pClient, int state)
{
    struct epoll_event ev;
    ev.events = state;
    ev.data.ptr = pClient;
    epoll_ctl(m_epollfd, EPOLL_CTL_MOD, pClient->GetSvrFd(), &ev);
}

