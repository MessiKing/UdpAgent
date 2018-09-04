
#include "common.h"
#include "configfile.h"
#include "ClientManager.h"
#include "ClientTransfer.h"
#include "ServerTransfer.h"

int main(int argc, char* argv[])
{

	ConfigFile conf((char*)CONFIG_FILE_NAME, argv[0]);

	logout::getInst()->Init( conf.GetLogcfg(), conf.GetProjName() );
	LOG_INFO("Application start!");

	PTR_CLIENT_TRANSFER->SetConfig(conf);
	PTR_CLIENT_TRANSFER->Run();

	CClientManager::getInstance()->StartWatcher();

	PTR_SVR_TRANSFER->do_epoll();

	return 0;
}


