#pragma once
#include"_stdlib_.h"
#include"_define_.h"

enum IO_TYPE
{
	IO_ACCEPT,
	IO_RECV,
	IO_SEND,
	IO_DISCONNECT
};


//重叠结构体
typedef struct IO_DATA {

	OVERLAPPED Overlapped;
	SOCKET socket;
	IO_TYPE type;
	WSABUF DataBuf;
	CHAR Buffer[TCP_MTU + (SOCKADDR_IN_LEN + 16) * 2];

} IO_DATA, * LPIO_DATA;


//服务器基类
class Basic_Server
{
	//-----------------------VALUE-----------------------------
private:
	//服务器是否开启
	bool isopen;
	//window自带
	LPWSADATA lpwsadata;
	//监听套接字
	SOCKET listen_socket;
	//监听地址
	sockaddr_in listen_adress;

	//存放重叠结构体
	std::set<LPIO_DATA> BOX_GROUP;

	//CPU核数
	SYSTEM_INFO sys_info;

	HANDLE iocpHandle;


	//管理员输入字符缓存区域
	char* buffer_cmd;

	//-----------------------FUCTION---------------------------
private:

	//客户端命令模块
	static int Cmd_Model(Basic_Server* server_box);

	//投递ACCEPT
	static int POST_ACCEPT(Basic_Server* server_box);

	//处理ACCEPT请求
	static int DO_ACCEPT(Basic_Server* server_box, LPIO_DATA io_data);

	//投递RECV
	static int POST_RECV(Basic_Server* server_box, LPIO_DATA io_data);

	
	//工作线程
	static int Work_Model(Basic_Server* server_box);
public:

	//初始化服务器(默认监听端口为0x0721)
	virtual inline int init(const char* ip = "0.0.0.0", const int port = 0x0721);

	//开始服务器
	virtual int run();

	//-------------------------CLASS----------------------------

	//构造函数
	Basic_Server();

	//析构函数
	~Basic_Server();

	//禁止拷贝复制，禁止拷贝构造
	Basic_Server& operator=(const Basic_Server&) = delete;
	Basic_Server(const Basic_Server&) = delete;
	Basic_Server(const Basic_Server&&) = delete;
};
