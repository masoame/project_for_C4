#include"Basic_Server.h"
#pragma once
#include"_stdlib_.h"
enum status
{
	//待机
	WAIT = 0x0000,
	//停止
	STOP = 0x0001,
	//朗读
	READ = 0x0002,
	//交流
	DISALOGUE = 0x0004,
	//下一个
	NEXT = 0x0008,
	//应答ACK
	ACK = 0x0010,
	//信息
	Message = 0x0020
};
//自定义协议
typedef struct Head_code
{
	//固定数字(不可改变)
	const uint16_t DIY = 0xffee;

	//语音盒状态(默认)
	uint16_t target = WAIT;

	//包大小
	uint32_t size;
}Head_code, * LPHead_code;

typedef class Server_Box :public Basic_Server
{
public:
	int Cmd_Model();

	int POST_RECV(LPIO_DATA io_data);

	int FileSend(char* path, LPIO_DATA id);
}*LpServer_Box;
