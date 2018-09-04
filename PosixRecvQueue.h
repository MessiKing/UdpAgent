/*
 * PosixRecvQueue.h
 *
 *  Created on: Aug 25, 2018
 *      Author: wangyd
 */

#ifndef POSIXRECVQUEUE_H_
#define POSIXRECVQUEUE_H_

#include <list>
#include <mutex>
#include <condition_variable>
#include <string>
#include "common.h"

class CRecvQueue
{
public:
	CRecvQueue();
	~CRecvQueue();

	//数据入队列
	void PushMsg(std::string  strMsg);
	//数据出队列
	std::string PopMsg();
	//返回数据大小
	int QueueSize();
private:
	std::mutex m_mutexLockForQueue;
	std::condition_variable m_condForQueue;
	std::list<std::string> m_listQueue;
};


#endif /* POSIXRECVQUEUE_H_ */
