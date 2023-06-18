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
inline int Client_Box::MakeFile(const char* path,char *file,int len)
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
	char* ptr;

	//数据帧大小为MTU-头帧大小
	const int size = TCP_MTU - sizeof(Head_code);
	int recv_size;


	buf->target = NEXT;
	//请求数据
	if (send(sockserver, buf, sizeof(Head_code), 0) == -1) return nullptr;

	while ((recv_size = recv(sockserver, buf, TCP_MTU, 0)) != -1)
	{
		if (buf->target == READ)
		{
			//文件传输第一个包为信息包
			if (buf->group_num = 0)
			{
				//设置缓存区
				if (filetemp != nullptr)delete[] filetemp;
				*len = buf->size;
				filetemp = new char[buf->size];
			}
			else
			{
				//指向数据区指针
				ptr = (char*)(buf + 1);

				//将指针移动到对应组号位置并拷贝数据
				char* temp = filetemp + (buf->group_num - 1) * size;
				memcpy(ptr, temp, size);

				//回复收到
				buf->target = READ | ACK;
				send(sockserver, buf, sizeof(Head_code), 0);
			}
		}
		//文件传输结束
		else if (buf->target == (READ | STOP))
		{
			//返回指针
			return filetemp;
		}
	}
	return filetemp;
}

//接收数据
int Client_Box::RecvData(void* arg)
{
	Client_Box* client_box = (Client_Box*)arg;
	int ptr = 0;
	char str[] = "0.mp3";
	while (true)
	{
		
		if (!client_box->arr[ptr])
		{
			str[0] = '0' + ptr;

			int len;
			char* buf = RecvNet(&len);
			MakeFile(str, buf,len);

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
	char str[] = "0.mp3";
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

