#include"_stdlib_.h"

char buffer[1024] = { 0 };

int main(int argc, char* argv[])
{
	int server_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	//服务器地址
	sockaddr_in server_addr;

	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = inet_addr("192.168.137.1");
	server_addr.sin_port = ntohs(0x0721);

	std::cout << connect(server_sock, (sockaddr*)&server_addr, sizeof(sockaddr)) << std::endl;


	strcpy(buffer, "please server send client");
	std::cout << send(server_sock, buffer, strlen(buffer) + 1, 0);


	std::cout << send(server_sock, "buffer", 7, 0);
	close(server_sock);
	return 0;
}