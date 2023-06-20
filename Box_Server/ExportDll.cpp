#include"ExportDll.h"

LpServer_Box temp = nullptr;


void server_start()
{
	if (temp != nullptr)return;

	temp = new Server_Box;
	temp->init();
	temp->run();

}

void sendFile(char* path, void* id)
{
	//如果id为空默认对组的第一个进行操作
	if (id == nullptr)
	{
		auto a = temp->BOX_GROUP.begin();
		id = *a;
	}

	temp->FileSend(path, (LPIO_DATA)id);
}