#include"Server_Box.h"

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

		//temp->id = io_data->socket;
		send(io_data->socket, (const char*)temp, sizeof(Head_code), 0);
		break;
	//回复已经收到对应文件
	case READ | ACK:

		break;
	}

	Basic_Server::POST_RECV(io_data);

	return true;
}
//收到后台文件发送命令后
int Server_Box::FileSend(char* path, LPIO_DATA id)
{
	//每一个发送帧的大小
	const int framesize = TCP_MTU - sizeof(Head_code);

	//接收缓存识别头文件
	LPHead_code temp = (LPHead_code)id->Buffer;

	//创建文件流
	std::ifstream file;
	file.open(path, std::ios::in | std::ios::binary | std::ios::ate);
	//无打开则返回false
	if (!file.is_open()) return false;

	//设置协议头
	temp->target = READ;
	temp->group_num = 0;
	temp->size = file.tellg();
	//开辟文件读取内存
	char* buffer = new char[temp->size];

	//发送给盒子让它做好接收准备
	if (SOCKET_ERROR == send(id->socket, (char*)temp, sizeof(Head_code), 0))
	{
		return false;
	}

	//将流定位到文件头,准备读取
	file.seekg(std::ios::_Seekbeg);

	//移动到要接收的文件头部
	char* sendbuffer = (char*)(temp + 1);

	//读入到缓存区
	file.read(buffer, temp->size);
	char* ptr;

	while (temp->size)
	{
		//如果文件大小小于帧，准备结束帧
		if ((temp->size - framesize) <= 0)
		{
			memcpy(sendbuffer, ptr, temp->size);
			send(id->socket, id->Buffer, temp->size + sizeof(Head_code), 0);

		}
		else
		{
			memcpy(sendbuffer, ptr, framesize);
			send(id->socket, id->Buffer, TCP_MTU, 0);
		}
		//等待回应
		WaitACK();
		//
		ptr += framesize;
	}

	//结束文件传输
	temp->target = READ | STOP;
	send(id->socket, id->Buffer, sizeof(Head_code), 0);
	


	return 0;
}
