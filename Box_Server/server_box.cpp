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
		//等待
	case WAIT:

		break;
		//读命令
	case READ:


		break;
		//回复已经收到对应文件
	case READ | ACK:

		break;
	}

	Basic_Server::POST_RECV(io_data);

	return true;
}
