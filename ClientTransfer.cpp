/*
 * ClientTransfer.cpp
 *
 *  Created on: Aug 25, 2018
 *      Author: wangyd
 */

#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <assert.h>
#include "ClientTransfer.h"
#include "Client.h"
#include "ClientManager.h"
#include "Message.h"
#include "configfile.h"

CClientTransfer* CClientTransfer::m_pSelf = NULL;

CClientTransfer* CClientTransfer::getInstance()
{
	if(NULL == m_pSelf)
		m_pSelf = new CClientTransfer();
	return m_pSelf;
}

void CClientTransfer::SetConfig(ConfigFile& conf)
{
	m_remotSvrIP = "";
	m_remotSvrPort = 0;
	m_localListenPort = 0;

	std::string xpath = "/" ROOT_ "/" SINGLE_ "[@" PROGRAM_ "=\'" + conf.GetProjName() + "\']/server/ip";
	m_remotSvrIP = conf.GetValue(BAD_CAST(xpath.c_str()));
	assert(m_remotSvrIP != "");

	xpath = "/" ROOT_ "/" SINGLE_ "[@" PROGRAM_ "=\'" + conf.GetProjName() + "\']/server/port";
	m_remotSvrPort = std::stoi(conf.GetValue(BAD_CAST(xpath.c_str())));
	assert(m_remotSvrPort != 0);

	xpath = "/" ROOT_ "/" SINGLE_ "[@" PROGRAM_ "=\'" + conf.GetProjName() + "\']/listenport";
	m_localListenPort = std::stoi(conf.GetValue(BAD_CAST(xpath.c_str())));
	assert(m_localListenPort != 0);

	xpath = "/" ROOT_ "/" SINGLE_ "[@" PROGRAM_ "=\'" + conf.GetProjName() + "\']/threadcount";
	m_threadCount = std::stoi(conf.GetValue(BAD_CAST(xpath.c_str())));
	m_threadCount = m_threadCount > 5 ? m_threadCount : 5;

//	m_remotSvrPort = 9989;
//	m_remotSvrIP = "127.0.0.1";
//	m_localListenPort = 10080;
//	m_threadCount = 1;

	pthread_mutex_init(&m_mutex, NULL);

	InitSocket();

}

void CClientTransfer::InitSocket()
{
	struct sockaddr_in servaddr;

	m_sockfd = socket(AF_INET, SOCK_DGRAM, 0);

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(m_localListenPort);

	bind(m_sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr));
}

void CClientTransfer::Run()
{
	for(int index = 0; index < m_threadCount; ++index)
	{
		CRecvQueue* pQueue = new CRecvQueue();
		m_vecRecvQueue.push_back(pQueue);
	}

	std::thread tRecv(&CClientTransfer::procRecv, this);
	tRecv.detach();

	for(int index = 0; index < m_threadCount; ++index)
	{
		std::thread tRecvMsg(&CClientTransfer::procRecvMsg, this, index);
		tRecvMsg.detach();
	}
}

void CClientTransfer::procRecv()
{
//	CClientTransfer* pSelf = (CClientTransfer*)argc;

	struct sockaddr_in cliaddr;
	socklen_t cliaddr_len;
	char buf[MAX_LENGTH];
	int len, index;
	CMsgTrain tMsg;

	while(true)
	{
		::bzero(buf, MAX_LENGTH);

		cliaddr_len = sizeof(cliaddr);
		len = recvfrom(this->m_sockfd, buf, MAX_LENGTH, 0, (struct sockaddr*)&cliaddr, &cliaddr_len);
		//LOG_DEBUG("[C->S] receive message from client. len:"<<to_string(len));
		if(len == -1)
		{
			continue;
		}

		//long long clientId = cliaddr.in_addr.s_addr + cliaddr.sin_port;
		index = (cliaddr.sin_addr.s_addr) % (this->m_threadCount);
		tMsg.setAddr(cliaddr);
		tMsg.setTrainMsg(std::string(buf, len));
		this->m_vecRecvQueue[index]->PushMsg(tMsg.Serialize());
	}
}


void CClientTransfer::procRecvMsg(int arg)
{
//	ptrTArgs pargs = (ptrTArgs)argc;
	int index = arg;
//	CClientTransfer* pSelf = (CClientTransfer*)(pargs->handle);

	std::string msg, content;
	struct sockaddr_in addr;
	while(true)
	{
		CMsgTrain tMsg;
		msg = this->m_vecRecvQueue[index]->PopMsg();
		tMsg.Deserialize(msg);
		tMsg.getAddr(addr);

		CClient* pClient = CClientManager::getInstance()->GetClient(addr);
		if(NULL == pClient)
		{
			continue;
		}

		content = tMsg.getTrainMsg();
		pClient->procRecvClientMsg(content);
	}
}

int CClientTransfer::Sendto(const char* buf, size_t len, const struct sockaddr * dst, socklen_t addrlen)
{
	if(m_sockfd < 0)
	{
		return -1;
	}

	pthread_mutex_lock(&m_mutex);

	int rtn = ::sendto(m_sockfd, buf, len, 0, dst, addrlen);

	pthread_mutex_unlock(&m_mutex);

	return rtn;
}


