#include"Client_Box.h"



void Client_Box::asyn_groove()
{

}

inline int Client_Box::linkserver(const char* ip, int port)
{
	int server_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	sockaddr_in server_addr;
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = inet_addr(ip);
	server_addr.sin_port = ntohs(port);

	if (connect(server_sock, (sockaddr*)&server_addr, sizeof(sockaddr)) != 0) return false;

	Head_code temp;

	send(server_sock, &temp, sizeof(temp), 0);

	return server_sock;
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
	return true;

}

int Client_Box::PlaySound()
{
	pthread_t  __newthread;

	int fd = pthread_create(&__newthread, NULL, Sound_static, this);

	void* temp;
	pthread_join(__newthread, &temp);
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

