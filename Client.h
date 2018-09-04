
#ifndef __CLIENT_H
#define __CLIENT_H

#include <pthread.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>

/**
 * 处理客户端接收到的信息
 */
class CClient{

public:
	CClient(struct sockaddr_in addr);
	~CClient();

public:
	void procRecvClientMsg(std::string& msg);
	void procRecvServerMsg(std::string& msg);
	void closeClient();
	void closeServer();

	int GetSvrFd(){return m_svrfd;}
	struct sockaddr_in* GetSvrAddr(){return &m_svrAddr;}
	void SetValid(int start){m_validCounter = start;}
	bool IsValid(int now);

private:
	int SendtoServer(const char* buf, int len);
	int SendtoClient(const char* buf, int len);

private:
	struct sockaddr_in m_cliAddr;
	struct sockaddr_in m_svrAddr;
	int m_svrfd;
	std::string m_trainIP;
	int m_trainPort;
	std::string m_svrIP;
	int m_svrPort;
	long long m_validCounter;
};

#endif



