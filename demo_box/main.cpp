#include"Client_Box.h"
int main(int argc, char* argv[])
{
	Client_Box client;
	//连接服务器
	int sockserver = client.linkserver();

	int ret;
	while (ret = recv(sockserver, client.buffer, TCP_MTU, 0))
	{
		//识别包不对丢弃数据包
		if (client.buf->DIY != 0xffee) continue;

		switch (client.buf->target)
		{
			//当收到读命令
		case READ:
			//异步创建缓存槽

			groove.detach();

			//





		}




	}

	MakeFile("test.wav", "测试", 7);
	MakeFile("test.wav", "err测试", 10);


	return 0;
}