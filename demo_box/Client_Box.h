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

class Client_Box
{
public:

	int sockserver;

	//全部初始化为false
	bool arr[10] = { false };

	char buffer[TCP_MTU] = { 0 };
	LPHead_code buf = (LPHead_code)buffer;

	//连接服务器
	int linkserver(const char* ip = "192.168.137.1", int port = 0x0721);
	//写入相应的文件
	int MakeFile(const char* path, char* file, int len);
	//从网络接收
	char* RecvNet(int* len);

	int RecvData(void* arg);
	int InitPlaySound();

	static void* Sound_static(void* path);

	~Client_Box();
};