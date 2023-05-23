#include"_stdlib_.h"

char buffer[1024] = { 0 };

int main(int argc, char* argv[])
{
	int server_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	//服务器地址
	sockaddr_in server_addr;

	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = inet_addr("61.139.65.135");
	server_addr.sin_port = ntohs(33855);

	std::cout << connect(server_sock, (sockaddr*)&server_addr, sizeof(sockaddr)) << std::endl;

	std::cout << "server success" << buffer << std::endl;

	close(server_sock);
	return 0;
}