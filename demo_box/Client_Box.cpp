#include"Client_Box.h"

//连接服务器
int Client_Box::linkserver(const char* ip, int port)
{
	sockserver = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	sockaddr_in server_addr;
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = inet_addr(ip);
	server_addr.sin_port = ntohs(port);

	if (connect(sockserver, (sockaddr*)&server_addr, sizeof(sockaddr)) != 0) return false;

	Head_code temp;

	send(sockserver, &temp, sizeof(temp), 0);

	return sockserver;
}

//将内存写入成文件
inline int Client_Box::MakeFile(const char* path, char* file, int len)
{
	std::ofstream tofile;
	tofile.open(path, std::ios::binary | std::ios::out);
	if (!tofile.is_open())
	{
		tofile.close();
		return false;
	}

	tofile.write(file, len);
	tofile.close();

	//已写入成文件将加入循环队列
	arr[path[0] - '0'] = true;

	//清楚开辟的内存空间
	delete[] file;
	return true;
}

char* Client_Box::RecvNet(int* len)
{
	//缓存区指针
	char* filetemp = nullptr;

	int temp = 0;
	//准备接收头
	while ((temp = recv(sockserver, buffer, sizeof(Head_code), 0)) == -1)
	{
		std::cout << "接收错误，错误代码: " << std::endl;
		std::cout << errno << std::endl;

		sleep(2);
	}
	//识别标志是否为READ
	if (buf->target == READ)
	{
		//文件传输第一个包为信息包
		*len = buf->size;
		filetemp = new char[*len];

		std::cout << "prepare to down" << std::endl;
		std::cout << "buffer: " << *len << std::endl;
		std::cout << "buffer: " << temp << std::endl;
	}
	//不为则返回0;
	else
	{
		return filetemp;
	}

	//剩余要接收的长度
	int all_size = *len;
	//工作指针
	char* ptr = filetemp;

	while (all_size > 0)
	{
		//开始接收文件数据本体
		if ((temp = recv(sockserver, ptr, all_size, 0)) != -1)
		{
			if (temp == 0)
			{
				std::cout << "服务器连接关闭:" << temp << std::endl;
				exit(-1);
			}
			std::cout << "接收到字节数:" << temp << std::endl;
			all_size -= temp;
			ptr += temp;
		}
	}

	return filetemp;
}

//接收数据
int Client_Box::RecvData(void* arg)
{
	Client_Box* client_box = (Client_Box*)arg;
	int ptr = 0;
	char str[] = "0.wav";
	while (true)
	{
		if (!client_box->arr[ptr])
		{
			str[0] = '0' + ptr;

			int len;
			char* buf = RecvNet(&len);
			MakeFile(str, buf, len);

			arr[ptr] = true;
			++ptr %= 10;
		}
		else
		{
			sleep(1);
		}
	}
	return true;
}

Client_Box::~Client_Box()
{
	close(sockserver);
}

int Client_Box::InitPlaySound()
{
	pthread_t  __newthread;

	int fd = pthread_create(&__newthread, NULL, Sound_static, this);
	pthread_detach(fd);

	return 0;
}

//循环播放语音
void* Client_Box::Sound_static(void* arg)
{
	Client_Box* client_box = (Client_Box*)arg;
	int ptr = 0;
	char str[] = "0.wav";
	while (true)
	{
		if (client_box->arr[ptr])
		{
			str[0] = '0' + ptr;
			system((std::string("play ") + str).c_str());
			client_box->arr[ptr] = false;
			++ptr %= 10;
		}
		else
		{
			usleep(100000);
		}
	}
	return nullptr;
}