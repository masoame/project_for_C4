#include"Server_Box.h"


int Server_Box::Cmd_Model(Server_Box* server_box)
{
	while (server_box->isopen)
	{
		std::cin >> server_box->buffer_cmd;

		if (strcmp(server_box->buffer_cmd, "#quit") == 0)
		{
			//shutdown(server_box->listen_socket, SD_BOTH);
			server_box->isopen = false;
			std::cout << "服务器关闭..." << std::endl;
			exit(-1);
		}
	}
	return 0;
}


int Server_Box::init(const char* ip, const int port)
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

inline int Server_Box::POST_ACCEPT(Server_Box* server_box)
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
	if (AcceptEx(server_box->listen_socket, temp->socket, temp->DataBuf.buf, TCP_MTU, sizeof(sockaddr_in) + 16, sizeof(sockaddr_in) + 16, &lpdwBytesReceived, &temp->Overlapped) == FALSE)
	{
		if (GetLastError() != WSA_IO_PENDING)
		{
			std::cout << "投递失败" << std::endl;
			return false;
		}
	}


	return 0;
}

//工作模块
int Server_Box::Work_Model(Server_Box* server_box)
{

	DWORD IO_SIZE = 0;
	//创建重叠结构体指针
	LPIO_DATA lpCompletionKey=nullptr;
	//重叠结构体
	LPIO_DATA lpOverlapped=nullptr;
	

	while (server_box->isopen)
	{
		BOOL res = GetQueuedCompletionStatus(server_box->iocpHandle, &IO_SIZE, (PULONG_PTR)lpCompletionKey, (LPOVERLAPPED*)&lpOverlapped, INFINITE);
		if (!res)
		{
			if (GetLastError() == WAIT_TIMEOUT || GetLastError() == ERROR_NETNAME_DELETED)
			{
				closesocket(lpOverlapped->socket);
				continue;
			}
		}
		
		if (IO_SIZE==0)
		{
			std::cout << "IO_SIZE==0" << GetLastError() << std::endl;
		}

		if (lpOverlapped == nullptr)
		{
			std::cout << "skip" << std::endl;
			continue;
		}

		

		switch (lpOverlapped->type)
		{
		case IO_ACCEPT:

			std::cout << "连接请求收到" << std::endl;

			break;
		case IO_DISCONNECT:



			break;
		case IO_RECV:



			break;
		case IO_SEND:



			break;
		}

		Sleep(100);
	}


	return 0;
}

int Server_Box::run()
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

Server_Box::Server_Box()
{
	//服务器状态为关闭
	isopen = false;
	//初始化网络环境
	lpwsadata = nullptr;
	//
	buffer_cmd = nullptr;
}

Server_Box::~Server_Box()
{
	//关闭服务器
	isopen = false;
	//清理申请内存
	if (lpwsadata != nullptr) delete lpwsadata;


	std::cout << "服务器数据释放" << std::endl;
}
