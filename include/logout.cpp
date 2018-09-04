
#include "logout.h"

logout* logout::m_pLogger = NULL;

logout::logout() {
	// TODO 自动生成的构造函数存根

}

logout::~logout() {
	// TODO 自动生成的析构函数存根
}

/**
 * 获取日志对象实例
 * @return
 */
logout* logout::getInst()
{
	if( m_pLogger == NULL )
	{
		m_pLogger = new logout();
	}

	return m_pLogger;
}

/**
 * 初始化日志模块
 * @param strConf 日志配置文件路径
 * @param strAppName 应用程序名称
 */
void logout::Init(const string strConf, const string strAppName)
{
    PropertyConfigurator::configure( strConf );
    m_Log4cxx = Logger::getLogger( strAppName );
}

/**
 * 获取日志处理对象
 * @return
 */
LoggerPtr logout::getLogger()
{
	return m_Log4cxx;
}

