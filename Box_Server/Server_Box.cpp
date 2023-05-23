#include"Server_Box.h"

int Server_Box::Listen_Model(Server_Box* server_box)
{
	sockaddr_in temp_addr;
	int temp_len = sizeof(sockaddr);

	//存储临时套接字
	SOCKET SOCKET_TEMP;

	while (server_box->isopen)
	{
		SOCKET_TEMP = accept(server_box->listen_socket, (sockaddr*)&temp_addr, &temp_len);
		Check_ret(SOCKET_TEMP, SOCKET_ERROR);

		//加入表中方便管理
		server_box->BOX_SOCK.insert(SOCKET_TEMP);
		std::cout << "设备ip:" << inet_ntoa(temp_addr.sin_addr) << "\n端口:" << ntohs(temp_addr.sin_port) << "\n设备链接成功" << std::endl;
	}
	//select()

	return 0;
}

int Server_Box::Cmd_Model(Server_Box* server_box)
{
	while (server_box->isopen)
	{
		std::cin >> server_box->buffer_cmd;

		if (strcmp(server_box->buffer_cmd, "#quit") == 0)
		{
			shutdown(server_box->listen_socket, SD_BOTH);
			server_box->isopen = false;
			std::cout << "服务器关闭..." << std::endl;
		}
	}
	return 0;
}

int Server_Box::Send(Server_Box* server_box, SOCKET target, HANDLE file, std::string cmd)
{
	if (file == NULL)
	{
	}

	return 0;
}

int Server_Box::AsyncSend(Server_Box* server_box, SOCKET target, HANDLE file, std::string cmd)
{
	if (file == NULL)
	{
	}

	return 0;
}

int Server_Box::Send_Model(Server_Box* server_box)
{
	while (server_box->isopen)
	{
		if (!server_box->send_queue.empty())
		{
		}
		Sleep(10);
	}

	return 0;
}

int Server_Box::Recv_Model(Server_Box* server_box)
{
	return 0;
}

int Server_Box::Core_Model(Server_Box* server_box)
{
	return 0;
}

int Server_Box::init(const char* ip, const int port)
{
	isopen = true;

	buffer_cmd = new char[2048];

	lpwsadata = new WSADATA;

	if (WSAStartup(MAKEWORD(2, 2), lpwsadata) != 0)
	{
		return -1;
	}
	listen_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	listen_adress.sin_family = AF_INET;
	listen_adress.sin_addr.S_un.S_addr = inet_addr(ip);
	listen_adress.sin_port = ntohs(port);

	Check_ret(bind(listen_socket, (sockaddr*)&listen_adress, sizeof(sockaddr)), -1);

	Check_ret(listen(listen_socket, 5), -1);

	return 0;
}

int Server_Box::run()
{
	isopen = true;

	HANDLE h_start = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)&Listen_Model, this, 0, NULL);
	HANDLE h_cmd = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)&Cmd_Model, this, 0, NULL);
	HANDLE h_core = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)&Core_Model, this, 0, NULL);

	Check_ret(WaitForSingleObject(h_start, INFINITE), -1);
	Check_ret(WaitForSingleObject(h_cmd, INFINITE), -1);
	Check_ret(WaitForSingleObject(h_core, INFINITE), -1);
	return 0;
}

Server_Box::Server_Box()
{
	isopen = false;

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