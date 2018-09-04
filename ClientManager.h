
#ifndef __CLIENTMANAGER_H
#define __CLIENTMANAGER_H

#include <thread>
#include <unistd.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <map>
#include <mutex>

#include "common.h"
#include "Client.h"

class CClientManager{

private:
	CClientManager();

public:
	~CClientManager();
	static CClientManager* getInstance();

public:
	CClient* GetClient(struct sockaddr_in addr);
	void Close(struct sockaddr_in addr);
	void StartWatcher();

private:
	void procValidWatcher();

private:
	std::mutex m_mutex;

	static CClientManager* m_pSelf;
	std::map<long long, CClient*> m_mapClient;
};

#define PTR_CLIENT_MGR CClientManager::getInstance()

#endif


