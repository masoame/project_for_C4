#pragma once
#include"_stdlib_.h"

struct Send
{
	//发送目标的套接字
	SOCKET target_sock;
	//区号
	uint16_t Area;
	//组号
	uint64_t Group;
	//大小(取TCP_MTU相关)
	uint64_t Size;
};

//自定义协议
struct DIY_protocol
{
	//固定数字(不可改变)
	const uint16_t DIY = 0xffee;

	PROCTOCOL_CTRL CTRL;
};

enum PROCTOCOL_CTRL
{
	//待机
	WAIT = 0x0000,
	//强行停止
	STOP = 0x0001,
	//朗读
	READ = 0x0002,
	//交流
	DISALOGUE = 0x0004,

	//保留UDP连接使用
	PACKAGE_READ = 0x0008,
	PACKAGE_DISALOGUE = 0x0010
};
