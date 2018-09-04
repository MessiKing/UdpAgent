/*
 * Message.cpp
 *
 *  Created on: Aug 27, 2018
 *      Author: wangyd
 */

#include "Message.h"

CMsgTrain::CMsgTrain()
{
	::bzero(&m_addr, sizeof(struct sockaddr_in));
}

CMsgTrain::~CMsgTrain()
{

}

std::string CMsgTrain::Serialize()
{
	int len = sizeof(struct sockaddr_in);
	char* pbuf = new char[len];

	memcpy(pbuf, &m_addr, len);

	message = std::string(pbuf, len);
	message += m_trainMsg;

	delete [] pbuf;

	return message;
}

void CMsgTrain::Deserialize(std::string msg)
{
	int len = sizeof(struct sockaddr_in);
	memcpy(&m_addr, msg.c_str(), len);
	m_trainMsg = std::string(msg, len, msg.length() - len);
}
