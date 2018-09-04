#include <sys/epoll.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <errno.h>
#include <iostream>
#include "common.h"
#include "Client.h"
#include "ClientTransfer.h"
#include "ServerTransfer.h"

/**
 * 构造函数
 */
CClient::CClient(struct sockaddr_in addr)
{
	char str[INET_ADDRSTRLEN];
	memcpy(&m_cliAddr, &addr, sizeof(struct sockaddr_in));
	memset(&m_svrAddr, 0x00, sizeof(struct sockaddr_in));
	m_svrfd = -1;
	m_validCounter = time(NULL);

	m_trainIP = inet_ntop(AF_INET, &m_cliAddr.sin_addr, str, INET_ADDRSTRLEN);
	m_trainPort = ntohs(m_cliAddr.sin_port);
	LOG_INFO("New connect with client. IP:"<<m_trainIP<<" Port:"<<std::to_string(m_trainPort));
}

CClient::~CClient()
{

}

/**
 * 处理接收到的客户端信息
 * 1.建立与服务器的链接
 * 2.转发数据
 */
void CClient::procRecvClientMsg(std::string& msg)
{
	// 新建链接
	std::string ip;
	int port;
	if(m_svrfd < 0)
	{
		char str[INET_ADDRSTRLEN];
		PTR_CLIENT_TRANSFER->GetRemotSvrAddr(ip, port);
		m_svrfd = PTR_SVR_TRANSFER->initSvrSocket(ip.c_str(), port, m_svrAddr);

		m_svrIP = inet_ntop(AF_INET, &m_svrAddr.sin_addr, str, INET_ADDRSTRLEN);
		m_svrPort = ntohs(m_svrAddr.sin_port);
		LOG_INFO("New connect with server. IP:"<<m_svrIP<<" Port:"<<std::to_string(m_svrPort));

		PTR_SVR_TRANSFER->add_event(this, EPOLLIN);
	}

	//LOG_DEBUG("svrfd: "<<to_string(m_svrfd)<<" message length:"<<to_string(msg.length()));

	if(msg.size() == 0)
	{
		LOG_DEBUG("Close connection with server. IP:"<<m_svrIP<<" Port:"<<std::to_string(m_svrPort));
		PTR_SVR_TRANSFER->delete_event(this, EPOLLIN);
		::close(m_svrfd);
		return;
	}

	SendtoServer(msg.c_str(), msg.length());
}

/**
 * 处理接收到的服务器信息
 *  转发数据
 */
void CClient::procRecvServerMsg(std::string& msg)
{
	SendtoClient(msg.c_str(), msg.size());
}

/**
 * 向服务器端发送数据
 */
int CClient::SendtoServer(const char* buf, int len)
{
	m_validCounter ++;
	LOG_DEBUG("[C->S]Send message to server. IP:"<<m_svrIP<<" Port:"<<std::to_string(m_svrPort)<<" fd:<"<<m_svrfd<<"> len:"<<len );
	int rtn = ::sendto(m_svrfd, buf, len, 0, (struct sockaddr*)&m_svrAddr, sizeof(struct sockaddr));

	return rtn;
}

/**
 * 向客户端发送数据
 */
int CClient::SendtoClient(const char* buf, int len)
{
	LOG_DEBUG("[S->C]Send message to client. IP:"<<m_trainIP<<" Port:"<<std::to_string(m_trainPort)<<" len:"<<len);
	int rtn = PTR_CLIENT_TRANSFER->Sendto(buf, len, (struct sockaddr*)&m_cliAddr, sizeof(struct sockaddr));
	return rtn;
}

/**
 * 客户端主动切断链接
 */
void CClient::closeClient()
{
	LOG_INFO("close. IP:"<<m_svrIP<<" Port:"<<std::to_string(m_svrPort)<<" fd:<"<<m_svrfd<<">");
	PTR_SVR_TRANSFER->delete_event(this, EPOLLIN);
//	::close(m_svrfd);
//	::shutdown(m_svrfd, SHUT_RDWR);
	::close(m_svrfd);
	m_svrfd = -1;
}

/**
 * 服务器端切断链接
 */
void CClient::closeServer()
{

}

bool CClient::IsValid(int now)
{
	LOG_DEBUG("Counter:"<<m_validCounter<<" now:"<<now);
	return (m_validCounter > now);
}



