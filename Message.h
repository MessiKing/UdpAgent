
#ifndef __MESSAGE_H
#define __MESSAGE_H

#include <string>
#include <string.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

class Message{

public:
	Message(){};
	virtual ~Message(){};

	virtual std::string Serialize() = 0;
	virtual void Deserialize(std::string msg) = 0;

protected:
	std::string message;

};

/**
 *
 */
class CMsgTrain : public Message{

public:
	CMsgTrain();
	virtual ~CMsgTrain();

	virtual std::string Serialize();
	virtual void Deserialize(std::string msg);

	void setAddr(struct sockaddr_in addr){memcpy(&m_addr, &addr, sizeof(struct sockaddr_in));}
	void setTrainMsg(std::string message){m_trainMsg = message;}

	void getAddr(struct sockaddr_in& addr){addr = m_addr;}
	std::string getTrainMsg(){return m_trainMsg;}

private:
	struct sockaddr_in m_addr;
	std::string m_trainMsg;
};


#endif

