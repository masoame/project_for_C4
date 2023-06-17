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

};
//自定义协议
typedef struct Head_code
{
	//固定数字(不可改变)
	const uint16_t DIY = 0xffee;

	//语音盒状态(默认)
	status target = WAIT;

	//组号(无组号则为-1)
	uint32_t group_num;

	//包大小
	uint16_t size;


}Head_code, * LPHead_code;



class server_box :public Basic_Server
{
	int POST_RECV(LPIO_DATA io_data);
};
