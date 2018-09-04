
#include "configfile.h"

/**
 * 构造函数
 * @param fname 配置文件路径
 * @param pname 应用名
 */
ConfigFile::ConfigFile( char *fname, char *pname ) :
    filename( fname ), proname( getpname( pname ) ), doc( NULL ), RootNode( NULL ), context( NULL )
{
    if ( access( filename, F_OK ) == -1 )
    {
        CreateConfig();
    }

    xmlKeepBlanksDefault( 0 );
    doc = xmlParseFile( filename );
    assert(doc != NULL);
    RootNode = xmlDocGetRootElement( doc );
    context = xmlXPathNewContext( doc );
    AddSingle();
}

/**
 * 析构函数
 */
ConfigFile::~ConfigFile()
{
    xmlXPathFreeContext( context );
    xmlFreeDoc( doc );
}

/**
 * 创建配置文件
 */
void ConfigFile::CreateConfig()
{
    doc = xmlNewDoc( BAD_CAST "1.0" );
    RootNode = xmlNewNode( NULL, BAD_CAST (ROOT_) );
    xmlDocSetRootElement( doc, RootNode );
    context = xmlXPathNewContext( doc );
    AddChildNode( RootNode, BAD_CAST (SHARE_) );
    AddDBConf();
    AddLogConf();
    AddMonConf();
    AddHistoryDir();
    AddStorageDir();
    AddMD5();
}

/**
 * 保存XML文档
 * @return    true 保存成功  false 保存失败
 */
bool ConfigFile::SaveXML()
{
    return xmlSaveFormatFileEnc( filename, doc, "UTF-8", 1 ) == -1 ? false : true;
}

/**
 * 获取程序的名称
 * @param fullname  带全路径的名称
 * @return     程序名称字符串
 */
string ConfigFile::getpname( char *fullname )
{
    char str[1024] = { 0 };
    string res( "" );
    strcpy( str, fullname );
    char *token = strtok( str, "/" );
    while ( token != NULL )
    {
        res = token;
        token = strtok( NULL, "/" );
    }
    return res;
}

/**
 * 获得节点集
 * @param xpath    支持xpath方式路径查询
 * @return   xmlXPathObjectPtr类型的对象指针
 */
xmlXPathObjectPtr ConfigFile::getNodeset( const xmlChar *xpath )
{

    xmlXPathObjectPtr result;
    if ( context == NULL )
    {
        return NULL;
    }

    result = xmlXPathEvalExpression( xpath, context );
    if ( result == NULL )
    {
        return NULL;
    }

    if ( xmlXPathNodeSetIsEmpty(result->nodesetval) )
    {
        xmlXPathFreeObject( result );
        return NULL;
    }

    return result;
}

/**
 * 获取数据库登录用户名
 * @return 用户名
 */
string ConfigFile::GetDBUser()
{
    xmlChar *xpath = BAD_CAST ("/" ROOT_ "/" SHARE_ "/" DATABASE_ "/" DBUSER_ );
    return getNodeValue( getNodesetFirst( xpath ) );
}
/**
 * node节点下添加子节点name
 * @param node  要添加子节点的节点
 * @param name  要添加的子节点名
 * @return      成功返回添加的子节点指针，失败返回NULL
 */
xmlNodePtr ConfigFile::AddChildNode( xmlNodePtr node, const xmlChar* name )
{
    xmlNodePtr _node = xmlNewNode( NULL, name );
    xmlAddChild( node, _node );
    return SaveXML() ? _node : NULL;
}
/**
 * 添加数据库配置项，共用
 */
void ConfigFile::AddDBConf()
{
    xmlChar *xpath = BAD_CAST ("/" ROOT_ "/" SHARE_ );
    xmlNodePtr cur = getNodesetFirst( xpath );
    if ( cur )
    {
        xmlNodePtr _node = AddChildNode( cur, BAD_CAST (DATABASE_) );
        if ( _node )
        {
            AddChildNode( _node, BAD_CAST (SID_) );
            AddChildNode( _node, BAD_CAST (DBUSER_) );
            AddChildNode( _node, BAD_CAST (PASSWD_) );
        }
        SaveXML();
    }
}
/**
 * 添加日志配置项，共用
 */
void ConfigFile::AddLogConf()
{
    xmlChar *xpath = BAD_CAST ("/" ROOT_ "/" SHARE_ );
    xmlNodePtr cur = getNodesetFirst( xpath );
    if ( cur )
    {
        xmlNodePtr _node = AddChildNode( cur, BAD_CAST (LOG_) );
        if ( _node )
        {
            xmlNewTextChild( _node, NULL, BAD_CAST (LOGCFG_), (const xmlChar *) "conf.d/DMSII.logcfg" );
        }
        SaveXML();
    }

}
/**
 * 添加文件存储目录位置，共用
 */
void ConfigFile::AddStorageDir()
{
    xmlChar *xpath = BAD_CAST ("/" ROOT_ "/" SHARE_ );
    xmlNodePtr cur = getNodesetFirst( xpath );
    if ( cur )
    {
        xmlNewTextChild( cur, NULL, BAD_CAST (STORAGEDIR_), (const xmlChar *) "./memdisk" );
        xmlNewTextChild( cur, NULL, BAD_CAST (BASEDIR_), (const xmlChar *) "./BASEDIR" );
        SaveXML();
    }
}

/**
 * 添加文件存储历史目录位置
 */
void ConfigFile::AddHistoryDir()
{
    xmlChar *xpath = BAD_CAST ("/" ROOT_ "/" SHARE_ );
    xmlNodePtr cur = getNodesetFirst( xpath );
    if ( cur )
    {
        xmlNewTextChild( cur, NULL, BAD_CAST (HISTORYDIR_), (const xmlChar *) "./" );

        SaveXML();
    }
}

/**
 * 添加MD5码配置项
 */
void ConfigFile::AddMD5()
{
    xmlChar *xpath = BAD_CAST ("/" ROOT_ "/" SHARE_ );
    xmlNodePtr cur = getNodesetFirst( xpath );
    if ( cur )
    {
        xmlNewTextChild( cur, NULL, BAD_CAST (DBMD5_), (const xmlChar *) "" );
        SaveXML();
    }
}

/**
 * 添加系统类型
 */
void ConfigFile::AddSysType()
{
    xmlChar *xpath = BAD_CAST ("/" ROOT_ "/" SHARE_ );
    xmlNodePtr cur = getNodesetFirst( xpath );
    if ( cur )
    {
        xmlNewTextChild( cur, NULL, BAD_CAST (SYSTYPE_), (const xmlChar *) "1" );
        SaveXML();
    }
}

/**
 * 添加监控配置，共用
 */
void ConfigFile::AddMonConf()
{
    xmlChar *xpath = BAD_CAST ("/" ROOT_ "/" SHARE_ );
    xmlNodePtr cur = getNodesetFirst( xpath );
    if ( cur )
    {
        xmlNodePtr _node = AddChildNode( cur, BAD_CAST (MONSER_) );
        if ( _node )
        {
            xmlNewTextChild( _node, NULL, BAD_CAST (IP_), (const xmlChar *) "127.0.0.1" );
            xmlNewChild( _node, NULL, BAD_CAST (PORT_), (const xmlChar *) "9999" );

        }
        SaveXML();
    }
}
/**
 * 根据xpath获取节点集的第一个节点
 * @param xpath 查询路径
 * @return  结果集的第一个节点指针
 */
xmlNodePtr ConfigFile::getNodesetFirst( const xmlChar* xpath )
{
    xmlNodePtr res = NULL;
    xmlXPathObjectPtr app_result = getNodeset( xpath );
    if ( app_result )
    {
        res = app_result->nodesetval->nodeTab[0];
        xmlXPathFreeObject( app_result );
    }
    return res;
}
/**
 * 获取node的值
 * @param node  要获取值的节点
 * @return      返回node的文本值
 */
string ConfigFile::getNodeValue( xmlNodePtr node )
{
    string res = "";
    if ( node )
    {
        xmlChar* value = xmlNodeGetContent( node );
        if ( value )
        {
            res = (char*) value;
            xmlFree( value );
        }
    }
    return res;
}

/**
 * 添加程序节点
 */
void ConfigFile::AddSingle()
{
    string tmp = "/" ROOT_ "/" SINGLE_ "[@" PROGRAM_ "=\'" + proname + "\']";
    xmlChar *xpath = BAD_CAST (tmp.c_str());
    xmlNodePtr cur = getNodesetFirst( xpath );
    if ( !cur )
    {
        xmlNodePtr _node = AddChildNode( RootNode, BAD_CAST (SINGLE_) );
        if ( _node )
        {
            xmlSetProp( _node, BAD_CAST (PROGRAM_), BAD_CAST (proname.c_str()) );
            xmlSetProp( _node, BAD_CAST (PID_), BAD_CAST ("0") );

            xmlNodePtr _childnode = AddChildNode( _node, BAD_CAST (ZMQ_) );
            if ( _childnode )
            {
                xmlNodePtr tmp_node = AddChildNode( _childnode, BAD_CAST (RECVER_) );
                xmlSetProp( tmp_node, BAD_CAST (MODE_), BAD_CAST ("") );
                xmlSetProp( tmp_node, BAD_CAST (FILTER_), BAD_CAST ("") );
                xmlSetProp( tmp_node, BAD_CAST (TIMEOUT_), BAD_CAST ("") );
                xmlNewTextChild( tmp_node, NULL, BAD_CAST (CONN_), (const xmlChar *) "tcp://127.0.0.1:7680" );
                xmlNewTextChild( tmp_node, NULL, BAD_CAST (BUFSIZE_), (const xmlChar *) "1048576" );

                tmp_node = AddChildNode( _childnode, BAD_CAST (SENDER_) );
                xmlSetProp( tmp_node, BAD_CAST (MODE_), BAD_CAST ("") );
                xmlSetProp( tmp_node, BAD_CAST (FILTER_), BAD_CAST ("") );
                xmlSetProp( tmp_node, BAD_CAST (TIMEOUT_), BAD_CAST ("") );
                xmlNewChild( tmp_node, NULL, BAD_CAST (CONN_), (const xmlChar *) "tcp://*:9999" );

            }

            xmlNewChild( _node, NULL, BAD_CAST (TIMEOUT_), (const xmlChar *) "10" );
        }

        SaveXML();
    }
}

/**
 * 直接获取路径指定值
 * @param xpath 路径
 * @return 路径指定的值
 */
string ConfigFile::GetValue( xmlChar *xpath )
{
    return getNodeValue( getNodesetFirst( xpath ) );
}

string ConfigFile::GetValue( const char *xpath )
{
    return GetValue( BAD_CAST (xpath) );
}

string ConfigFile::GetValue( string &xpath )
{
    return GetValue( xpath.c_str() );
}

/**
 * 获取路径指定值集合
 * @param xpath    路径
 * @param valueset  返回的值集合
 */
void ConfigFile::GetValue( xmlChar *xpath, vector<string> &valueset )
{
    xmlNodePtr node_ = NULL;
    xmlXPathObjectPtr app_result = getNodeset( xpath );
    if ( app_result )
    {
        for ( int i = 0; i < app_result->nodesetval->nodeNr; i++ )
        {
            node_ = app_result->nodesetval->nodeTab[i];
            valueset.push_back( getNodeValue( node_ ) );
        }
        xmlXPathFreeObject( app_result );
    }
}

/**
 * 获取路径下，所有节点的值，并存放在一个vector<string>的容器中
 * @param xpath      查询路径
 * @param valueset   值集合
 */
void ConfigFile::GetValue( const char *xpath, vector<string> &valueset )
{
    GetValue( BAD_CAST (xpath), valueset );
}

/**
 * 获取路径下，所有节点的值，并存放在一个vector<string>的容器中
 * @param xpath      查询路径
 * @param valueset   值集合
 */
void ConfigFile::GetValue( string &xpath, vector<string> &valueset )
{
    GetValue( xpath.c_str(), valueset );
}

/**
 * 获取指定路径节点下的子节点数
 * @param xpath 路径
 * @return 子节点个数
 */
unsigned int ConfigFile::GetChildNodeCount( xmlChar *xpath )
{
    unsigned int res = 0;
    xmlXPathObjectPtr app_result = getNodeset( xpath );
    if ( app_result )
    {
        res = app_result->nodesetval->nodeNr;
        xmlXPathFreeObject( app_result );
    }
    return res;
}


/**
 * 获取日志配置文件路径
 * @return 路径
 */
string ConfigFile::GetLogcfg()
{
    string tmp = "/" ROOT_ "/" SHARE_ "/" LOG_ "/" LOGCFG_ ;
    return GetValue( BAD_CAST (tmp.c_str()) );
}


/**
 * 获取应用ID
 * @return 应用ID
 */
string ConfigFile::GetPID()
{
    string tmp = "/" ROOT_ "/" SINGLE_ "[@" PROGRAM_ "=\'" + proname + "\']/@" PID_ ;
    return GetValue( BAD_CAST (tmp.c_str()) );
}


/**
 * 获取应用名称
 * @return 应用名
 */
string ConfigFile::GetProjName()
{
    return proname;
}

/**
 * 设置xpath对应节点的值
 * @param xpath    路径
 * @param value    修订值
 * @return         true 执行成功，false 执行失败
 */
bool ConfigFile::SetValue( string path, string value )
{
    xmlChar *xpath = BAD_CAST (path.c_str());
    xmlNodePtr cur = getNodesetFirst( xpath );
    if ( cur )
    {
        xmlNodeSetContent( cur, (const xmlChar *) value.c_str() );
        SaveXML();
        return true;
    }
    return false;
}
