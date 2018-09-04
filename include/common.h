
#ifndef COMMON_H_
#define COMMON_H_

#include <iostream>
#include <string>
#include "logout.h"

using namespace std;

// 系统配置文件名
#define CONFIG_FILE_NAME	"./conf.d/config.xml"
//日志配置文件
//#define CONFIG_LOG_NAME   "../conf.d/log.conf"

// 系统中sqlite的文件名称
#define OTPSQLITE  "sqlite_database"

// 基础数据文件使用时存放目录
#define DBFILEPATH "USEDDBFILE"

/* 指针释放宏操作定义 */
#define SAFE_DLTPT(p) if( NULL != p ) { delete p; p=NULL; }

// 日志输出函数宏定义
#define LOG_ERROR(msg)	LOG4CXX_ERROR(logout::getInst()->getLogger(),__FILE__<<" ["<<__LINE__<<"] "<<msg)
#define LOG_WARN(msg)	LOG4CXX_WARN(logout::getInst()->getLogger(),__FILE__<<" ["<<__LINE__<<"] "<<msg)
#define LOG_INFO(msg) 	LOG4CXX_INFO(logout::getInst()->getLogger(),__FILE__<<" ["<<__LINE__<<"] "<<msg)
#define LOG_DEBUG(msg) 	LOG4CXX_DEBUG(logout::getInst()->getLogger(),__FILE__<<" ["<<__LINE__<<"] "<<msg)

//#define LOG_ERROR(msg) ""
//#define LOG_WARN(msg) ""
//#define LOG_INFO(msg) ""
//#define LOG_DEBUG(msg) ""
//定义字节序转换函数宏
#define CONVERTENDIAN(a, n) \
{ \
		unsigned char *pHead = (unsigned char *)a; \
		unsigned char *pEnd = pHead+n-1; \
		for(; pHead <= pEnd; ++pHead, --pEnd) \
		{ \
			*pHead ^= *pEnd; \
			*pEnd ^= *pHead; \
			*pHead ^= *pEnd; \
		} \
}

#ifndef PPC
	//字节长度为2字节
	#define CONVERTENDIAN2(a) CONVERTENDIAN(a,2)
	//字节长度为4字节
	#define CONVERTENDIAN4(a) CONVERTENDIAN(a,4)
	//字节长度为6字节
	#define CONVERTENDIAN6(a) CONVERTENDIAN(a,6)
	//字节长度为8字节
	#define CONVERTENDIAN8(a) CONVERTENDIAN(a,8)
#else
	//字节长度为2字节
	#define CONVERTENDIAN2(a)
	//字节长度为4字节
	#define CONVERTENDIAN4(a)
	//字节长度为6字节
	#define CONVERTENDIAN6(a)
	//字节长度为8字节
	#define CONVERTENDIAN8(a)
#endif

#endif /* COMMON_H_ */
