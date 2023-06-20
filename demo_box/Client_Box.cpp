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
	char* filetemp = buffer;

	int temp = 0;
	if ((temp = recv(sockserver, buffer, TCP_MTU, 0)) == -1)
	{
		std::cout << "接收错误" << std::endl;
	}

	if (buf->target == READ)
	{
		//文件传输第一个包为信息包
		*len = buf->size;
		filetemp = new char[buf->size + 10];

		std::cout << "prepare to down" << std::endl;
		std::cout << "buffer: " << *len << std::endl;
		std::cout << "buffer: " << temp << std::endl;
	}
	else
	{
		return filetemp;
	}

	if ((temp = recv(sockserver, filetemp, *len, 0)) == -1)
	{
		std::cout << "接收错误" << std::endl;
	}
	std::cout << temp << std::endl;

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