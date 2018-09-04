
#ifndef LOGOUT_H_
#define LOGOUT_H_

#include <log4cxx/propertyconfigurator.h>
#include <log4cxx/logger.h>
#include <string>

using namespace std;
using namespace log4cxx;

class logout
{
private:
	static logout* m_pLogger;			// 单例
	log4cxx::LoggerPtr m_Log4cxx;		// Log4cxx日志对象

private:
	logout();

public:
	~logout();

	static logout* getInst();
	void Init(const string strConf, const string strAppName);	// 初始化
	LoggerPtr getLogger();										// 获取日志处理对象
};

#endif /* LOGOUT_H_ */
