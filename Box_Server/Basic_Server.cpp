﻿#include"Basic_Server.h"


int Basic_Server::Cmd_Model(Basic_Server* basic_server)
{
	while (basic_server->isopen)
	{
		std::cin >> basic_server->buffer_cmd;

		if (strcmp(basic_server->buffer_cmd, "#quit") == 0)
		{
			//shutdown(server_box->listen_socket, SD_BOTH);
			basic_server->isopen = false;
			std::cout << "服务器关闭..." << std::endl;
			exit(-1);
		}
	}
	return 0;
}



int Basic_Server::init(const char* ip, const int port)
{
	isopen = true;

	lpwsadata = new WSADATA;

	//获取本地系统信息
	GetSystemInfo(&sys_info);

	//初始化网络环境
	if (WSAStartup(MAKEWORD(2, 2), lpwsadata) != 0)
	{
		return -1;
	}
	//创建监听SOCKET
	listen_socket = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);

	//创建IOCP句柄
	iocpHandle = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
	Check_ret(iocpHandle, NULL);

	//监听地址
	listen_adress.sin_family = AF_INET;
	listen_adress.sin_addr.S_un.S_addr = inet_addr(ip);
	listen_adress.sin_port = ntohs(port);

	//固定端口
	Check_ret(bind(listen_socket, (sockaddr*)&listen_adress, sizeof(sockaddr)), -1);

	//开始监听
	Check_ret(listen(listen_socket, SOMAXCONN), -1);

	LPIO_DATA io_data = new IO_DATA{ 0 };
	io_data->socket = listen_socket;
	io_data->type = IO_ACCEPT;


	//将socket绑定到IOCP
	Check_ret(CreateIoCompletionPort((HANDLE)listen_socket, iocpHandle, (ULONG_PTR)io_data, 0), NULL);

	//服务初始化完成
	std::cout << "Server init complete" << std::endl;

	return 0;
}

inline int Basic_Server::POST_ACCEPT(Basic_Server* basic_server)
{
	//先创建好SOCKET
	SOCKET tempsock = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, NULL, WSA_FLAG_OVERLAPPED);
	Check_ret(tempsock, INVALID_SOCKET);

	//设置重叠结构体
	LPIO_DATA temp = new IO_DATA{ 0 };
	temp->type = IO_ACCEPT;
	temp->DataBuf.buf = temp->Buffer;
	temp->DataBuf.len = sizeof(temp->Buffer);
	temp->socket = tempsock;
	DWORD lpdwBytesReceived = 0;

	//绑定到listen_sock
	if (AcceptEx(basic_server->listen_socket, temp->socket, temp->DataBuf.buf, TCP_MTU, sizeof(sockaddr_in) + 16, sizeof(sockaddr_in) + 16, &lpdwBytesReceived, &temp->Overlapped) == FALSE)
	{
		if (GetLastError() != WSA_IO_PENDING)
		{
			std::cout << "投递失败" << std::endl;
			return false;
		}
	}
	basic_server->BOX_GROUP.insert(temp);

	return 0;
}

int Basic_Server::DO_ACCEPT(Basic_Server* basic_server, LPIO_DATA io_data)
{
	sockaddr_in* Localaddr = nullptr, * Remoteaddr = nullptr;
	int LocaladdrLength = sizeof(sockaddr_in), RemoteaddrLength = sizeof(sockaddr_in);
	GetAcceptExSockaddrs(io_data->DataBuf.buf, TCP_MTU, sizeof(sockaddr_in) + 16, sizeof(sockaddr_in) + 16, (sockaddr**)&Localaddr, &LocaladdrLength, (sockaddr**)&Remoteaddr, &RemoteaddrLength);

	std::cout << "验证信息: " << io_data->DataBuf.buf << std::endl;

	io_data->type = IO_RECV;

	Check_ret(CreateIoCompletionPort((HANDLE)io_data->socket, basic_server->iocpHandle, (ULONG_PTR) & io_data, 0) ,NULL);

	POST_RECV(basic_server, io_data);
	
	return 0;
}

int Basic_Server::POST_RECV(Basic_Server* server_box, LPIO_DATA io_data)
{
	DWORD dwFlags = 0;
	DWORD dwBytes = 0;

	if ((WSARecv(io_data->socket, &io_data->DataBuf, 1, &dwBytes, &dwFlags, &io_data->Overlapped, NULL) == SOCKET_ERROR) && WSAGetLastError() != WSA_IO_PENDING) return false;

	return true;
}

//工作模块
int Basic_Server::Work_Model(Basic_Server* basic_server)
{

	DWORD IO_SIZE = 0;
	//创建重叠结构体指针
	LPIO_DATA lpCompletionKey=nullptr;
	//重叠结构体
	LPIO_DATA lpOverlapped=nullptr;
	

	while (basic_server->isopen)
	{
		BOOL res = GetQueuedCompletionStatus(basic_server->iocpHandle, &IO_SIZE, (PULONG_PTR)&lpCompletionKey, (LPOVERLAPPED*)&lpOverlapped, INFINITE);
		if (!res)
		{
			if (GetLastError() == WAIT_TIMEOUT || GetLastError() == ERROR_NETNAME_DELETED)
			{
				if (closesocket(lpOverlapped->socket) != 0)
				{
					std::cout << "套接字关闭失败" << std::endl;
				}

				basic_server->BOX_GROUP.erase(lpOverlapped);
				delete lpOverlapped;

				continue;
			}
		}

		if (IO_SIZE == 0 && lpOverlapped->type != IO_ACCEPT)
		{
			std::cout << "客户端断开" << std::endl;
			if (closesocket(lpOverlapped->socket) != 0)
			{
				std::cout << "套接字关闭失败" << WSAGetLastError()<< std::endl;
			}

			basic_server->BOX_GROUP.erase(lpOverlapped);
			delete lpOverlapped;
			continue;
		}


		switch (lpOverlapped->type)
		{
		case IO_ACCEPT:
			std::cout << IO_SIZE << std::endl;
			std::cout << "客户端连接" << std::endl;
			DO_ACCEPT(basic_server, lpOverlapped);

			break;
		case IO_DISCONNECT:



			break;
		case IO_RECV:

			std::cout << lpOverlapped->Buffer << std::endl;
			
			POST_RECV(basic_server, lpOverlapped);

			break;
		case IO_SEND:



			break;
		}
	}

	return 0;
}

//启动
int Basic_Server::run()
{
	isopen = true;

	int work_num = sys_info.dwNumberOfProcessors * 2;

	std::cout << "CPU核心数为" << sys_info.dwNumberOfProcessors << std::endl;

	HANDLE* workgroup = new HANDLE[work_num];

	//投递SOCKET等待connect
	for (int i = 0; i != work_num; i++)
	{
		POST_ACCEPT(this);
	}


	//创建CPU核心数两倍的工作线程
	for (int i = 0; i!= work_num; i++)
	{
		workgroup[i] = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)Work_Model, this, NULL, NULL);
	}

	//等待工作线程返回
	for (int i = 0; i != work_num; i++)
	{
		WaitForSingleObject(workgroup[i], INFINITE);
	}
	return 0;
}

Basic_Server::Basic_Server()
{
	//服务器状态为关闭
	isopen = false;
	//初始化网络环境
	lpwsadata = nullptr;
	//cmd输入缓存
	buffer_cmd = nullptr;
}

Basic_Server::~Basic_Server()
{
	//关闭服务器
	isopen = false;
	//清理申请内存
	if (lpwsadata != nullptr) delete lpwsadata;

	closesocket(listen_socket);
	CloseHandle(iocpHandle);

	std::cout << "服务器数据释放" << std::endl;
}
