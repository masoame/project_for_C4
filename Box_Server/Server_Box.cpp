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

	
	buffer_cmd = new char[2048];

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


	//将socket绑定到IOCP
	Check_ret(CreateIoCompletionPort((HANDLE)listen_socket, iocpHandle, 0, 0), NULL);

	//服务初始化完成
	std::cout << "Server init complete" << std::endl;

	return 0;
}

//工作模块
int Server_Box::Work_Model(Server_Box* server_box)
{
	DWORD IO_SIZE;
	//创建重叠结构体指针
	void* lpCompletionKey;

	LPIO_DATA lpOverlapped;


	while (server_box->isopen)
	{
		bool res = GetQueuedCompletionStatus(server_box->iocpHandle, &IO_SIZE, (PULONG_PTR)lpCompletionKey, (LPOVERLAPPED*)&lpOverlapped, INFINITE);
		if (!res)
		{
			if (GetLastError() == WAIT_TIMEOUT || GetLastError() == ERROR_NETNAME_DELETED)
			{

			}
		}




		Sleep(10);
	}



	return 0;
}

int Server_Box::run()
{
	isopen = true;

	int work_num = sys_info.dwNumberOfProcessors * 2;
	HANDLE* workgroup = new HANDLE[work_num];

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
	//网络环境未初始化
	lpwsadata = nullptr;

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
