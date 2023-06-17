#include"Client_Box.h"


//创建文件"槽",文件槽
void Client_Box::asyn_groove(int)
{
	if(filebuffer.)
}

inline int Client_Box::linkserver(const char* ip="192.168.137.1", int port= 0x0721)
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



static Head_code before;

