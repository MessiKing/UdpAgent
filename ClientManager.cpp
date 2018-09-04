
#include "ClientManager.h"

CClientManager* CClientManager::m_pSelf = NULL;

/**
 * 构造
 */
CClientManager::CClientManager()
{

}

CClientManager::~CClientManager()
{
	
}

/**
 * 单例
 */
CClientManager* CClientManager::getInstance()
{
	if(NULL == m_pSelf)
		m_pSelf = new CClientManager();
	return m_pSelf;
}

/**
 * 新增客户端
 */
CClient* CClientManager::GetClient(struct sockaddr_in addr)
{
	long long key = addr.sin_addr.s_addr + addr.sin_port;

	std::lock_guard<std::mutex> lock(m_mutex);

	std::map<long long, CClient*>::iterator iter = m_mapClient.find(key);
	CClient* pClient;
	if(m_mapClient.end() == iter)
	{
		pClient = new CClient(addr);
		m_mapClient[key] = pClient;
	}
	else
	{
		pClient = iter->second;
	}

	return pClient;
}

/**
 *
 */
void CClientManager::StartWatcher()
{
	std::thread t(&CClientManager::procValidWatcher, this);

	t.detach();
}

/**
 *
 */
void CClientManager::procValidWatcher()
{
	std::map<long long, CClient*>::iterator iter;

	time_t now = time(NULL);
	time_t old = now;
	while(true)
	{
		::usleep(3* 60 * 1000 * 1000);

		now = time(NULL);
		std::lock_guard<std::mutex> lock(m_mutex);
		iter = m_mapClient.begin();
		for(; iter != m_mapClient.end();)
		{
			if(iter->second->IsValid(old))
			{
				iter->second->SetValid(now);
				++iter;
			}
			else
			{
				// 关闭无效链接
				iter->second->closeClient();
				m_mapClient.erase(iter++);
			}
		}

		old = now;
	}
}

/**
 * 客户端断开链接
 */
void CClientManager::Close(struct sockaddr_in addr)
{
	LOG_DEBUG("close client connection.");
	std::lock_guard<std::mutex> lock(m_mutex);

	long long key = addr.sin_addr.s_addr + addr.sin_port;
	std::map<long long, CClient*>::iterator iter = m_mapClient.find(key);
	if(m_mapClient.end() != iter)
	{
		iter->second->closeClient();
	}

	m_mapClient.erase(iter);
}



