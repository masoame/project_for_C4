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
//
void* Server_Box::SEND_QUEUE::action(FLAG flag, char* data)
{
	std::unique_lock<std::mutex> lck(mtx);
	switch (flag)
	{
	//获取当前队头元素
	case front:

		return _queue.front();
		break;
	//加入元素
	case push:

		_queue.push(data);
		break;
	//删除元素
	case pop:

		delete[] _queue.front();
		_queue.pop();
		break;
	case isempty:

		return (void*)_queue.empty();

		break;
	}
	return nullptr;
}


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
		}
	}
	return 0;
}

int Server_Box::Send(Server_Box* server_box, SOCKET target, void* file, uint16_t len)
{
	if (file == NULL)
	{

	}

	return 0;
}

int Server_Box::AsyncSend(Server_Box* server_box, SOCKET target, void* file, uint16_t len)
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
		//if (server_box->send_queue.action(SEND_QUEUE::isempty))
		//{
		//	std::cout << "队列为空等待..." << std::endl;
		//}
		Sleep(100);
	}

	return 0;
}

int Server_Box::Recv_Model(Server_Box* server_box)
{
	sockaddr_in temp = {0};
	int len = sizeof(sockaddr);
	char* buffer = new char[1024];
	while (server_box->isopen)
	{
		recv(server_box->listen_socket, buffer, 1024, 0);

		std::cout << "收到回复地址:" << inet_ntoa(temp.sin_addr) << "端口:" << ntohs(temp.sin_port) << std::endl;
		Sleep(10);
	}

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
	int optval = 1;
	setsockopt(listen_socket, SOL_SOCKET, SO_REUSEADDR, (char*)&optval, sizeof(optval));


	//创建IOCP句柄
	HANDLE iocpHandle = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
	Check_ret(iocpHandle, NULL);

	Check_ret(CreateIoCompletionPort((HANDLE)listen_socket, iocpHandle, 0, 0), NULL);

	std::cout << "Server started." << std::endl;

	return 0;
}

int Server_Box::run()
{
	isopen = true;

	std::thread listen_thr(Listen_Model, this);
	std::thread Cmd_thr(Cmd_Model, this);
	std::thread Recv_thr(Recv_Model, this);
	std::thread Send_thr(Send_Model, this);
	
	//HANDLE h_listen = listen_thr.native_handle();
	//HANDLE h_cmd = Cmd_thr.native_handle();
	//HANDLE h_core = Core_thr.native_handle();
	//HANDLE h_send = Send_thr.native_handle();


	//Check_ret(WaitForSingleObject(h_listen, INFINITE), -1);
	//Check_ret(WaitForSingleObject(h_cmd, INFINITE), -1);
	//Check_ret(WaitForSingleObject(h_core, INFINITE), -1);
	//Check_ret(WaitForSingleObject(h_send, INFINITE), -1);

	listen_thr.join();
	Cmd_thr.join();
	Recv_thr.join();
	Send_thr.join();

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