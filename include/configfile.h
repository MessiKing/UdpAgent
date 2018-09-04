
#ifndef CONFIGFILE_H
#define CONFIGFILE_H

#include <stdio.h>
#include <unistd.h>
#include <libxml/parser.h>
#include <libxml/tree.h>
#include <libxml/xpath.h>
#include <assert.h>
#include <string>
#include <string.h>
#include <vector>

using namespace std;

#define ROOT_ "config"            //根节点
#define SHARE_ "share"            //共享节点
#define SYSTYPE_  "sysType"        //系统类型
#define LOGCFG_ "logcfgfile"       //日志文件路径节点
#define DATABASE_ "database"       //数据库节点
#define DBUSER_ "dbuser"           //数据库用户名
#define SID_ "sid"                 //数据库SID
#define PASSWD_ "passwd"           //用户密码
#define SINGLE_ "single"           //程序节点名称
#define LOG_ "log"                 //日志节点
#define PROGRAM_ "program"         //程序名称属性节点
#define PID_ "pid"                 //程序id属性节点
#define RECVER_ "recver"           //zmq接收配置节点
#define SENDER_ "sender"           //zmq发送配置节点
#define ZMQ_ "zmq"                 //zmq节点
#define CONN_ "conn"               //zmq发送连接配置节点
#define MODE_ "mode"               //zmq 发送 接收 模式属性节点
#define FILTER_ "filter"           //zmq接收模块中过滤的属性节点
#define TIMEOUT_ "timeout"         //程序中延时等待的时间
#define MONSER_ "monitorserver"    //监控程序配置
#define IP_	"ip"                   //ip
#define PORT_ "port"               //端口号
#define BUFSIZE_ "bufsize"         //缓冲区的大小
#define SERVER_ "server"           //服务节点
#define KIND_ "kind"               //服务种类节点
#define NAME_ "name"               //名称
#define STORAGEDIR_ "StorageDir"   //存储路径
#define BASEDIR_ "BaseDir"         //基本目录
#define BASEDBDIR_ "BaseDBDir"         //基本目录
#define BASETRAINLIST_ "BaseTrainList"         //基本目录
#define USEDIR_ "UseDir"           //使用目录
#define BACK_ "back"               //备份节点
#define POSTFIX_ "postfix"         //后缀
#define TABLE_ "table"             //表名
#define DBMD5_ "DownMD5"           //MD5值节点
#define CODE_  "code"              //路局代号
#define VERSION_  "version"        //数据版本
#define DATATYPE_  "datatype"      //数据类型
#define HISTORYDIR_ "HistoryDir"    //历史目录
#define UNITID_ "UnitID"			//单位ID
#define DB_ "DB"           //MySql数据库节点
#define USER_ "user"           //Myql用户节点
#define PWD_ "pwd"           //MySql登录密码节点
#define DBNAME_ "DBname"                 //MySql数据库名称节点
#define HOST_ "host"                 //MySql连接主机节点
#define MYSQL_ "mysql"

/**
 * 配置文件操作类
 */
class ConfigFile
{
private:
    //文件名
    const char* filename;
    //程序名
    string proname;
    xmlDocPtr doc;
    xmlNodePtr RootNode;
    xmlXPathContextPtr context;
private:
    //创建配置文件
    void CreateConfig();
    //添加数据库配置
    void AddDBConf();
    //添加日志配置
    void AddLogConf();
    //添加监控配置
    void AddMonConf();
    //添加存储路径
    void AddStorageDir();
    //添加历史文件目录
    void AddHistoryDir();
    //添加MD5配置
    void AddMD5();
    //添加程序节点
    void AddSingle();
    //添加系统类型
    void AddSysType();
    //保存XML文档
    bool SaveXML();
    //添加孩子节点
    xmlNodePtr AddChildNode( xmlNodePtr node, const xmlChar* name );
    //获得节点集
    xmlXPathObjectPtr getNodeset( const xmlChar *xpath );
    //获得节点集中的第一个节点
    xmlNodePtr getNodesetFirst( const xmlChar *xpath );
    //获得节点值
    string getNodeValue( xmlNodePtr node );
    //获得程序的名称
    string getpname( char *fullname );
public:
    ConfigFile( char *fname, char *pname );
    ~ConfigFile();
    //获取数据库登录用户名
    string GetDBUser();

    //获取数据库实例
    string GetSID();

    //获取密码
    string GetPasswd();

    //获取zmq接收链接地址
    string GetRecverAddr();

    //获取zmq接收模式
    string GetRecverMode();

    //获取zmq接收过滤关键字
    string GetRecverFilter();

    //获取zmq接收超时时间
    string GetRecverTimeout();

    //获取zmq接收缓冲尺寸
    string GetRecverBufSize();

    //获取发送连接地址
    string GetSendAddr();

    //获取zmq发送模式
    string GetSendMode();

    //获取发送过滤关键字
    string GetSendFilter();

    //获取发送超时时间
    string GetSendTimeout();
    //获取系统类型
    string GetSysType();

    // 获取单位ID
    string GetUnitID();

    //根据xpath获取值
    string GetValue( xmlChar *xpath );
    string GetValue( const char *xpath );
    string GetValue( string &xpath );

    //根据xpath获取值集合
    void GetValue( xmlChar *xpath, vector<string> &valueset );
    void GetValue( const char *xpath, vector<string> &valueset );
    void GetValue( string &xpath, vector<string> &valueset );

    //获取日志配置文件路径
    string GetLogcfg();

    //获取监控IP
    string GetMonIP();

    //获取监控端口
    string GetMonPort();

    //获取应用ID
    string GetPID();

    //获取应用名称
    string GetProjName();

    //获取超时设置
    string GetTimeout();

    //获取文件存储目录路径
    string GetStorageDir();

    //获取文件存储历史目录路径
    string GetHistoryDir();

    //获取文件存储目录路径
    string GetBaseDir();
    string GetBaseDBDir();
    string GetBaseTrainList();
    //获取基础数据使用目录路径
    string GetUseDir();

    //获取md值
    string GetMD5();
    //设置md码值
    bool SetMD5( string& mdvalue );

    //设置xpath对应节点的值
    bool SetValue( string xpath, string value );

    //获取xpath查询到的节点下的子节点数
    unsigned int GetChildNodeCount( xmlChar *xpath );

};

#endif // CONFIGFILE_H
