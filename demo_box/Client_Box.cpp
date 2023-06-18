#include"Client_Box.h"

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

	arr[path[0] - '0'] = true;
	return true;

}

char* Client_Box::RecvNet(int* len)
{
	//缓存区指针
	char* filetemp = nullptr;
	char* ptr;

	//数据帧大小为MTU-头帧大小
	const int size = TCP_MTU - sizeof(Head_code);
	while (recv(sockserver, buf, sizeof(Head_code), 0) != -1)
	{
		if (buf->target & READ)
		{
			//文件传输第一个包为信息包
			if (buf->group_num = 0)
			{
				//设置缓存区
				if (filetemp != nullptr)delete[] filetemp;
				filetemp = new char[buf->size];
			}
			else
			{

				//指向数据区指针
				ptr = (char*)(buf + 1);

				char* temp = filetemp + buf->group_num * size;
				memcpy(ptr, temp, size);
			}

		}
		else
		{
			return 0;
		}
	}
	return 0;
}

//接收数据
int Client_Box::recvdata(void* arg)
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
			sleep(1);
		}
	}
	return nullptr;
}



static Head_code before;

