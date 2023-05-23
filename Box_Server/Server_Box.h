﻿#pragma once
#include"_stdlib_.h"
#include"_define_.h"

//盒子服务器
class Server_Box
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

	//存放语音盒用户组
	std::set<SOCKET> BOX_SOCK;
	//发送队列
	std::queue<char*> send_queue;

	//管理员输入字符缓存区域
	char* buffer_cmd;

public:

	//-----------------------FUCTION---------------------------
private:
	//监听模块
	static int Listen_Model(Server_Box* server_box);
	//客户端命令模块
	static int Cmd_Model(Server_Box* server_box);

	//发送(异步发送)
	static int AsyncSend(Server_Box* server_box, SOCKET target, HANDLE file, std::string cmd);
	//发送(非异步发送)
	static int Send(Server_Box* server_box, SOCKET target, HANDLE file, std::string cmd);
	//发送模块
	static int Send_Model(Server_Box* server_box);
	//接收模块
	static int Recv_Model(Server_Box* server_box);

	//核心处理模块
	static int Core_Model(Server_Box* server_box);
public:
	//初始化服务器(默认监听端口为0x0721)
	int init(const char* ip = "0.0.0.0", const int port = 0x0721);

	//开始服务器
	int run();

	//-------------------------CLASS----------------------------

	//构造函数
	Server_Box();

	//析构函数
	~Server_Box();

	//禁止拷贝复制，禁止拷贝构造
	Server_Box& operator=(const Server_Box&) = delete;
	Server_Box(const Server_Box&) = delete;
	Server_Box(const Server_Box&&) = delete;
};