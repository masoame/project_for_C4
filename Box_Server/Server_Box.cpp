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
	DWORD IO_SIZE;
	void* lpCompletionKey;
	LPOVERLAPPED lpOverlapped;


	while (server_box->isopen)
	{
		Check_ret(GetQueuedCompletionStatus(server_box->iocpHandle, &IO_SIZE, (PULONG_PTR)&lpCompletionKey, &lpOverlapped, 5), false);

		if (IO_SIZE == 0)
		{
			std::cout << "客户端断开" << std::endl;

		}

	
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

	SYSTEM_INFO si;
	GetSystemInfo(&si);

	CPUCORE_NUM = si.dwNumberOfProcessors;

	if (WSAStartup(MAKEWORD(2, 2), lpwsadata) != 0)
	{
		return -1;
	}
	//创建监听SOCKET
	listen_socket = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);

	//创建IOCP句柄
	iocpHandle = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
	Check_ret(iocpHandle, NULL);

	//将socket绑定到IOCP
	Check_ret(CreateIoCompletionPort((HANDLE)listen_socket, iocpHandle, 0, 0), NULL);

	//监听地址
	listen_adress.sin_family = AF_INET;
	listen_adress.sin_addr.S_un.S_addr = inet_addr(ip);
	listen_adress.sin_port = ntohs(port);

	//固定端口
	Check_ret(bind(listen_socket, (sockaddr*)&listen_adress, sizeof(sockaddr)), -1);

	//开始监听
	Check_ret(listen(listen_socket, SOMAXCONN), -1);

	//int optval = 1;
	//setsockopt(listen_socket, SOL_SOCKET, SO_REUSEADDR, (char*)&optval, sizeof(optval));


	// 创建重叠结构体
	LPIO_DATA perIoData = new IO_DATA;
	memset(&(perIoData->Overlapped), 0, sizeof(WSAOVERLAPPED));
	perIoData->DataBuf.len = MAX_BUFF_SIZE;
	perIoData->DataBuf.buf = perIoData->Buffer;

	LPFN_ACCEPTEX m_lpfnAcceptEx;				// AcceptEx函数指针  
	GUID GuidAcceptEx = WSAID_ACCEPTEX;			// GUID，这个是识别AcceptEx函数必须的  
	DWORD dwBytes = 0;

	Check_ret(WSAIoctl(listen_socket, SIO_GET_EXTENSION_FUNCTION_POINTER, &GuidAcceptEx, sizeof(GuidAcceptEx), &m_lpfnAcceptEx, sizeof(m_lpfnAcceptEx), &dwBytes, NULL, NULL), SOCKET_ERROR);


	GetAcceptExSockaddrs
	AcceptEx();
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