/*
 * PosixRecvQueue.cpp
 *
 *  Created on: Aug 25, 2018
 *      Author: wangyd
 */

#include "PosixRecvQueue.h"

/**
 * 构造函数
 * @param 无
 * @return 无
 */
CRecvQueue::CRecvQueue()
{
	m_listQueue.clear();
}

/**
 * 析构函数
 * @param 无
 * @return 无
 */
CRecvQueue::~CRecvQueue()
{

}


/**
 *  数据入队列
 * @param strMsg 消息内容
 * @return 无
 */
void CRecvQueue::PushMsg(std::string  strMsg)
{
	std::unique_lock<std::mutex> lk(m_mutexLockForQueue);

	m_listQueue.push_back(strMsg);

	m_condForQueue.notify_all();
}

/**
 *  数据出队列
 * @param
 * @return 消息
 */
std::string CRecvQueue::PopMsg()
{
	std::unique_lock<std::mutex> lk(m_mutexLockForQueue);

	if (m_listQueue.size() == 0)
	{
		m_condForQueue.wait(lk);
	}
	std::string strMsg = m_listQueue.front();

	m_listQueue.pop_front();

	return strMsg;
}


