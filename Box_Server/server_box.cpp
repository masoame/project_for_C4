#include"Server_Box.h"

int Server_Box::Cmd_Model()
{
	char path[1024];
	while (isopen)
	{
		std::cin >> buffer_cmd;

		if (strcmp(buffer_cmd, "quit") == 0)
		{
			isopen = false;
			std::cout << "服务器关闭..." << std::endl;
			exit(-1);
		}
		if (strcmp(buffer_cmd, "select") == 0)
		{
			for (auto temp : BOX_GROUP)
			{
				std::cout << temp << std::endl;
			}
		}
		if (strcmp(buffer_cmd, "send") == 0)
		{
			std::cin >> path;
			auto A = BOX_GROUP.begin();
			FileSend(path, *A);
		}
	}
	return 0;
}

int Server_Box::POST_RECV(LPIO_DATA io_data)
{
	LPHead_code temp = (LPHead_code)io_data->Buffer;

	//验证不成功则返回
	if (temp->DIY != 0xffee)
	{
		//删除相关的结构体
		BOX_GROUP.erase(io_data);
		//关闭套接字
		closesocket(io_data->socket);
		//删除结构体
		delete io_data;

		return false;
	}

	switch (temp->target)
	{
		//唯一识别id
	case WAIT:

		break;

	}

	Basic_Server::POST_RECV(io_data);

	return true;
}
//收到后台文件发送命令后
int Server_Box::FileSend(char* path, LPIO_DATA io_data)
{

	//接收缓存识别头文件
	LPHead_code temp = (LPHead_code)io_data->Buffer;

	//创建文件流
	std::ifstream file;
	file.open(path, std::ios::in | std::ios::binary | std::ios::ate);
	//无打开则返回false
	if (!file.is_open()) return false;

	//设置协议头
	temp->target = READ;
	temp->size = file.tellg();

	//开辟文件读取内存
	char* buffer = new char[temp->size];

	//发送给盒子让它做好接收准备
	if (SOCKET_ERROR == send(io_data->socket, (char*)temp, sizeof(Head_code), 0))
	{
		return false;
	}

	//将流定位到文件头,准备读取
	file.seekg(std::ios::_Seekbeg);

	//读入到缓存区
	file.read(buffer, temp->size);

	send(io_data->socket, buffer, temp->size, 0);

	std::cout << "文件发送成功" << std::endl;

	return 0;
}